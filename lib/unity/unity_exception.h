#ifndef UNITY_EXCEPTION_H
#define UNITY_EXCEPTION_H
#include "unity_internals.h"
#ifdef UNITY_EXCEPTIONS

#define _EXPECT_EXCEPTION(x) {\
		try {\
		x;	\
		TEST_FAIL_MESSAGE("Expected exception, but it not happened");\
		}catch(...){\
		}\
		}
#define _EXPECT_EXCEPTION_MESSAGE(x,msg) {\
		try {\
		x;	\
		TEST_FAIL_MESSAGE(msg);\
		}catch(...){\
		}\
		}

#define _EXPECT_NO_EXCEPTION(x) {\
		try {\
		x;	\
		}catch(...){\
		TEST_FAIL_MESSAGE("Unhandled exception rised");\
		}\
		}

#define _EXPECT_NO_EXCEPTION_MESSAGE(x,msg) {\
		try {\
		x;	\
		}catch(...){\
		TEST_FAIL_MESSAGE(msg);\
		}\
		}

#define EXPECT_EXCEPTION(x) {_EXPECT_EXCEPTION(x)}
#define EXPECT_EXCEPTION_MESSAGE(x,msg) {_EXPECT_EXCEPTION_MESSAGE(x,msg)}
#define EXPECT_NO_EXCEPTION(x) {_EXPECT_NO_EXCEPTION(x)}
#define EXPECT_NO_EXCEPTION_MESSAGE(x,msg) {_EXPECT_NO_EXCEPTION_MESSAGE(x,msg)}

#else
#define _EXPECT_EXCEPTION(x) {x;}
#define _EXPECT_EXCEPTION_MESSAGE(x,msg) {x;}
#define _EXPECT_NO_EXCEPTION(x) {x;}
#define _EXPECT_NO_EXCEPTION_MESSAGE(x,msg) {x;}

#define EXPECT_EXCEPTION(x) {TEST_IGNORE_MESSAGE("Exception doesn't support, test ignored");}
#define EXPECT_EXCEPTION_MESSAGE(x,msg) {TEST_IGNORE_MESSAGE("Exception doesn't support, test ignored");}
#define EXPECT_NO_EXCEPTION(x) {TEST_IGNORE_MESSAGE("Exception doesn't support, test ignored");}
#define EXPECT_NO_EXCEPTION_MESSAGE(x,msg) {TEST_IGNORE_MESSAGE("Exception doesn't support, test ignored");}
#endif


#endif

