#pragma once

#define CPH_ENABLE_UART0
#define CPH_ENABLE_TIMER0
#include <cph/gpio.h>
#include <void/static_assert.h>
#include <cph/basic_usart.h>
#include <cph/iostream.h>
#include <cph/system_counter.h>
#include <cph/gpio.h>

typedef cph::basic_ostream<cph::AdapterUsartSync_Debug<cph::Usart0>> ostream;
typedef  cph::TTimerConfig<cph::TTimerNumber::TIMER_0, cph::TimerPrecision::Resolution_1ms> TIMER_CONFIG;
typedef cph::io::Pb5 pinLed;