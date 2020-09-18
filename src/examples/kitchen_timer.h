#pragma once
#include "fsm/hsm.h"
#include <iostream>
#include EVENT_QUEUE_HEADER
//Kitchen timer, which disables by using keyboard


class HsmKitchenTimer : public hsm::Hsm {
	public:
		enum TTimerSignals {
			SIG_UP = hsm::ReservedSignals::SIG_USER,
			SIG_DOWN,
			SIG_APPLY,
			SIG_TICK
		};
		enum TimerState {
			STATE_SETTING,
			STATE_TIMING
		};
		struct EventTick : hsm::Event {
			uint8_t tick_seconds;
		};
		constexpr static uint8_t INIT_TIMEOUT = 30;
		TimerState _currentState;
		uint8_t _timeoutSec; //seconds till end
		uint8_t _codeToDisable; //entered code to disable timer
		uint8_t _defuse; //secret code to disable timer
		std::ostream& _stream;
		typedef void(*ptr_cbAlarm)();
		ptr_cbAlarm _cbOnAlarm;

		HsmKitchenTimer(uint8_t defuse,
		                std::ostream& stream) : Hsm(hsm::as_state(&HsmKitchenTimer::initial)),
			_defuse(defuse), _stream(stream), _cbOnAlarm(nullptr) {} /* the default ctor */

		uint8_t getCurrentTime() const {
			return _timeoutSec;
		}
		void setOnAlarm(ptr_cbAlarm f) {
			_cbOnAlarm = f;
		}
	private:


		hsm::EventResult initial(const  hsm::Event& e) {
			_timeoutSec = INIT_TIMEOUT;
			return eventTransition(&HsmKitchenTimer::setting);
		}
		hsm::EventResult setting(const  hsm::Event& e) {
			switch (e.sig) {
			case SIG_UP:
				if (_timeoutSec < 60) {
					_timeoutSec++;
					_stream << "timeout=" << _timeoutSec << "\n";
				}

				return eventHandled();

			case SIG_DOWN:
				if (_timeoutSec > 1) {
					_timeoutSec--;
					_stream << "timeout=" << _timeoutSec << "\n";
				}

				return eventHandled();

			case SIG_APPLY:
				return eventTransition(&HsmKitchenTimer::timing);
			}

			return eventSuper(&HsmKitchenTimer::top);
		}
		hsm::EventResult timing(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_codeToDisable = 0;
				return eventHandled();

			case SIG_UP:
				_codeToDisable <<= 1;
				_codeToDisable |= 1;
				return eventHandled();

			case SIG_DOWN:
				_codeToDisable <<= 1;
				return eventHandled();

			case SIG_APPLY:
				if (_codeToDisable == _defuse) {
					return eventTransition(&HsmKitchenTimer::setting);
				}

				return eventHandled();

			case SIG_TICK: {
				const EventTick& t = (const EventTick&)e;

				if (t.tick_seconds <= _timeoutSec) {
					_timeoutSec -= t.tick_seconds;
				} else {
					_timeoutSec = 0;
				}

				_stream << "time:" << _timeoutSec << "\n";

				if (_timeoutSec == 0) {
					_stream << "ALARM!";

					if (_cbOnAlarm) { _cbOnAlarm(); }

					return eventTransition(&HsmKitchenTimer::setting);
				}

				return eventHandled();
			}
			}

			return eventSuper(&HsmKitchenTimer::top);
		}


		hsm::EventResult final(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "final-ENTRY" << "\n";
				//TODO terminate application here
				return eventHandled();
			}
			}

			return eventSuper(&HsmKitchenTimer::top);
		}

};
