#include <unity/unity_fixture.h>
#include "examples/test_dumb_parser.h"
#include "examples/test_table_parser.h"
#include "examples/test_pattern_parser.h"
#include "examples/test_fsm_parser.h"
#include "examples/test_static_fsm_parser.h"
#include "examples/test_hsm_parser.h"
#include "examples/test_kitchen_timer.h"
#include "patterns/test_deffered_event.h"
#include "patterns/test_orthogonal.h"
#include "patterns/test_reminder.h"
#include "patterns/test_transition_to_history.h"
#include "patterns/test_ultimate_hook.h"
#include "void/variant.h"
#include <variant>
#include EVENT_QUEUE_HEADER

HsmEventQueue eventQueue;


struct X {
	int x, y;
};


using myvar = vd::variant<int, float, X>;

void print(const myvar& v) {
	vd::visit(vd::overloaded{
		[](int v) {
			std::cout << "int:" << v << "\n";
		},
		[](float v) {
			std::cout << "float:" << v << "\n";
		},
		[](const X & v) {
			std::cout << "X:" << v.x << "," << v.y << "\n";
		}
	}, v);
}



static void RunAllTests(void) {
	print(myvar{1});
	print(myvar{2.f});
	print(myvar{X{ 3, 4 }});
	RUN_TEST_GROUP_CLASS(TestDumbParser);
	RUN_TEST_GROUP_CLASS(TestTableParser);
	RUN_TEST_GROUP_CLASS(TestPatternParser);
	RUN_TEST_GROUP_CLASS(TestFsmParser);
	RUN_TEST_GROUP_CLASS(TestStaticFsmParser);
	RUN_TEST_GROUP_CLASS(TestHsmParser);
	RUN_TEST_GROUP_CLASS(TestDefferedEventHsm);
	RUN_TEST_GROUP_CLASS(TestReminderHsm);
	RUN_TEST_GROUP_CLASS(TestOrthogonalHsm);
	RUN_TEST_GROUP_CLASS(TestTransitionToHistoryHsm);
	RUN_TEST_GROUP_CLASS(TestUltimateHookHsm);
	RUN_TEST_GROUP_CLASS(TestKitchenTimerHsm);
}



int main(int argc, const char* argv[]) {
	UnityMain(argc, argv, RunAllTests);
}
