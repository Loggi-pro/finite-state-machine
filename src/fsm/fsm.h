#pragma once
//Combine pattern state machine
//generic event processor
#include "fsm_types.h"

namespace fsm {
	class Fsm: FsmBase {
		public:
			typedef void (Fsm::* State)(const Event&);
		private:
			State _currentState;
		protected:
			template <typename DerivedState>
			void transition(DerivedState target) {
				(this->*_currentState)(FsmBase::_ReservedEvents[SIG_EXIT]); //exit action
				_currentState = static_cast<State>(target);
				(this->*_currentState)(FsmBase::_ReservedEvents[SIG_ENTRY]); //entry action
			}
		public:
			Fsm(State initial) : _currentState(initial) {}
			void init() {}
			void dispatch(const Event& ev) { (this->*_currentState)(ev); }
	};


}