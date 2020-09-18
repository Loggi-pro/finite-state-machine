#include "unity_assertion.h"
static uint32_t _UnityWasAssert = 0;
static UNITY_LINE_TYPE _UnityAssertOnLineAt = 0;
static const char* _UnityAssertInFile = 0;
static uint8_t _UnityIsExpectAssert;
void UnityResetAssertStatus() {
	_UnityWasAssert = 0;
	_UnityAssertInFile = 0;
	_UnityAssertOnLineAt = 0;
}
uint32_t UnityGetAssertStatus() {
	uint32_t tmp = _UnityWasAssert;

	if (_UnityWasAssert > 0) {
		_UnityWasAssert--;
	}

	return tmp;
}

void UnityGetAssertInfo(const char** inFile, UNITY_LINE_TYPE* atLine) {
	*inFile = _UnityAssertInFile;
	*atLine = _UnityAssertOnLineAt;
}



void UnitySetAssertStatus(char status, const char* file, UNITY_LINE_TYPE line) {
	_UnityWasAssert += status;

	if (_UnityWasAssert == 1) {
		_UnityAssertOnLineAt = line;
		_UnityAssertInFile = file;
	}
}

uint8_t UnityIsExpectAssert() {
	return _UnityIsExpectAssert;
}

void UnitySetExpectAssert(uint8_t val) {
	_UnityIsExpectAssert = val;
}
