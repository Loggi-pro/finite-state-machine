#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/fsm_parser.h"

class TestFsmParser : UnityTestClass<TestFsmParser> {
		static inline OptimalParserFsm parser = OptimalParserFsm();
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			parser.dispatch({ OptimalParserFsm::SIG_CHAR });
			parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //open 1
			parser.dispatch({ OptimalParserFsm::SIG_STAR });//2
			parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //3
			parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //4
			parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //5
			parser.dispatch({ OptimalParserFsm::SIG_STAR });//close 6
			parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestFsmParser, newTest);
		}
};