#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/fsm_static_parser.h"

class TestStaticFsmParser : UnityTestClass<TestStaticFsmParser> {
		static inline StaticParserFsm parser = StaticParserFsm{};
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			//parser.dispatch(event_char{});
			parser.dispatch(event_slash{}); //open 1
			parser.dispatch(event_star{});//2
			parser.dispatch(event_char{}); //3
			parser.dispatch(event_char{}); //4
			parser.dispatch(event_slash{}); //5
			parser.dispatch(event_star{});//close 6
			parser.dispatch(event_slash{}); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestStaticFsmParser, newTest);
		}
};