#pragma once
//Table state machine


//generic event processor
class StateTable {
	public:
		typedef void (StateTable::* Action)();
		struct Transition {
			Action action;
			int nextState;
		};
	private:
		const Transition* _table;
		int _nSignals;
		int _nStates;
	protected:
		int _state;
		StateTable(const Transition* table, int nStates, int nSignals) : _table(table), _nSignals(nSignals),
			_nStates(nStates) {
		}
	public:
		virtual ~StateTable() {}; //virtual dtor
		void dispatch(unsigned event) {
			const Transition* t = _table + _state * _nSignals + event;
			(this->*(t->action))();
			_state = t->nextState;
		}
		void doNothing() {} //do nothing default action
};


//specific Comment parser state machine
class TableParser : public StateTable {
	public:
		enum Signal {
			SIG_CHAR, SIG_STAR, SIG_SLASH, SIG_MAX
		};

		enum State {
			STATE_CODE, STATE_SLASH, STATE_COMMENT, STATE_STAR, STATE_MAX
		};

	private:
		static const StateTable::Transition _table[STATE_MAX][SIG_MAX];
		int _commentCount;
		void a1() { _commentCount++; }
		void a2() { _commentCount += 2; }
	public:
		TableParser() : StateTable(&_table[0][0], STATE_MAX, SIG_MAX) { init(); }
		void init() { _commentCount = 0; _state = STATE_CODE; }
		int commentCount() const { return _commentCount; }
};


const StateTable::Transition TableParser::_table[STATE_MAX][SIG_MAX] = {
	{	{&StateTable::doNothing, STATE_CODE},
		{&StateTable::doNothing, STATE_CODE},
		{&StateTable::doNothing, STATE_SLASH}
	},
	{	{&StateTable::doNothing, STATE_CODE},
		{static_cast<StateTable::Action>(&TableParser::a2), STATE_COMMENT},
		{&StateTable::doNothing, STATE_CODE}
	},
	{	{static_cast<StateTable::Action>(&TableParser::a1), STATE_COMMENT},
		{&StateTable::doNothing, STATE_STAR},
		{static_cast<StateTable::Action>(&TableParser::a1), STATE_COMMENT},
	},
	{	{static_cast<StateTable::Action>(&TableParser::a2), STATE_COMMENT},
		{static_cast<StateTable::Action>(&TableParser::a1), STATE_STAR},
		{static_cast<StateTable::Action>(&TableParser::a2), STATE_CODE},
	}
};
