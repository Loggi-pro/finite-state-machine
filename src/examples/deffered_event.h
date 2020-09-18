#pragma once
#include "fsm/hsm.h"
#include EVENT_QUEUE_HEADER
#include <iostream>
#include <list>

//Hsm server while busy (handle request) cannot handle other requests->defer it, and store in queue
//Request must be stored inside object, pointers used in global queue

class HsmServer : public hsm::Hsm {
	public:
		enum TServerSignals {
			SIG_NEW_REQUEST = hsm::ReservedSignals::SIG_USER, /* the new request signal */
			SIG_RECEIVED, /* the request has been received */
			SIG_AUTHORIZED, /* the request has been authorized */
			SIG_TERMINATE /* terminate the application */
		};
		struct RequestEvent : public hsm::Event {
			uint32_t ref_num; /* reference number of the request */
		};
		constexpr static int MAX_DEFFERED_EVENTS = 4;
		std::list<RequestEvent> _requestQueue; //event buffer here
		RequestEvent _lastRecalledEvent;
		std::ostream& _stream;
		HsmServer(std::ostream& stream): Hsm(hsm::as_state(&HsmServer::initial)),
			_stream(stream) {} // the default ctor


		int countHandled() const {
			return _handledCount;
		}
		int countIgnored()const {
			return _ignoredCount;
		}
		int countDefferedNow()const {
			return _requestQueue.size();
		}
	private:
		uint8_t _ignoredCount = 0;	//count of ignored messages
		uint8_t _handledCount = 0;	//count of handled messages
		const RequestEvent* recallEvent() {
			if (_requestQueue.empty()) { return nullptr; }

			_lastRecalledEvent = _requestQueue.front();
			eventQueue.putEvent(_lastRecalledEvent, *this);
			_requestQueue.pop_front();
			return &_lastRecalledEvent;
		}
		bool deferEvent(const RequestEvent& e) {
			if (_requestQueue.size() >= MAX_DEFFERED_EVENTS) { return false; }

			_requestQueue.push_back(e);
			return true;
		}
	public:
		void init() {
			Hsm::init();
			_ignoredCount = 0;
			_handledCount = 0;
			_requestQueue.clear();
		}

		hsm::EventResult initial(const  hsm::Event& e) {
			return eventTransition(&HsmServer::idle);
		}

		hsm::EventResult idle(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				/* recall the request from the private requestQueue */
				const RequestEvent* rq = recallEvent();
				_stream << "idle-ENTRY\n";

				if (rq != nullptr) { /* recall posted an event? */
					_stream << "Request #" << rq->ref_num << " recalled\n";
					//must to call new event
				} else {
					_stream << "No deferred requests\n";
				}

				return eventHandled();
			}

			case SIG_NEW_REQUEST: {
				const RequestEvent& reqestEvent = (const RequestEvent&)e;
				_stream << "Processing request #" << reqestEvent.ref_num << "\n";
				return eventTransition(&HsmServer::receiving);
			}

			case SIG_TERMINATE: {
				return eventTransition(&HsmServer::final);
			}
			}

			return eventSuper(&HsmServer::top);
		}
		hsm::EventResult busy(const  hsm::Event& e) {
			switch (e.sig) {
			case SIG_NEW_REQUEST: {
				if (deferEvent((const RequestEvent&)e)) {
					//defer the request
					_stream << "Request #" << ((const RequestEvent&)e).ref_num << " deffered\n";
				} else {
					// notify the request sender that the request was ignored..
					_stream << "Request #" << ((const RequestEvent&)e).ref_num << " IGNORED\n";
					_ignoredCount++;
				}

				return eventHandled();
			}

			case SIG_TERMINATE: {
				return eventTransition(&HsmServer::final);
			}
			}

			return eventSuper(&HsmServer::top);
		}
		hsm::EventResult receiving(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "receiving-ENTRY\n";
				//WAIT EVENT SIG_RECEIVED
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				return eventHandled();
			}

			case SIG_RECEIVED: {
				return eventTransition(&HsmServer::authorizing);
			}
			}

			return eventSuper(&HsmServer::busy);
		}
		hsm::EventResult authorizing(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "authorizing-ENTRY\n";
				//WAIT EVENT SIG_AUTHORIZED
				return eventHandled();
			}

			case hsm::ReservedSignals::SIG_EXIT: {
				return eventHandled();
			}

			case SIG_AUTHORIZED: {
				_handledCount++;
				return eventTransition(&HsmServer::idle);
			}
			}

			return eventSuper(&HsmServer::busy);
		}
		hsm::EventResult final(const  hsm::Event& e) {
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY: {
				_stream << "final-ENTRY\n";
				//TODO terminate application here
				return eventHandled();
			}
			}

			return eventSuper(&HsmServer::top);
		}

};
