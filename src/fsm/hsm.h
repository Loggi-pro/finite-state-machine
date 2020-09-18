#pragma once
#include <void/assert.h>
#include "fsm_types.h"
namespace hsm {

	namespace details {
		enum EventResultDetails { EventHandled = 0, EventIgnored, EventTransition, EventReturnParent }; //return by handler
	}
	using EventResult = details::EventResultDetails;
	using fsm::Event;
	using fsm::ReservedSignals;
	class Hsm;
	// name of the pointer-to-function type
	typedef  EventResult(Hsm::* StateHandler) (const Event& e);  // argument list



	template<typename T>
	static constexpr StateHandler as_state(EventResult(T::* f)(const Event& e)) {
		return static_cast<StateHandler>(f);
	}


	class Hsm : fsm::FsmBase {
		private:
			constexpr static uint8_t MAX_NEST_DEPTH = 7;

			template <class T>
			using StateHandlerDerived = EventResult(T::*)(const Event& e);

			bool _isInState(StateHandler state) {
				StateHandler temp = this->m_state;
				bool inState = false;  // assume that this HSM is not in 'state'
				EventResult r;

				if (this->m_state == state) { return true; }

				// scan the state hierarchy bottom-up

				do {
					r = invoke<ReservedSignals::SIG_EMPTY>(this->m_state); //get super state

					// do the states match?
					if (this->m_state == state) {
						inState = true;    // 'true' means that match found
						break;
					}
				} while (r != details::EventIgnored); // top() state not reached

				this->m_state = temp; // restore the stable state configuration
				return inState; // return the status
			}


			//invoke state function
			template < fsm::Signal signal>
			constexpr inline EventResult invoke(const StateHandler& state) { //invoke strategy
				//return (this->*state)({ signal });// invoke state handler s
				return (this->*state)(FsmBase::_ReservedEvents[signal]);// invoke state handler s
			}

			constexpr inline EventResult invoke(const StateHandler& state, Event const& e) {
				return (this->*state)(e);// invoke state handler s
			}

			constexpr inline void invoke_enter(const StateHandler& state) {
				if (invoke<ReservedSignals::SIG_ENTRY>(state) == details::EventHandled) {
					// software tracing instrumentation for state enter
				}
			}

			constexpr inline bool invoke_exit(const StateHandler& state) {
				if (invoke<ReservedSignals::SIG_EXIT>(state) == details::EventHandled) {
					// software tracing instrumentation for state enter
					return true;
				}

				return false;
			}


			//helper function to execute transition sequence in a hierarchical state
			/// machine (HSM).

			int8_t hsm_tran(StateHandler(&path)[MAX_NEST_DEPTH]) {
				int8_t ip = -1; // transition entry path index
				int8_t iq; // helper transition entry path index
				StateHandler t = path[0];
				StateHandler const s = path[2];
				EventResult r;

				if (s == t) { // (a) check source==target (transition to self)
					invoke_exit(s); // exit the source
					return 0; // enter the target
				}

				invoke<ReservedSignals::SIG_EMPTY>(t); // superstate of target
				t = this->m_state;

				// (b) check source==target->super
				if (s == t) {
					return 0; // enter the target
				}

				invoke<ReservedSignals::SIG_EMPTY>(s); // superstate of src

				// (c) check source->super==target->super
				if (this->m_state == t) {
					invoke_exit(s); // exit the source
					return 0; // enter the target
				}

				// (d) check source->super==target
				if (this->m_state == path[0]) {
					invoke_exit(s); // exit the source
					return -1; //maybe wrong line
				}

				// (e) check rest of source==target->super->super..
				// and store the entry path along the way
				iq = 0; // indicate that LCA not found
				ip = 1; // enter target and its superstate
				path[1] = t; // save the superstate of target
				t = this->m_state; // save source->super
				// find target->super->super
				r = invoke<ReservedSignals::SIG_EMPTY>(path[1]);

				while (r == details::EventReturnParent) { //5==============================
					path[++ip] = this->m_state; // store the entry path

					if (this->m_state == s) { // is it the source?
						iq = 1; // indicate that LCA found
						// entry path must not overfl
						assert(ip < MAX_NEST_DEPTH);
						--ip; // do not enter the source
						r = details::EventHandled; // terminate the loop
					} else { //it is not the source, keep going up
						r = invoke<ReservedSignals::SIG_EMPTY>(this->m_state);
					}
				} //-5==============================

				if (iq != 0) {//LCA found
					return ip;
				}

				//the LCA not found yet?
				//5==============================
				//entry path must not overflow
				assert(ip < MAX_NEST_DEPTH);
				invoke_exit(s); //exit the source
				// (f) check the rest of source->super == target->super->super...
				iq = ip;
				r = details::EventIgnored; // indicate LCA NOT found

				do {
					if (t == path[iq]) { // is this the LCA?
						r = details::EventHandled; // indicate LCA found
						ip = iq - 1; //do not enter LCA
						iq = -1; // terminate the loop
					} else {
						--iq; // try lower superstate of target
					}
				} while (iq >= 0);

				if (r == details::EventHandled) {
					//LCA found
					return ip;
				}

				// LCA not found yet?
				// (g) check each source->super->...for each target->super...
				r = details::EventIgnored; // keep looping

				do {
					// exit t unhandled?
					if (invoke<ReservedSignals::SIG_EXIT>(t) == details::EventHandled) {
						invoke<ReservedSignals::SIG_EMPTY>(t);
					}

					t = this->m_state; // set to super of t
					iq = ip;

					do { //8==============================
						if (t == path[iq]) { // is this LCA?
							// do not enter LCA
							ip = iq - 1;
							iq = -1; //break inner
							r = details::EventHandled; //break outer
						} else {
							--iq;
						}
					} while (iq >= 0); //-8
				} while (r != details::EventHandled);

				return ip;
			}


		protected:
			StateHandler m_state; // current active state (state-variable)


			//return event
			constexpr inline EventResult eventHandled() {
				return details::EventHandled;
			}
			constexpr inline EventResult eventIgnored() {
				return  details::EventIgnored;
			}

			template <typename T>
			constexpr inline EventResult eventSuper(StateHandlerDerived<T> super) {
				this->m_state = as_state<T>(super);
				return  details::EventReturnParent;
			}
			template <typename T>
			constexpr inline EventResult eventTransition(StateHandlerDerived<T> target) {
				this->m_state = as_state<T>(target);
				return  details::EventTransition;
			}

			static constexpr const Event& nullEvent() {
				return FsmBase::_ReservedEvents[ReservedSignals::SIG_EMPTY];
			}


		public:


			void init(const Event& e = nullEvent()) {
				StateHandler t = &Hsm::top; // HSM starts in the top state
				// the top-most initial transition must be taken
				assert(invoke(m_state, e) == details::EventTransition);

				do { // drill into the target. . .
					StateHandler path[MAX_NEST_DEPTH];
					int8_t ip = 0; // transition entry path index
					path[0] = this->m_state; // save the target of the initial transition
					invoke<ReservedSignals::SIG_EMPTY>(m_state);

					while (this->m_state != t) {
						path[++ip] = this->m_state;
						invoke<ReservedSignals::SIG_EMPTY>(this->m_state);
					}

					this->m_state = path[0]; // restore the target of the initial tran.
					// entry path must not overflow
					assert(ip < MAX_NEST_DEPTH);

					do { // retrace the entry path in reverse (desired) order. . .
						invoke_enter(path[ip]); // enter path [ip]
					} while ((--ip) >= 0);

					t = path[0]; // current state becothiss the new source
				} while (invoke<ReservedSignals::SIG_INIT>(t) == details::EventTransition);

				this->m_state = t;
			}


			void dispatch(const Event& e) {
				StateHandler t;
				StateHandler s;
				EventResult r;
				t = this->m_state; // save the current state

				// process the Event hierarchically. . .
				do {
					s = this->m_state;
					r = invoke(s, e);// invoke state handler s
				} while (r == details::EventReturnParent);

				// transition taken?
				if (r == details::EventTransition) {
					StateHandler path[MAX_NEST_DEPTH];
					path[0] = this->m_state; // save the target of the transition
					path[1] = t;
					path[2] = s; //path variable to function

					// exit current state to transition source s. . .
					while (t != s) {
						if (invoke_exit(t)) { //exit handled?
							invoke<ReservedSignals::SIG_EMPTY>(t); // find superstate of t
						}

						t = this->m_state; // this->m_state holds the superstate
					}

					//. . .
					t = path[0]; // target of the transition
					int8_t ip = hsm_tran(path); // transition entry path index

					// retrace the entry path in reverse (desired) order...
					for (; ip >= 0; --ip) {
						invoke_enter(path[ip]); // enter path [ip]
					}

					t = path[0]; // stick the target into register
					this->m_state = t; // update the current state

					// drill into the target hierarchy...
					while (invoke<ReservedSignals::SIG_INIT>(t) == details::EventTransition) {
						ip = 0;
						path[0] = this->m_state;
						invoke<ReservedSignals::SIG_EMPTY>(this->m_state); // find superstate

						while (this->m_state != t) {
							path[++ip] = this->m_state;
							invoke<ReservedSignals::SIG_EMPTY>(this->m_state); // find superstate
						}

						this->m_state = path[0];
						// entry path must not overflow
						assert(ip < MAX_NEST_DEPTH);

						do { // retrace the entry path in reverse (correct) order...
							invoke_enter(path[ip]); // enter path [ip]
						} while ((--ip) >= 0);

						t = path[0];
					}//while
				}

				//...
				this->m_state = t; //set new state or restore
			}
			constexpr EventResult top(const Event&) {
				return eventIgnored();
			}
			template <typename T>
			bool isInState(StateHandlerDerived<T> state) {
				return _isInState(as_state<T>(state));
			}
		protected:
			Hsm(StateHandler initial) : m_state(initial) {} // protected ctor
	};

}


