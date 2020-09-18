#pragma once
//Virtual strategy pattern state machine

//generic event processor
class Parser;

//abstract state
class BaseState {
	public:
		virtual void onCHAR(Parser* context, char ch) {};
		virtual void onSTAR(Parser* context) {};
		virtual void onSLASH(Parser* context) {};
};


class StateCode : public BaseState {
	public:
		virtual void onSLASH(Parser* context);
};

class StateSlash : public BaseState {
	public:
		virtual void onCHAR(Parser* context, char ch);
		virtual void onSTAR(Parser* ctx);
		virtual void onSLASH(Parser* ctx);
};

class StateComment : public BaseState {
	public:
		virtual void onCHAR(Parser* context, char ch);
		virtual void onSTAR(Parser* context);
		virtual void onSLASH(Parser* context);
};

class StateStar : public BaseState {
	public:
		virtual void onCHAR(Parser* context, char ch);
		virtual void onSTAR(Parser* context);
		virtual void onSLASH(Parser* context);
};


class Parser {
		friend class StateCode;
		friend class StateSlash;
		friend class StateComment;
		friend class StateStar;
		BaseState* _currentState;
		int _commentCount;
		static inline StateCode stateCode = StateCode();
		static inline StateSlash stateSlash = StateSlash();
		static inline StateComment stateComment = StateComment();
		static inline StateStar stateStar = StateStar();
	protected:
		void transition(BaseState& target) { _currentState = &target; }
	public:
		Parser() : _currentState(&stateCode) {}
		void init() { _commentCount = 0; transition(stateCode); }
		void onCHAR(char ch) { _currentState->onCHAR(this, ch); }
		void onSTAR() { _currentState->onSTAR(this); }
		void onSLASH() { _currentState->onSLASH(this); };
		int commentCount()const { return _commentCount; }
};


void StateCode::onSLASH(Parser* context) {
	context->transition(Parser::stateSlash);
}



void StateSlash::onCHAR(Parser* context, char ch) {
	context->transition(context->stateCode);
}
void StateSlash::onSTAR(Parser* ctx) {
	ctx->_commentCount += 2;
	ctx->transition(ctx->stateComment);
}
void StateSlash::onSLASH(Parser* ctx) {
	ctx->transition(ctx->stateCode);
};



void StateComment::onCHAR(Parser* context, char ch) {
	context->_commentCount++;
}
void StateComment::onSTAR(Parser* context) {
	context->transition(context->stateStar);
}
void StateComment::onSLASH(Parser* context) {
	context->_commentCount++;
};



void StateStar::onCHAR(Parser* context, char ch) {
	context->_commentCount += 2;
	context->transition(context->stateComment);
}
void StateStar::onSTAR(Parser* context) {
	context->_commentCount += 2;
	context->transition(context->stateComment);
}
void StateStar::onSLASH(Parser* context) {
	context->_commentCount += 2;
	context->transition(context->stateCode);
};
