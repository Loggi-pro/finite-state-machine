#pragma once
#include "void/variant.h"
#include "void/optional.h"
#include <iostream>

struct event_char {};
struct event_star {};
struct event_slash {};


using events = vd::variant<event_char, event_star, event_slash>;

class StaticParserFsm;


struct state_code {};
struct state_slash {};
struct state_comment {};
struct state_star {};

using state = vd::variant<state_code, state_slash, state_comment, state_star>;

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
			vd::optional<StateVariant> newState = vd::visit(Transitions{this}, _state, event);

			if (newState) {
				_state = vd::move(newState.value);
			}
		}

};


struct transitions;

class StaticParserFsm : public static_fsm<state, transitions> {
		friend struct transitions;
		int _commentCount;
	public:
		StaticParserFsm() {}
		int commentCount()const {
			return _commentCount;
		}
		void init() {
			_state = vd::move(state_code{});
			_commentCount = 0;
		}
};



struct transitions : public static_transitions<StaticParserFsm> {
	public:
		using static_transitions<StaticParserFsm>::static_transitions;

		template <typename T>
		vd::optional<state> operator()(state_code& s, const T& e) {
			return vd::visit(vd::overloaded{
				[](const event_slash&) {
					return vd::optional<state> { state_slash{} };
				},
				[](const auto&) {
					return vd::optional<state> {};
				} }
			, e);
		}
		//
		vd::optional<state> operator()(state_slash& s, const event_star& e) {
			self()->_commentCount += 2;
			return { state_comment{} };
		}

		vd::optional<state> operator()(state_slash& s, const event_slash& e) {
			return { state_code{} };
		}

		template <typename T>
		vd::optional<state> operator()(state_slash& s, const T& e) {
			return {};
		}
		//
		vd::optional<state> operator()(state_comment& s, const event_star& e) {
			self()->_commentCount++;
			return { state_star{} };
		}

		vd::optional<state> operator()(state_comment& s, const event_char& e) {
			self()->_commentCount++;
			return {};
		}
		vd::optional<state> operator()(state_comment& s, const event_slash& e) {
			self()->_commentCount++;
			return {};
		}


		template <typename T>
		vd::optional<state> operator()(state_comment& s, const T& e) {
			return {};
		}
		//

		vd::optional<state> operator()(state_star& s, const event_slash& e) {
			self()->_commentCount++;
			return { state_code{} };
		}

		vd::optional<state> operator()(state_star& s, const event_char& e) {
			self()->_commentCount++;
			return { state_comment{} };
		}

		vd::optional<state> operator()(state_star& s, const event_star& e) {
			self()->_commentCount++;
			return { state_comment{} };
		}


		template <typename T>
		vd::optional<state> operator()(state_star& s, const T& e) {
			return {};
		}

};
