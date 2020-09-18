#define UNITY_EXCLUDE_STDINT_H
#define UNITY_EXCLUDE_STDLIB_MALLOC
#define UNITY_EXCLUDE_FLOAT
#include "stdcommon.h"
#include <cph/unity/unity_fixture.h>


//_GROUP_NAME

TEST_GROUP(_GROUP_NAME);

TEST_SETUP(_GROUP_NAME) {
}


TEST_TEAR_DOWN(_GROUP_NAME) {
}

TEST(_GROUP_NAME, TestName) {
	//TEST_ASSERT_EQUAL(true, FRingBuffer.is_empty());
}

TEST_GROUP_RUNNER(_GROUP_NAME) {
	RUN_TEST_CASE(_GROUP_NAME, TestName);
	/*
	RUN_TEST_CASE(GROUP_NAME, CheckBuffer_ShouldEmpty					);
	RUN_TEST_CASE(GROUP_NAME, Check_Push_NotEmpty						);
	RUN_TEST_CASE(GROUP_NAME, Check_Push_Count							);
	RUN_TEST_CASE(GROUP_NAME, Push_Success								);
	RUN_TEST_CASE(GROUP_NAME, Pop_Success								);
	RUN_TEST_CASE(GROUP_NAME, InlinePop_Success							);
	RUN_TEST_CASE(GROUP_NAME, Check_InlinePop_NotExist					);
	RUN_TEST_CASE(GROUP_NAME, Look_from_first_Success					);
	RUN_TEST_CASE(GROUP_NAME, Check_Look_from_first_NotExist			);
	RUN_TEST_CASE(GROUP_NAME, Look_from_last_Success					);
	RUN_TEST_CASE(GROUP_NAME, Check_Look_from_last_NotExist				);
	RUN_TEST_CASE(GROUP_NAME, InlineLook_from_first_Success				);
	RUN_TEST_CASE(GROUP_NAME, Check_InlineLook_from_first_NotExist		);
	RUN_TEST_CASE(GROUP_NAME, InlineLook_from_last_Success				);
	RUN_TEST_CASE(GROUP_NAME, Check_InlineLook_from_last_NotExist		);
	RUN_TEST_CASE(GROUP_NAME, Check_Override_Success					);
	RUN_TEST_CASE(GROUP_NAME, Check_OverrideOfCounters_LookSuccess		);
	RUN_TEST_CASE(GROUP_NAME, Check_OverrideOfCounters_PopSuccess		);
	*/
}