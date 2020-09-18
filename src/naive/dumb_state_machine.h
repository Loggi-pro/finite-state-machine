#pragma once
//Using direct dispatch method

//Example for comment char counter

class DumbParser {
	public:
		enum Signal {
			SIG_CHAR, SIG_STAR, SIG_SLASH
		};

		enum State {
			STATE_CODE, STATE_SLASH, STATE_COMMENT, STATE_STAR
		};
	private:
		State _state; //current state
		int _commentCount; //character counter
	public:
		void init() { _commentCount = 0; transition(STATE_CODE); }
		void dispatch(Signal event) {
			switch (_state) {
			case STATE_CODE:
				switch (event) {
				case SIG_SLASH:
					transition(STATE_SLASH);
					break;
				}

				break;

			case STATE_SLASH:
				switch (event) {
				case SIG_STAR:
					_commentCount += 2;
					transition(STATE_COMMENT);
					break;

				case SIG_CHAR:
				case SIG_SLASH:
					transition(STATE_CODE);
					break;
				}

				break;

			case STATE_COMMENT:
				switch (event) {
				case SIG_STAR:
					transition(STATE_STAR);
					break;

				case SIG_CHAR:
				case SIG_SLASH:
					++_commentCount;
					break;
				}

				break;

			case STATE_STAR:
				switch (event) {
				case SIG_STAR:
					++_commentCount;
					break;

				case SIG_SLASH:
					_commentCount += 2;
					transition(STATE_CODE);
					break;

				case SIG_CHAR:
					_commentCount += 2;
					transition(STATE_COMMENT);
					break;
				}

				break;

			default:
				break;
			}
		}
		void transition(State target) { _state = target; }

		int commentCount() {
			return _commentCount;
		}
};

