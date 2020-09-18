#pragma once

#include <unity/unity_fixture.h>
#include <unity/unity_exception.h>

#define RUN_TEST_GROUP_CLASS(TestClass) TestClass::run()
#define RUN_TEST_CASE_CLASS(CLASS,TESTNAME) UnityTestClass<CLASS>::invokeTest<TESTNAME>(__FILE__,"TEST(" #CLASS "::" #TESTNAME ")",#CLASS,#TESTNAME)

template <typename CLASS>
class UnityTestClass {
private:
	static void _exceptHandlerForSetup() {
		_EXPECT_NO_EXCEPTION_MESSAGE(
			EXPECT_NO_ASSERT_MESSAGE(CLASS::setup(), "Unhandled assertion in test SETUP"), "Unhandled exception in test SETUP"
		);
	}

	static void _exceptHandlerForTearDown() {
		_EXPECT_NO_EXCEPTION_MESSAGE(
			EXPECT_NO_ASSERT_MESSAGE(CLASS::tearDown(), "Unhandled assertion in test TEAR DOWN")
			, "Unhandled exception in test TEAR DOWN");
	}

	template <unityfunction f>
	static void _exceptHandler() {
		_EXPECT_NO_EXCEPTION(
			EXPECT_NO_ASSERT(
				f()
			)
		);
	}
public:
	template <unityfunction f>
	static void invokeTest(const char* file, const char* msg,char const* group, char const *testname) {
		UnityTestRunner(_exceptHandlerForSetup,
			_exceptHandler<f>,
			_exceptHandlerForTearDown,
			msg,
			group, testname,
			file, __LINE__);
	}

	template <unityfunction f>
	static void invokeTest(const char* file,char const* testname) {
		invokeTest<f>(file, "TEST", "", testname);
	}
};


/*
class TestClass:UnityTestClass<TestClass> {
public:
	static void setup() {

	}
	static void tearDown() {
	}


	static void newTest() {
		TEST_ASSERT_EQUAL(1,1);
	}
	static void newTest2() {
		TEST_ASSERT_EQUAL(1, 0);
	}

	static void run() {
		RUN_TEST_CASE_CLASS(TestClass,newTest);
		RUN_TEST_CASE_CLASS(TestClass,newTest2);

	}
};

...
RUN_TEST_GROUP_CLASS(TestClass);
*/