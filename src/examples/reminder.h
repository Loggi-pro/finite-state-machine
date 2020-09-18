#pragma once
#include "fsm/hsm.h"
#include <iostream>
#include EVENT_QUEUE_HEADER


/*Example of nested HSM. HsmAlarm - internal HSM, which implements alarm clock. HsmAlarmClock - container HSM, which implements GUI, console representation, 12\24hr mode */
/*This example shows how to use nested HSMs.
1. Container HSM forwards event to internal alarm clock synchronously
2. Internal HSM fires event (SIG_ALARM) to container asynchronous, by global eventQueue
*/

class HsmSensor : public hsm::Hsm {
		uint32_t _comment_count;
		uint32_t _poll_counter;
		uint32_t _proc_counter;
		std::ostream& _stream;
	public:
		enum HsmSensorSignals {
			SIG_TIMEOUT = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
			SIG_DATA_READY,
			SIG_TERMINATE
		};
		HsmSensor(std::ostream& stream) : hsm::Hsm(hsm::as_state(&HsmSensor::initial)), _poll_counter(0),
			_proc_counter(0), _stream(stream) {}

		constexpr static hsm::Event const testEvent[] = {
			{HsmSensor::SIG_TIMEOUT}, {HsmSensor::SIG_DATA_READY}, {HsmSensor::SIG_TERMINATE}
		};
		template <HsmSensorSignals sig>
		constexpr static hsm::Event const& getEvent() {
			return testEvent[sig - hsm::ReservedSignals::SIG_USER];
		}

		constexpr hsm::EventResult initial(const hsm::Event& e) {
			return eventTransition(hsm::as_state(&HsmSensor::polling));
		}
		hsm::EventResult polling(const hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				//WAIT SIG_TIMEOUT 4 times
				return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT:
				return eventHandled();

			case hsm::ReservedSignals::SIG_INIT:
				return eventTransition(hsm::as_state(&HsmSensor::processing));

			case SIG_TIMEOUT:
				_poll_counter++;
				_stream << "polling\n";

				if (_poll_counter % 4 == 0) {
					eventQueue.putEvent(getEvent< SIG_DATA_READY>(), *this);
				}

				return eventHandled();

			case SIG_TERMINATE:
				return eventTransition(hsm::as_state(&HsmSensor::final));
			}

			return eventSuper(&Hsm::top);
		}

		hsm::EventResult processing(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_INIT:
				return eventTransition(hsm::as_state(&HsmSensor::idle));
			}

			return eventSuper(&HsmSensor::polling);
		}
		hsm::EventResult idle(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_stream << "idle-ENTRY\n";
				return eventHandled();

			case SIG_DATA_READY:
				return eventTransition(&HsmSensor::busy);
			}

			return eventSuper(&HsmSensor::processing);
		}
		hsm::EventResult busy(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_stream << "busy-ENTRY\n";
				return eventHandled();

			case SIG_TIMEOUT:
				_proc_counter++;
				_stream << "processing " << _proc_counter << "\n";

				//processing
				if (_proc_counter % 2 == 0) {
					return eventTransition(&HsmSensor::idle);
				}

				return eventHandled();
			}

			return eventSuper(&HsmSensor::processing);
		}
		hsm::EventResult final(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				return eventHandled();

			default :
				return eventSuper(&Hsm::top);
			}
		}
};