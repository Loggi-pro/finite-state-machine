#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/hsm_parser.h"





class TestHsmParser : UnityTestClass<TestHsmParser> {
		static inline HsmParser parser{};
	public:
		static void setup() {
			parser.init();
		}
		static void tearDown() {
		}


		static void newTest() {
			parser.dispatch({ HsmParser::SIG_CHAR });
			parser.dispatch({ HsmParser::SIG_SLASH }); //open 1
			parser.dispatch({ HsmParser::SIG_STAR });//2
			parser.dispatch({ HsmParser::SIG_CHAR }); //3
			parser.dispatch({ HsmParser::SIG_CHAR }); //4
			parser.dispatch({ HsmParser::SIG_SLASH }); //5
			parser.dispatch({ HsmParser::SIG_STAR });//close 6
			parser.dispatch({ HsmParser::SIG_SLASH }); //7
			TEST_ASSERT_EQUAL(7, parser.commentCount());
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestHsmParser, newTest);
		}
};

