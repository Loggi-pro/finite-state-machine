#pragma once
#include "fsm/hsm.h"
class HsmParser : public hsm::Hsm {
		uint32_t _comment_count;
	public:
		enum HsmParserSignals {
			SIG_SLASH = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
			SIG_STAR, SIG_CHAR
		};
		HsmParser() : Hsm(hsm::as_state(&HsmParser::initial)), _comment_count(0) {}
		uint32_t commentCount()const {
			return _comment_count;
		}

		constexpr static hsm::Event const testEvent[] = {
			{HsmParser::SIG_SLASH}, {HsmParser::SIG_STAR}, {HsmParser::SIG_CHAR}
		};
		template <HsmParserSignals sig>
		constexpr static hsm::Event const& getEvent() {
			return testEvent[sig - hsm::ReservedSignals::SIG_USER];
		}

	private:
		constexpr hsm::EventResult initial(const hsm::Event& e) {
			return eventTransition(hsm::as_state(&HsmParser::code));
		}
		hsm::EventResult code(const hsm::Event& e) { //state handler
			switch (e.sig) {
			case SIG_SLASH: return eventTransition(&HsmParser::code_slash);

			case SIG_STAR: return eventHandled();

			case SIG_CHAR: return eventHandled();
			}

			return eventSuper(&Hsm::top);
		}
		/**/hsm::EventResult code_slash(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case SIG_SLASH: return eventTransition(&HsmParser::code);

			case SIG_STAR: return eventTransition(&HsmParser::comment);

			case SIG_CHAR: return eventTransition(&HsmParser::code);

			default:
				break;
			}

			return eventSuper(&HsmParser::code);
		}
		hsm::EventResult comment(const hsm::Event& e) { //state handler
			switch (e.sig) {
			case hsm::ReservedSignals::SIG_ENTRY:
				_comment_count += 2;
				return eventHandled();

			case SIG_SLASH:
				_comment_count += 1;
				return eventHandled();

			case SIG_STAR:
				_comment_count += 1;
				return eventTransition(&HsmParser::comment_star);

			case SIG_CHAR:
				_comment_count += 1;
				return eventHandled();

			default:
				break;
			}

			return eventSuper(&Hsm::top);
		}
		/**/hsm::EventResult comment_star(const hsm::Event& e) {  //state handler
			switch (e.sig) {
			case SIG_SLASH:
				_comment_count += 1;
				return eventTransition(&HsmParser::code);

			case SIG_STAR:
				_comment_count += 1;
				return eventHandled();

			case SIG_CHAR:
				_comment_count += 1;
				return eventTransition(&HsmParser::comment);

			default:
				break;
			}

			return eventSuper(&HsmParser::comment);
		}

};


