#pragma once
#include <unity/unity_fixture_classes.h>
#include "examples/deffered_event.h"

//Hsm server while busy (handle request) cannot handle other requests->defer it, and store in queue
//Request must be stored inside object, pointers used in global queue
/*
idle-ENTRY
No deferred requests
Processing request #1
receiving-ENTRY
authorizing-ENTRY
Request #2 deffered
Request #3 deffered
Request #4 deffered
idle-ENTRY
Request #2 recalled
Processing request #2
receiving-ENTRY
authorizing-ENTRY
Request #5 deffered
Request #6 deffered
Request #7 IGNORED
idle-ENTRY
Request #3 recalled
Processing request #3
receiving-ENTRY
authorizing-ENTRY
idle-ENTRY
Request #4 recalled
Processing request #4
receiving-ENTRY
authorizing-ENTRY
idle-ENTRY
Request #5 recalled
Processing request #5
receiving-ENTRY
authorizing-ENTRY
idle-ENTRY
Request #6 recalled
Processing request #6
receiving-ENTRY
authorizing-ENTRY
idle-ENTRY
No deferred requests
final-ENTRY

*/

class TestDefferedEventHsm : UnityTestClass<TestDefferedEventHsm> {
		static inline std::ostream nullstream{ 0 };
		static inline HsmServer server = HsmServer{ nullstream };
		static HsmServer::RequestEvent newRequest() {
			static int i = 0;
			HsmServer::RequestEvent ev;
			ev.sig = HsmServer::SIG_NEW_REQUEST;
			ev.ref_num = ++i;
			return ev;
		}
	public:
		static void setup() {
			server.init();
			eventQueue.clear();
		}
		static void tearDown() {
		}


		static void testServer() {
			//idle
			server.dispatch(newRequest()); //->new request #1
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::receiving));
			//busy
			server.dispatch({ HsmServer::SIG_RECEIVED });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::authorizing));
			//wait for authorized
			server.dispatch(newRequest());//->defer #2
			TEST_ASSERT_EQUAL(1, server.countDefferedNow());
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::busy));
			server.dispatch(newRequest()); //->defer #3
			TEST_ASSERT_EQUAL(2, server.countDefferedNow());
			server.dispatch(newRequest()); //->defer #4
			TEST_ASSERT_EQUAL(3, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			//idle
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			TEST_ASSERT_EQUAL(1, server.countHandled());
			eventQueue.checkQueue(); //->recalling request #2
			//busy
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::busy));
			TEST_ASSERT_EQUAL(2, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_RECEIVED });
			server.dispatch(newRequest());//->defer #5
			server.dispatch(newRequest());//->defer #6
			TEST_ASSERT_EQUAL(4, server.countDefferedNow());
			TEST_ASSERT_EQUAL(0, server.countIgnored());
			server.dispatch(newRequest()); //->ignore #7
			TEST_ASSERT_EQUAL(4, server.countDefferedNow());
			TEST_ASSERT_EQUAL(1, server.countIgnored());
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			//idle
			TEST_ASSERT_EQUAL(2, server.countHandled());
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			//new request
			eventQueue.checkQueue(); //->recalling request #3
			TEST_ASSERT_EQUAL(3, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_RECEIVED });
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			TEST_ASSERT_EQUAL(3, server.countHandled());
			//new request
			eventQueue.checkQueue(); //->recalling request #4
			TEST_ASSERT_EQUAL(2, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_RECEIVED });
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			TEST_ASSERT_EQUAL(4, server.countHandled());
			//new request
			eventQueue.checkQueue();//->recalling request #5
			TEST_ASSERT_EQUAL(1, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_RECEIVED });
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			TEST_ASSERT_EQUAL(5, server.countHandled());
			//new request
			eventQueue.checkQueue();//->recalling request #6
			TEST_ASSERT_EQUAL(0, server.countDefferedNow());
			server.dispatch({ HsmServer::SIG_RECEIVED });
			server.dispatch({ HsmServer::SIG_AUTHORIZED });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::idle));
			TEST_ASSERT_EQUAL(6, server.countHandled());
			server.dispatch({ HsmServer::SIG_TERMINATE });
			TEST_ASSERT_EQUAL(true, server.isInState(&HsmServer::final));
		}

		static void run() {
			RUN_TEST_CASE_CLASS(TestDefferedEventHsm, testServer);
		}
};

