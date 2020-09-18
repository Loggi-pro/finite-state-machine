#pragma once
#include <periph/gpio.h>
#include <void/static_assert.h>
#include <periph/basic_usart.h>
#include <periph/iostream.h>
#include <periph/system_counter.h>
#include <periph/gpio.h>

typedef cph::basic_ostream<cph::AdapterUsartSync_Debug<cph::Usart0>> ostream;
typedef  cph::TTimerConfig<cph::TIMER_0, cph::TimerPrecision::Resolution_1ms> TIMER_CONFIG;
typedef cph::io::Pb5 pinLed;