#include "hardware.h"
#include <void/chrono.h>
#include <void/interrupt.h>
#include <cph/timer.h>
#include <iostream>
#include "examples/fsm_parser.h"
#include "examples/fsm_static_parser.h"
#include <void/variant.h>
VOID_ISR(CPH_INT_TIMER0_OCRA) {
	cph::Private::timer0::THwTimer0::IntOCRA_Handler();
}
namespace std {
	ostream cout;
}

struct Obj {
	int a;
	int b;
};


vd::variant<int, Obj> v{1};

int main() {
	cph::SystemCounter::initPrecision<TIMER_CONFIG>();
	vd::EnableInterrupts();
	std::cout << "start" << std::endl;
	static auto parser = StaticParserFsm{};
	//OptimalParserFsm();
	/*parser.init();
	parser.dispatch({ OptimalParserFsm::SIG_CHAR });
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //open 1
	parser.dispatch({ OptimalParserFsm::SIG_STAR });//2
	parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //3
	parser.dispatch({ OptimalParserFsm::SIG_CHAR }); //4
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //5
	parser.dispatch({ OptimalParserFsm::SIG_STAR });//close 6
	parser.dispatch({ OptimalParserFsm::SIG_SLASH }); //7*/
	parser.dispatch(event_char{});
	parser.dispatch(event_slash{}); //open 1
	parser.dispatch(event_star{});//2
	parser.dispatch(event_char{}); //3
	parser.dispatch(event_char{}); //4
	parser.dispatch(event_slash{}); //5
	parser.dispatch(event_star{});//close 6
	parser.dispatch(event_slash{}); //7
	vd::visit(vd::overloaded{
		[&](int&) {},
		[&](auto&) {}
	}, v);

	while (true) {
	}

	return 0;
}