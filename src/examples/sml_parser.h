#pragma once
#include <sml.hpp>

namespace sml = vd::sml;



namespace sml_parser {

	struct event_char {};
	struct event_star {};
	struct event_slash {};

	struct state_code {};
	struct state_slash {};
	struct state_comment {};
	class state_star {};

	struct shared_data {
		unsigned char commentCount = 0;

		void comment_increase(int v = 1) {
			commentCount += v;
		}
	};
	struct transitions {
		using Self = transitions;

		auto operator()() {
			using namespace vd::sml;
			/**
			 * Initial state: *initial_state
			 * Transition DSL: src_state + event [ guard ] / action = dst_state
			 */
			return make_transition_table(
			           *state<state_code> +event<event_slash> = state<state_slash>
			, state<state_slash>	+event<event_star>	/ [](shared_data & d) { d.comment_increase(2); } = state<state_comment>
			, state<state_slash>	+event<event_char>													= state<state_code>
			, state<state_comment>	+event<event_char>	/ [](shared_data & d) { d.comment_increase(); }	= state<state_comment>
			, state<state_comment>	+event<event_slash> / [](shared_data & d) { d.comment_increase(); } = state<state_comment>
			, state<state_comment>	+event<event_star>	/ [](shared_data & d) { d.comment_increase(); }	= state<state_star>
			, state<state_star>		+event<event_slash> / [](shared_data & d) { d.comment_increase(); } = state<state_code>
			, state<state_star>		+event<event_char>	/ [](shared_data & d) { d.comment_increase(); }	= state<state_comment>
			, state<state_star>		+event<event_star>	/ [](shared_data & d) { d.comment_increase(); }	= state<state_comment>
			       );
		}

	};

}


struct SmlParser {
	sml_parser::shared_data d{};
	sml::sm < sml_parser::transitions> _stateMachine{ d };
	SmlParser() {}

	void init() {
		d.commentCount = 0;
	}

	template <typename T>
	void dispatch(T&& ev) {
		_stateMachine.process_event(ev);
	}

	template <typename T>
	bool is(T&& state) {
		return _stateMachine.is(state);
	}

	int commentCount() const {
		return d.commentCount;
	}

};