#pragma once
#include "fsm/hsm.h"
#include <iostream>

/*
Make base class UltimateHookGeneric with base logic

Make derived class with specified logic behaviour in some states : UltimateHookSpecific
*/
enum LastCalledEvent {UNKNOWN, A_GENERIC, B_GENERIC, C_GENERIC, D_GENERIC, A_SPECIFIC};
template <typename Derived> //CRTP
class UltimateHookGeneric : public hsm::Hsm {
		friend Derived;
	protected:
		std::ostream& _stream;
		LastCalledEvent _lastEvent;
	public:
		enum UltimateHookSignals {
			SIG_A = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
			SIG_B, SIG_C, SIG_D
		};
		UltimateHookGeneric(std::ostream& stream) : hsm::Hsm(hsm::as_state(
			            &UltimateHookGeneric::initial)), _stream(stream), _lastEvent(UNKNOWN) {}

		constexpr static hsm::Event const testEvent[] = {
			{SIG_A}, {SIG_B}, {SIG_C}, {SIG_D}
		};
		template <UltimateHookSignals sig>
		constexpr static hsm::Event const& getEvent() {
			return testEvent[sig - hsm::ReservedSignals::SIG_USER];
		}
		LastCalledEvent getLastEvent()const {
			return _lastEvent;
		}
	private:
		hsm::EventResult initial(const hsm::Event& e) {
			_stream << "top-INIT\n";
			return eventTransition(hsm::as_state(&UltimateHookGeneric::generic));
		}

		//terminate state
		hsm::EventResult final(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_stream << "final-ENTRY(terminate)\n";
				return eventHandled();
			}

			return eventSuper(&UltimateHookGeneric::top);
		}
		hsm::EventResult generic(const hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_INIT:
				_stream << "generic-INIT\n";
				return eventTransition(hsm::as_state(&UltimateHookGeneric::specific));

			case SIG_A:
				_stream << "generic-A\n";
				_lastEvent = A_GENERIC;
				return eventHandled();

			case SIG_B:
				_stream << "generic-B\n";
				_lastEvent = B_GENERIC;
				return eventHandled();

			case SIG_C://reset idion
				_stream << "generic-C(reset)\n";
				_lastEvent = C_GENERIC;
				return eventTransition(hsm::as_state(&UltimateHookGeneric::generic));

			case SIG_D:
				_stream << "generic-D(terminate)\n";
				_lastEvent = D_GENERIC;
				return eventTransition(hsm::as_state(&UltimateHookGeneric::final));
			}

			return eventSuper(&Hsm::top);
		}
		//in specific state override some behaviour (may be virtual
		//virtual hsm::EventResult v_specific(const hsm::Event* e) = 0;
		hsm::EventResult specific(const hsm::Event& e) { //state handler
			return ((Derived*)this)->specific(e);
		}

};

class UltimateHookSpecific : public UltimateHookGeneric< UltimateHookSpecific> {
		friend UltimateHookGeneric< UltimateHookSpecific>;
	public:
		UltimateHookSpecific(std::ostream& stream): UltimateHookGeneric<UltimateHookSpecific>(stream) {}
	private:
		//override base class behaviour in some states(can be virtual function)
		hsm::EventResult specific(const hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_stream << "specific-ENTRY\n";
				return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT:
				_stream << "specific-EXIT\n";
				return eventHandled();

			case SIG_A:
				_stream << "specific-A\n";
				_lastEvent = A_SPECIFIC;
				return eventHandled();
			}

			return eventSuper(&UltimateHookGeneric::generic);
		}

};