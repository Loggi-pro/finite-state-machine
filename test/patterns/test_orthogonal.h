#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/orthogonal.h"


/* usage
	HsmAlarmClock clock;
	clock.init();
	clock.dispatch(EventTime{ SIG_TICK, 1 });
	clock.dispatch({ SIG_ALARM_ON});
	clock.dispatch(EventTime{ SIG_TICK, 2 });
	clock.dispatch({ SIG_ALARM_ON });
	clock.dispatch(EventTime{ SIG_TICK, 3 });
	clock.dispatch({ SIG_ALARM_OFF });
	clock.dispatch(EventTime{ SIG_TICK, 4 });
	clock.dispatch(EventSet{ SIG_ALARM_SET, 20 * 60 + 1 });
	clock.dispatch(EventTime{ SIG_TICK, 5 });
	clock.dispatch(EventSet{ SIG_ALARM_SET, 10 });
	clock.dispatch(EventTime{ SIG_TICK, 6 });
	clock.dispatch({ SIG_ALARM_ON });
	clock.dispatch(EventTime{ SIG_TICK, 7 });
	clock.dispatch(EventTime{ SIG_TICK, 8 });
	clock.dispatch({ SIG_CLOCK_12H });
	clock.dispatch(EventTime{ SIG_TICK, 9 });
	clock.dispatch(EventTime{ SIG_TICK, 10 });
	eventQueue.checkQueue();
	clock.dispatch(EventTime{ SIG_TICK, 11 });
	clock.dispatch(EventTime{ SIG_TICK, 12 });
*/


/* result
Alarm off 12:0
24 hour mode
0:1
Alarm ON 12:0
0:2
0:3
Alarm off 12:0
0:4
Alarm SET 0:0
0:5
Alarm SET 0:10
0:6
Alarm ON 0:10
0:7
0:8
12 hour mode
12:9 AM
12:10 AM
ALARM FROM CLOCK!
WAKE UP FROM CONTAINER!
12:11 AM
12:12 AM
*/

class TestOrthogonalHsm : UnityTestClass<TestOrthogonalHsm> {
		static inline std::ostream nullstream{ 0 };
		static inline HsmAlarmClock clock = HsmAlarmClock{ nullstream };
	public:
		static void setup() {
			clock.init();
			eventQueue.clear();
		}
		static void tearDown() {
		}


		static void newTest() {
			TEST_ASSERT_EQUAL(CLOCKMODE_24H, clock.getClockMode());
			TEST_ASSERT_EQUAL(ALARM_OFF, clock.getAlarmMode());
			clock.dispatch(EventTime{ SIG_TICK, 1 });
			clock.dispatch({ SIG_ALARM_ON });
			TEST_ASSERT_EQUAL(ALARM_ON, clock.getAlarmMode());
			clock.dispatch(EventTime{ SIG_TICK, 2 });
			clock.dispatch({ SIG_ALARM_ON });
			clock.dispatch(EventTime{ SIG_TICK, 3 });
			clock.dispatch({ SIG_ALARM_OFF });
			TEST_ASSERT_EQUAL(ALARM_OFF, clock.getAlarmMode());
			clock.dispatch(EventTime{ SIG_TICK, 4 });
			clock.dispatch(EventSet{ SIG_ALARM_SET, 20 * 60 + 1 });
			clock.dispatch(EventTime{ SIG_TICK, 5 });
			clock.dispatch(EventSet{ SIG_ALARM_SET, 10 });
			clock.dispatch(EventTime{ SIG_TICK, 6 });
			clock.dispatch({ SIG_ALARM_ON });
			TEST_ASSERT_EQUAL(ALARM_ON, clock.getAlarmMode());
			clock.dispatch(EventTime{ SIG_TICK, 7 });
			clock.dispatch(EventTime{ SIG_TICK, 8 });
			clock.dispatch({ SIG_CLOCK_12H });
			TEST_ASSERT_EQUAL(CLOCKMODE_12H, clock.getClockMode());
			clock.dispatch(EventTime{ SIG_TICK, 9 });
			clock.dispatch(EventTime{ SIG_TICK, 10 });
			TEST_ASSERT(!clock.isAlarm());
			eventQueue.checkQueue();
			TEST_ASSERT(clock.isAlarm());
			clock.dispatch(EventTime{ SIG_TICK, 11 });
			clock.dispatch(EventTime{ SIG_TICK, 12 });
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestOrthogonalHsm, newTest);
		}
};

