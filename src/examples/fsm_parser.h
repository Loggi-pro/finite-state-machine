#pragma once
#include "fsm/fsm.h"
class OptimalParserFsm : public fsm::Fsm {
	public:
		enum Signal {
			SIG_CHAR = fsm::ReservedSignals::SIG_USER,
			SIG_STAR, SIG_SLASH
		};
	private:
		int _commentCount;
		//all actions coreesponding to code state
		void code(const fsm::Event& ev) {
			switch (ev.sig) {
			case fsm::SIG_EMPTY: break;

			case fsm::SIG_ENTRY: break;

			case fsm::SIG_EXIT: break;

			case SIG_SLASH:
				transition(&OptimalParserFsm::slash);
				break;
			}
		}
		//all actions coreesponding to slash state
		void slash(const fsm::Event& ev) {
			switch (ev.sig) {
			case fsm::SIG_EMPTY: break;

			case fsm::SIG_ENTRY: break;

			case fsm::SIG_EXIT: break;

			case SIG_STAR:
				_commentCount += 2;
				transition(&OptimalParserFsm::comment);
				break;

			case SIG_CHAR:
				transition(&OptimalParserFsm::code);
				break;

			case SIG_SLASH:
				break;
			}
		}
		//all actions coreesponding to comment state
		void comment(const fsm::Event& ev) {
			switch (ev.sig) {
			case fsm::SIG_EMPTY: break;

			case fsm::SIG_ENTRY: break;

			case fsm::SIG_EXIT: break;

			case SIG_STAR:
				_commentCount++;
				transition(&OptimalParserFsm::star);
				break;

			case SIG_CHAR:
				_commentCount++;
				break;

			case SIG_SLASH:
				_commentCount++;
				break;
			}
		}
		//all actions coreesponding to star state
		void star(const fsm::Event& ev) {
			switch (ev.sig) {
			case fsm::SIG_ENTRY: break;

			case fsm::SIG_EXIT: break;

			case fsm::SIG_EMPTY: break;

			case SIG_SLASH:
				_commentCount ++;
				transition(&OptimalParserFsm::code);
				break;

			case SIG_CHAR:
				_commentCount ++;
				transition(&OptimalParserFsm::comment);
				break;

			case SIG_STAR:
				_commentCount ++;
				transition(&OptimalParserFsm::comment);
				break;
			}
		}
	public:
		OptimalParserFsm() : fsm::Fsm((fsm::Fsm::State) & OptimalParserFsm::code), _commentCount(0)  {}
		int commentCount()const { return _commentCount; }

};