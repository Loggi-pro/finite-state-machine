#include <void/chrono.h>
#include <void/interrupt.h>
#include <periph/timer.h>
#include "hardware.h"
#include <iostream>
#include "examples/hsm_parser.h"
VOID_ISR(CPH_INT_TIMER0_OCRA) {
	cph::Private::timer0::THwTimer0::IntOCRA_Handler();
}
namespace std {
	ostream cout;
}

int main() {
	cph::TSystemCounter::initPrecision<TIMER_CONFIG>();
	vd::EnableInterrupts();
	std::cout << "start" << std::endl;
	static auto parser = HsmParser();
	parser.init();
	parser.dispatch({ parser.SIG_CHAR });

	while (true) {
	}

	return 0;
}