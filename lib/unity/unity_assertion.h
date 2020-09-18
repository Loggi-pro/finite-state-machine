#ifndef UNITY_ASSERTION_H
#define UNITY_ASSERTION_H
#include "unity_internals.h"



//ASSERT Expectation
#define EXPECT_ASSERT_MESSAGE(x,msg) {\
		uint8_t before = UnityIsExpectAssert();\
		UnitySetExpectAssert(1);\
		x;	\
		if (UnityGetAssertStatus()==0){TEST_FAIL_MESSAGE(msg);} else {\
		}\
		UnitySetExpectAssert(before); \
	}

#define EXPECT_ASSERT(x) {\
		uint8_t before = UnityIsExpectAssert();\
		UnitySetExpectAssert(1);\
		x;	\
		if (UnityGetAssertStatus()==0){TEST_FAIL();} else {\
			UnityResetAssertStatus(); \
		}\
		UnitySetExpectAssert(before);\
	}

#define EXPECT_NO_ASSERT(x) {\
		uint8_t before = UnityIsExpectAssert(); \
		UnitySetExpectAssert(0); \
		(void)x;\
		if (UnityGetAssertStatus() != 0) {\
			const char* file; UNITY_LINE_TYPE line; UnityGetAssertInfo(&file, &line); \
			const char* msg = "Unhandled assertion in file"; \
			UnityFailInFile(msg, __LINE__, file, line); \
			UnityResetAssertStatus(); \
		} else {}\
		UnitySetExpectAssert(before);\
	}
#define EXPECT_NO_ASSERT_MESSAGE(x,msg) {\
		uint8_t before = UnityIsExpectAssert(); \
		UnitySetExpectAssert(0); \
		x;	\
		if (UnityGetAssertStatus()!=0){\
			const char* file; UNITY_LINE_TYPE line; UnityGetAssertInfo(&file, &line);\
			UnityFailInFile(msg,__LINE__,file,line);\
		} else {}\
		UnitySetExpectAssert(before);\
	}

void UnityResetAssertStatus();
uint32_t UnityGetAssertStatus();
void UnityGetAssertInfo(const char** inFile, UNITY_LINE_TYPE* atLine);

void UnitySetAssertStatus(char status, const char* inFile, UNITY_LINE_TYPE atLine);

uint8_t UnityIsExpectAssert();
void  UnitySetExpectAssert(uint8_t val);

#endif