#pragma once
#include "fsm/hsm.h"
#include <iostream>
#include EVENT_QUEUE_HEADER
/*Example of nested HSM. HsmAlarm - internal HSM, which implements alarm clock. HsmAlarmClock - container HSM, which implements GUI, console representation, 12\24hr mode */
/*This example shows how to use nested HSMs.
1. Container HSM forwards event to internal alarm clock synchronously
2. Internal HSM fires event (SIG_ALARM) to container asynchronous, by global eventQueue
*/

enum HsmAlarmSignals {
	SIG_TICK = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
	SIG_ALARM_SET, SIG_ALARM_ON, SIG_ALARM_OFF, SIG_ALARM, SIG_CLOCK_12H, SIG_CLOCK_24H, SIG_TERMINATE
};
struct EventSet : hsm::Event {
	uint32_t digit;
};
struct EventTime : hsm::Event {
	uint32_t currentTime;
};
enum ClockMode {
	CLOCKMODE_12H, CLOCKMODE_24H
};
enum AlarmMode {
	ALARM_ON, ALARM_OFF
};

class HsmAlarm : public hsm::Hsm {
	public:
		HsmAlarm(hsm::Hsm* container, std::ostream& stream) : Hsm(hsm::as_state(&HsmAlarm::initial)),
			_container(container), _stream(stream), _mode(ALARM_OFF)  {}
		AlarmMode getMode()const {
			return _mode;
		}
	private:
		uint32_t _alarmTime;
		hsm::Hsm* _container;
		hsm::Event _alarmEvent;
		std::ostream& _stream;
		AlarmMode _mode;
		hsm::EventResult initial(const hsm::Event& e) {
			_alarmTime = 12 * 60;
			return eventTransition(&HsmAlarm::off);
		}
		hsm::EventResult off(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				// while in the off state, the alarm is kept in decimal format
				_alarmTime = (_alarmTime / 60) * 100 + _alarmTime % 60;
				_stream << "Alarm off " << _alarmTime / 100 << ":" << _alarmTime % 100 << "\n";
				_mode = ALARM_OFF;
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				//upon exit, the alarm is converted to binary format
				_alarmTime = (_alarmTime / 100) * 60 + _alarmTime % 100;
				return eventHandled();
			}

			case SIG_ALARM_ON: {
				return eventTransition(&HsmAlarm::on);
			}

			case SIG_ALARM_SET: {
				//while setting, the alarm is kept in decimal format
				const EventSet& se = (const EventSet&)e;
				uint32_t alarm = (10 * _alarmTime + se.digit) % 10000;

				if ((alarm / 100 < 24) && (alarm % 100 < 60)) {//alarm in range?
					_alarmTime = alarm;
				} else { //out of range->start over
					_alarmTime = 0;
				}

				_stream << "Alarm SET " << _alarmTime / 100 << ":" << _alarmTime % 100 << "\n";
				return eventHandled();
			}
			}

			return eventSuper(&Hsm::top);
		}
		hsm::EventResult on(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "Alarm ON " << _alarmTime / 60 << ":" << _alarmTime % 60 << "\n";
				_mode = ALARM_ON;
				return eventHandled();
			}

			case SIG_ALARM_SET: {
				_stream << "Cannot set Alarm when it is ON\n";
				return eventHandled();
			}

			case SIG_ALARM_OFF: {
				return eventTransition(&HsmAlarm::off);
			}

			case SIG_TICK: {
				auto& te = (const EventTime&)e;

				if (te.currentTime == _alarmTime) {
					_stream << "ALARM FROM CLOCK!\n";
					//post FIFO asynchronoues event for container->SIG_ALARM
					_alarmEvent.sig = SIG_ALARM;
					eventQueue.putEvent(_alarmEvent, *_container);
				}

				return eventHandled();
			}
			}

			return eventSuper(&Hsm::top);
		}
};


class HsmAlarmClock : public hsm::Hsm {
	public:
		HsmAlarmClock(std::ostream& stream) : _alarm(this, stream),
			Hsm(hsm::as_state(&HsmAlarmClock::initial)),
			_currentTime(0), _stream(stream), _mode(CLOCKMODE_24H) {}
		AlarmMode getAlarmMode()const {
			return _alarm.getMode();
		}
		ClockMode getClockMode() const {
			return _mode;
		}
		bool isAlarm() {
			if (_alarmHappened) {
				_alarmHappened = false;
				return true;
			} else {
				return false;
			}
		}
	private:
		HsmAlarm _alarm;
		uint32_t _currentTime;
		std::ostream& _stream;
		ClockMode _mode;
		bool _alarmHappened;
		hsm::EventResult initial(const hsm::Event& e) {
			_alarm.init();
			return eventTransition(&HsmAlarmClock::timekeeping);
		}
		hsm::EventResult timekeeping(const  hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				//periodic timeout every second
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_INIT: {
				return eventTransition(&HsmAlarmClock::mode24hr);
			}

			case SIG_CLOCK_12H: {
				return eventTransition(&HsmAlarmClock::mode12hr);
			}

			case SIG_CLOCK_24H: {
				return eventTransition(&HsmAlarmClock::mode24hr);
			}

			case SIG_ALARM: { //it comes from _alarm object
				_stream << "WAKE UP FROM CONTAINER!\n";
				_alarmHappened = true;
				return eventHandled();
			}

			case SIG_ALARM_SET:
			case SIG_ALARM_ON:
			case SIG_ALARM_OFF:
				//synchronously dispatch to the orthogonal component
				_alarm.dispatch(e);
				return eventHandled();

			case SIG_TERMINATE: {
				return eventTransition(&HsmAlarmClock::final);
			}
			}

			return eventSuper(&Hsm::top);
		}

		hsm::EventResult mode24hr(const  hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "24 hour mode\n";
				_mode = CLOCKMODE_24H;
				return eventHandled();
			}

			case SIG_TICK: {
				++_currentTime;
				_currentTime %= 24 * 60; //roll over
				_stream << _currentTime / 60 << ":" << _currentTime % 60 << "\n";
				EventTime te; //temp sync event for the component
				te.sig = SIG_TICK;
				te.currentTime = _currentTime;
				//sync dispatch to the orthogonal component
				_alarm.dispatch(te);
				return eventHandled();
			}
			}

			return eventSuper(&HsmAlarmClock::timekeeping);
		}
		hsm::EventResult mode12hr(const  hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "12 hour mode\n";
				_mode = CLOCKMODE_12H;
				return eventHandled();
			}

			case SIG_TICK: {
				++_currentTime;
				_currentTime %= 12 * 60; //roll over
				uint32_t h = _currentTime / 60;
				uint32_t h_val = (h % 12) ? (h % 12) : 12;
				const char* s = (h / 12) ? "PM" : "AM";
				_stream << h_val << ":" << _currentTime % 60 << " " << s << "\n";
				EventTime te; //temp sync event for the component
				te.sig = SIG_TICK;
				te.currentTime = _currentTime;
				//sync dispatch to the orthogonal component
				_alarm.dispatch(te);
				return eventHandled();
			}
			}

			return eventSuper(&HsmAlarmClock::timekeeping);
		}


		hsm::EventResult final(const  hsm::Event& e) {  //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "final\n";
				//TODO terminate task
				return eventHandled();
			}
			}

			return eventSuper(&Hsm::top);
		}
};

