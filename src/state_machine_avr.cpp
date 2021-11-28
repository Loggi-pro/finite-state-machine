#include "hardware.h"
#include <void/chrono.h>
#include <void/interrupt.h>
#include <cph/timer.h>
#include <iostream>
#include "examples/fsm_parser.h"
#include "examples/fsm_static_parser.h"
#include "examples/sml_parser.h"
#include <void/variant.h>
#include <void/tuple.h>
VOID_ISR(CPH_INT_TIMER0_OCRA) {
	cph::Private::timer0::THwTimer0::IntOCRA_Handler();
}
namespace std {
	ostream cout;
}


//#define OPTIMAL_FSM	// FLASH = 2070, Ram = 43
//#define STATIC_FSM	// FLASH = 1550, Ram = 49
#define SML_FSM			// FLASH = 1504, Ram = 62

#ifdef OPTIMAL_FSM

auto testOptimalParserFsm() {
	static auto parser = OptimalParserFsm();
	parser.init();
	parser.dispatch({ OptimalParserFsm::SIG_CHAR });
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //open 1
	parser.dispatch({ OptimalParserFsm::SIG_STAR });//2
	parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //3
	parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //4
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //5
	parser.dispatch({ OptimalParserFsm::SIG_STAR });//close 6
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //7
	return parser;
}
#define CALL_PARSER() testOptimalParserFsm()
#elif defined(STATIC_FSM)

auto testStaticParserFsm() {
	static auto parser = fsm_static_parser::StaticParserFsm{};
	parser.init();
	parser.dispatch(fsm_static_parser::event_char{});
	parser.dispatch(fsm_static_parser::event_slash{}); //open 1
	parser.dispatch(fsm_static_parser::event_star{});//2
	parser.dispatch(fsm_static_parser::event_char{}); //3
	parser.dispatch(fsm_static_parser::event_char{}); //4
	parser.dispatch(fsm_static_parser::event_slash{}); //5
	parser.dispatch(fsm_static_parser::event_star{});//close 6
	parser.dispatch(fsm_static_parser::event_slash{}); //7
}
#define  CALL_PARSER() testStaticParserFsm()
#elif defined(SML_FSM)

auto testSmlParser() {
	static auto parser = SmlParser{};
	parser.init();
	parser.dispatch(sml_parser::event_char{});
	parser.dispatch(sml_parser::event_slash{}); //open 1
	parser.dispatch(sml_parser::event_star{});//2
	parser.dispatch(sml_parser::event_char{}); //3
	parser.dispatch(sml_parser::event_char{}); //4
	parser.dispatch(sml_parser::event_slash{}); //5
	parser.dispatch(sml_parser::event_star{});//close 6
	parser.dispatch(sml_parser::event_slash{}); //7
	return parser;
}
#define CALL_PARSER() testSmlParser()
#endif

int main() {
	cph::SystemCounter::initPrecision<TIMER_CONFIG>();
	vd::EnableInterrupts();
	std::cout << "start" << std::endl;
	CALL_PARSER();

	while (true) {
	}

	return 0;
}