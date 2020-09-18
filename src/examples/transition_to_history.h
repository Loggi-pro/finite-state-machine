#pragma once
#include "fsm/hsm.h"
#include <iostream>
/*Example of toaster, which save own state when open door, and return to this state when door is closed*/

enum ToasterMode { TOASTERMODE_NONE, TOASTERMODE_BAKE, TOASTERMODE_TOAST};

class HsmToasterOven : public hsm::Hsm {
	public:
		enum HsmToasterOvenSignal {
			SIG_OPEN = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
			SIG_CLOSE, SIG_TOAST, SIG_BAKE, SIG_OFF, SIG_TERMINATE
		};
		hsm::StateHandler _doorClosedHistory;
		std::ostream& _stream;
		bool _isDoorOpen;
		bool _isLampOn;
		bool _isHeaterOn;
		ToasterMode _mode;
		HsmToasterOven(std::ostream& stream) : Hsm(hsm::as_state(&HsmToasterOven::initial)),
			_doorClosedHistory(hsm::as_state(&HsmToasterOven::off)),
			_stream(stream), _isDoorOpen(false), _isLampOn(false), _isHeaterOn(false),
			_mode(TOASTERMODE_NONE) {}
		bool isDoorOpen()const {
			return _isDoorOpen;
		}
		bool isLampOn()const {
			return _isLampOn;
		}
		bool isHeaterOn()const {
			return _isHeaterOn;
		}
		bool isToasting()const {
			return _mode == TOASTERMODE_TOAST;
		}
		bool isBaking()const {
			return _mode == TOASTERMODE_BAKE;
		}
	private:
		hsm::EventResult initial(const hsm::Event& e) {
			_doorClosedHistory = hsm::as_state(&HsmToasterOven::off);
			return eventTransition(&HsmToasterOven::doorClosed);
		}
		hsm::EventResult doorOpen(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "door open, lamp on\n";
				_isDoorOpen = true;
				_isLampOn = true;
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				_stream << "lamp off;";
				_isLampOn = false;
				_doorClosedHistory = hsm::as_state(&HsmToasterOven::toasting);
				return eventHandled();
			}

			case SIG_CLOSE: {
				//when door closed ->return to last state
				return eventTransition(_doorClosedHistory);
			}
			}

			return eventSuper(&HsmToasterOven::top);
		}
		hsm::EventResult off(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "toaster off\n";
				_doorClosedHistory = hsm::as_state(&HsmToasterOven::off);
				_mode = TOASTERMODE_NONE;
				return eventHandled();
			}
			}

			return eventSuper(&HsmToasterOven::doorClosed);
		}
		hsm::EventResult heating(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "heater on;";
				_isHeaterOn = true;
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				_stream << "heater off;";
				_isHeaterOn = false;
				return eventHandled();
			}
			}

			return eventSuper(&HsmToasterOven::doorClosed);
		}
		hsm::EventResult toasting(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "toasting\n";
				_mode = TOASTERMODE_TOAST;
				_doorClosedHistory = hsm::as_state(&HsmToasterOven::toasting);
				return eventHandled();
			}
			}

			return eventSuper(&HsmToasterOven::heating);
		}
		hsm::EventResult baking(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "baking\n";
				_mode = TOASTERMODE_BAKE;
				_doorClosedHistory = hsm::as_state(&HsmToasterOven::baking);
				return eventHandled();
			}
			}

			return eventSuper(&HsmToasterOven::heating);
		}
		hsm::EventResult doorClosed(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "door closed;";
				_isDoorOpen = false;
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_INIT: {
				return eventTransition(&HsmToasterOven::off);
			}

			case SIG_OPEN: {
				return eventTransition(&HsmToasterOven::doorOpen);
			}

			case SIG_TOAST: {
				return eventTransition(&HsmToasterOven::toasting);
			}

			case SIG_BAKE: {
				return eventTransition(&HsmToasterOven::baking);
			}

			case SIG_OFF: {
				return eventTransition(&HsmToasterOven::off);
			}

			case SIG_TERMINATE: {
				return eventTransition(&HsmToasterOven::final);
			}
			}

			return eventSuper(&Hsm::top);
		}

		hsm::EventResult final(const  hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "->final\n";
				//TODO terminate app
				return eventHandled();
			}
			}

			return eventSuper(&Hsm::top);
		}
};