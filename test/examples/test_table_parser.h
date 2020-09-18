#pragma once
#include <unity/unity_fixture_classes.h>
#include "naive/table_state_machine.h"

class TestTableParser : UnityTestClass<TestTableParser> {
		static inline TableParser parser = TableParser();
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			parser.dispatch(TableParser::SIG_CHAR);
			parser.dispatch(TableParser::SIG_SLASH); //open 1
			parser.dispatch(TableParser::SIG_STAR);	//2
			parser.dispatch(TableParser::SIG_CHAR); //3
			parser.dispatch(TableParser::SIG_CHAR); //4
			parser.dispatch(TableParser::SIG_SLASH); //5
			parser.dispatch(TableParser::SIG_STAR); //close 6
			parser.dispatch(TableParser::SIG_SLASH); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestTableParser, newTest);
		}
};