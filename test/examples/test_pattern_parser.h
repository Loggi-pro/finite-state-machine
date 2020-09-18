#pragma once
#include <unity/unity_fixture_classes.h>
#include "naive/state_pattern_machine.h"

class TestPatternParser : UnityTestClass<TestPatternParser> {
		static inline Parser parser = Parser();
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			parser.onCHAR(1);
			parser.onSLASH(); //open 1
			parser.onSTAR();	//2
			parser.onCHAR(1); //3
			parser.onCHAR(1); //4
			parser.onSLASH(); //5
			parser.onSTAR(); //close 6
			parser.onSLASH(); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestPatternParser, newTest);
		}
};