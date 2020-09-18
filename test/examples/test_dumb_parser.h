#pragma once
#include <unity/unity_fixture_classes.h>
#include "naive/dumb_state_machine.h"

class TestDumbParser : UnityTestClass<TestDumbParser> {
		static inline DumbParser parser = DumbParser();
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			parser.dispatch(DumbParser::SIG_CHAR);
			parser.dispatch(DumbParser::SIG_SLASH); //open 1
			parser.dispatch(DumbParser::SIG_STAR);	//2
			parser.dispatch(DumbParser::SIG_CHAR); //3
			parser.dispatch(DumbParser::SIG_CHAR); //4
			parser.dispatch(DumbParser::SIG_SLASH); //5
			parser.dispatch(DumbParser::SIG_STAR); //close 6
			parser.dispatch(DumbParser::SIG_SLASH); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestDumbParser, newTest);
		}
};