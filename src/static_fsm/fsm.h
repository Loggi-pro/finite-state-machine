#pragma once
#include "void/variant.h"
#include "void/optional.h"

template <typename Derived> //CRTP
struct static_transitions {
private:
	void* parent;
protected:
	Derived* self() {
		return static_cast<Derived*>(parent);
	}
public:
	static_transitions(void* p) : parent(p) {}
};


template <typename StateVariant, typename Transitions>
class static_fsm {
protected:
	StateVariant _state;
public:
	static_fsm() {}
	template <typename EventVariant>
	void dispatch(const EventVariant& event) {
		vd::optional<StateVariant> newState = vd::visit(Transitions{ this }, _state, event);

		if (newState) {
			_state = vd::move(newState.value);
		}
	}

};


