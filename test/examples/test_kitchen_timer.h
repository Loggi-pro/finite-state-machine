#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/kitchen_timer.h"



class TestKitchenTimerHsm : UnityTestClass<TestKitchenTimerHsm> {
		static inline std::ostream nullstream{ 0 };
		static inline HsmKitchenTimer timer{7, nullstream };
		static inline int alarmHappened = 0;
	public:
		static void setup() {
			timer.init();
			eventQueue.clear();
			alarmHappened = 0;
		}
		static void tearDown() {
		}


		static void newTest() {
			timer.setOnAlarm([]() {alarmHappened++; });
			TEST_ASSERT_EQUAL(30, timer.getCurrentTime());
			timer.dispatch({ HsmKitchenTimer::SIG_UP });
			TEST_ASSERT_EQUAL(31, timer.getCurrentTime());
			timer.dispatch({ HsmKitchenTimer::SIG_DOWN });
			TEST_ASSERT_EQUAL(30, timer.getCurrentTime());
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 1 });
			TEST_ASSERT_EQUAL(30, timer.getCurrentTime());
			//apply
			//test tick
			timer.dispatch({ HsmKitchenTimer::SIG_APPLY});
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 1 });
			TEST_ASSERT_EQUAL(29, timer.getCurrentTime());
			//test wrong code
			timer.dispatch({ HsmKitchenTimer::SIG_UP });
			timer.dispatch({ HsmKitchenTimer::SIG_UP });
			timer.dispatch({ HsmKitchenTimer::SIG_APPLY });
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 1 });
			TEST_ASSERT_EQUAL(28, timer.getCurrentTime()); //not disabled
			//should disable
			timer.dispatch({ HsmKitchenTimer::SIG_UP });
			timer.dispatch({ HsmKitchenTimer::SIG_APPLY });
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 1 });
			TEST_ASSERT_EQUAL(28, timer.getCurrentTime()); //disabled
			//test alarm
			timer.dispatch({ HsmKitchenTimer::SIG_APPLY });
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 27 });
			TEST_ASSERT_EQUAL(0, alarmHappened);
			TEST_ASSERT_EQUAL(1, timer.getCurrentTime());
			timer.dispatch(HsmKitchenTimer::EventTick{ HsmKitchenTimer::SIG_TICK, 1 });
			TEST_ASSERT_EQUAL(1, alarmHappened);
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestKitchenTimerHsm, newTest);
		}
};

