#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/reminder.h"

/* usage
	HsmSensor sensor;
	sensor.init();
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	eventQueue.checkQueue();
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	//
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	eventQueue.checkQueue();
	sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
	sensor.dispatch({ HsmSensor::SIG_TERMINATE });
*/


/* result
idle - ENTRY
polling
polling
polling
polling
busy - ENTRY
processing 1
processing 2
idle - ENTRY
polling
polling
polling
polling
busy - ENTRY
processing 3
*/



class TestReminderHsm : UnityTestClass<TestReminderHsm> {
		static inline std::ostream nullstream{ 0 };
		static inline HsmSensor sensor = HsmSensor{ nullstream };
	public:
		static void setup() {
			sensor.init();
			eventQueue.clear();
		}
		static void tearDown() {
		}

		static void newTest() {
			TEST_ASSERT(sensor.isInState(&HsmSensor::idle));
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			TEST_ASSERT(sensor.isInState(&HsmSensor::polling));
			eventQueue.checkQueue();
			TEST_ASSERT(sensor.isInState(&HsmSensor::busy));
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			//
			TEST_ASSERT(sensor.isInState(&HsmSensor::idle));
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			TEST_ASSERT(sensor.isInState(&HsmSensor::polling));
			eventQueue.checkQueue();
			TEST_ASSERT(sensor.isInState(&HsmSensor::busy));
			sensor.dispatch({ HsmSensor::SIG_TIMEOUT });
			sensor.dispatch({ HsmSensor::SIG_TERMINATE });
			TEST_ASSERT(sensor.isInState(&HsmSensor::final));
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestReminderHsm, newTest);
		}
};

