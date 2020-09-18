#pragma once
namespace fsm {
	enum ReservedSignals {
		SIG_EMPTY = 0,
		SIG_ENTRY = 1, /* signal for coding entry actions */
		SIG_EXIT, /* signal for coding exit actions */
		SIG_INIT, /* signal for coding initial transitions */
		SIG_USER /* first signal that can be used in user code */
	};
	typedef uint8_t Signal;
	struct Event {
		Signal sig;
		//can be extended for other arguments
	};

	class FsmBase {
		protected:
			constexpr static Event const _ReservedEvents[] = {
				{ (fsm::Signal)fsm::SIG_EMPTY },
				{ (fsm::Signal)fsm::SIG_ENTRY },
				{ (fsm::Signal)fsm::SIG_EXIT },
				{ (fsm::Signal)fsm::SIG_INIT}
			};
	};
}


//path to platform dependend event queue implementation
#ifdef WIN32
	//using std list
	#define EVENT_QUEUE_HEADER "fsm/win32/event_queue.h"
#else
	#warning EVENT_QUEUE_HEADER is not defined in 'fsm_types.h' file. Must contain path to event_queue implementation. Ignore this if you did not use event_queue in your project.
#endif