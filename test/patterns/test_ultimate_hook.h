#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/ultimate_hook.h"

/* usage

	UltimateHookSpecific testHook;
	testHook.init();
	testHook.dispatch(UltimateHookSpecific::getEvent<UltimateHookSpecific::SIG_A>());
	testHook.dispatch(UltimateHookSpecific::getEvent<UltimateHookSpecific::SIG_B>());
	testHook.dispatch(UltimateHookSpecific::getEvent<UltimateHookSpecific::SIG_C>());
	testHook.dispatch(UltimateHookSpecific::getEvent<UltimateHookSpecific::SIG_D>());
*/

/* result
top-INIT
generic-INIT
specific-ENTRY
specific-A
generic-B
generic-C(reset)
specific-EXIT
generic-INIT
specific-ENTRY
generic-D(terminate)
specific-EXIT
final-ENTRY(terminate)
*/


class TestUltimateHookHsm : UnityTestClass<TestUltimateHookHsm> {
		static inline std::ostream nullstream{ 0 };

		static inline UltimateHookSpecific testHook = UltimateHookSpecific{ nullstream};
	public:
		static void setup() {
			testHook.init();
			eventQueue.clear();
		}
		static void tearDown() {
		}


		static void newTest() {
			testHook.dispatch({ UltimateHookSpecific::SIG_A });
			TEST_ASSERT_EQUAL(A_SPECIFIC, testHook.getLastEvent());
			testHook.dispatch({ UltimateHookSpecific::SIG_B });
			TEST_ASSERT_EQUAL(B_GENERIC, testHook.getLastEvent());
			testHook.dispatch({ UltimateHookSpecific::SIG_C });
			TEST_ASSERT_EQUAL(C_GENERIC, testHook.getLastEvent());
			testHook.dispatch({ UltimateHookSpecific::SIG_D });
			TEST_ASSERT_EQUAL(D_GENERIC, testHook.getLastEvent());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestUltimateHookHsm, newTest);
		}
};

