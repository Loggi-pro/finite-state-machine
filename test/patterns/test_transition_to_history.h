#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/transition_to_history.h"

/* usage
HsmToasterOven toaster;
toaster.init();
std::cout << "b:";
toaster.dispatch({ HsmToasterOven::SIG_BAKE });
std::cout << "o:";
toaster.dispatch({ HsmToasterOven::SIG_OPEN });
std::cout << "c:";
toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
std::cout << "t:";
toaster.dispatch({ HsmToasterOven::SIG_TOAST });
std::cout << "o:";
toaster.dispatch({ HsmToasterOven::SIG_OPEN });
std::cout << "c:";
toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
std::cout << "f:";
toaster.dispatch({ HsmToasterOven::SIG_OFF });
std::cout << "o:";
toaster.dispatch({ HsmToasterOven::SIG_OPEN });
std::cout << "c:";
toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
*/


/* result
door closed; toaster off
b : heater on; baking
o : heater off; door open, lamp on
c : lamp off; door closed; heater on; baking
t : heater off; heater on; toasting
o : heater off; door open, lamp on
c : lamp off; door closed; heater on; toasting
f : heater off; toaster off
o : door open, lamp on
c : lamp off; door closed; toaster off
*/


class TestTransitionToHistoryHsm : UnityTestClass<TestTransitionToHistoryHsm> {
		static inline std::ostream nullstream{ 0 };
		static inline HsmToasterOven toaster = HsmToasterOven{ nullstream };
	public:
		static void setup() {
			toaster.init();
			eventQueue.clear();
		}
		static void tearDown() {
		}

		static void newTest() {
			toaster.dispatch({ HsmToasterOven::SIG_BAKE });
			TEST_ASSERT(toaster.isHeaterOn());
			TEST_ASSERT(toaster.isBaking());
			toaster.dispatch({ HsmToasterOven::SIG_OPEN });
			TEST_ASSERT(!toaster.isHeaterOn());
			TEST_ASSERT(toaster.isDoorOpen());
			TEST_ASSERT(toaster.isLampOn());
			toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
			TEST_ASSERT(toaster.isHeaterOn());
			TEST_ASSERT(!toaster.isDoorOpen());
			TEST_ASSERT(!toaster.isLampOn());
			TEST_ASSERT(toaster.isBaking());
			toaster.dispatch({ HsmToasterOven::SIG_TOAST });
			TEST_ASSERT(toaster.isHeaterOn());
			TEST_ASSERT(!toaster.isDoorOpen());
			TEST_ASSERT(!toaster.isLampOn());
			TEST_ASSERT(toaster.isToasting());
			toaster.dispatch({ HsmToasterOven::SIG_OPEN });
			TEST_ASSERT(!toaster.isHeaterOn());
			TEST_ASSERT(toaster.isDoorOpen());
			TEST_ASSERT(toaster.isLampOn());
			TEST_ASSERT(toaster.isToasting());
			toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
			TEST_ASSERT(toaster.isHeaterOn());
			TEST_ASSERT(!toaster.isDoorOpen());
			TEST_ASSERT(!toaster.isLampOn());
			TEST_ASSERT(toaster.isToasting());
			toaster.dispatch({ HsmToasterOven::SIG_OFF });
			TEST_ASSERT(!toaster.isHeaterOn());
			TEST_ASSERT(!toaster.isDoorOpen());
			TEST_ASSERT(!toaster.isLampOn());
			TEST_ASSERT(!toaster.isToasting());
			TEST_ASSERT(!toaster.isBaking());
			toaster.dispatch({ HsmToasterOven::SIG_OPEN });
			TEST_ASSERT(!toaster.isHeaterOn());
			TEST_ASSERT(toaster.isDoorOpen());
			TEST_ASSERT(toaster.isLampOn());
			TEST_ASSERT(!toaster.isToasting());
			TEST_ASSERT(!toaster.isBaking());
			toaster.dispatch({ HsmToasterOven::SIG_CLOSE });
			TEST_ASSERT(!toaster.isHeaterOn());
			TEST_ASSERT(!toaster.isDoorOpen());
			TEST_ASSERT(!toaster.isLampOn());
			TEST_ASSERT(!toaster.isToasting());
			TEST_ASSERT(!toaster.isBaking());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestTransitionToHistoryHsm, newTest);
		}
};

