#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/sml_parser.h"

class TestSmlParser : UnityTestClass<TestSmlParser> {
		//static inline sml::sm<SmlParser> parser = sml::sm<SmlParser> {};
		//static inline sml::sm<states> sm{};
	public:
		static void setup() {
		}
		static void tearDown() {
		}


		static void newTest() {
			using namespace sml;
			SmlParser parser{}; // pass dependencies via ctor
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_code>));
			parser.dispatch(sml_parser::event_char{});
			TEST_ASSERT_EQUAL(0, parser.commentCount());
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_code>));
			parser.dispatch(sml_parser::event_slash{}); //open 1
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_slash>));
			parser.dispatch(sml_parser::event_star{});//2
			TEST_ASSERT_EQUAL(2, parser.commentCount());
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_comment>));
			parser.dispatch(sml_parser::event_char{}); //3
			TEST_ASSERT_EQUAL(3, parser.commentCount());
			parser.dispatch(sml_parser::event_char{}); //4
			TEST_ASSERT_EQUAL(4, parser.commentCount());
			parser.dispatch(sml_parser::event_slash{}); //5
			TEST_ASSERT_EQUAL(5, parser.commentCount());
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_comment>));
			parser.dispatch(sml_parser::event_star{});//close 6
			TEST_ASSERT_EQUAL(6, parser.commentCount());
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_star>));
			parser.dispatch(sml_parser::event_slash{}); //
			TEST_ASSERT_EQUAL(7, parser.commentCount());
			TEST_ASSERT_TRUE(parser.is(state<sml_parser::state_code>));
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestSmlParser, newTest);
		}
};