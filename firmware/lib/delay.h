#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>

#define SLEEP_TICKS 136U

#define INTERPRETER_CLOCK_TICK clock_tick++
#define SLEEP(x) clock_tick = 0; sleep_until = x

uint16_t sleep_until = 0;
volatile uint16_t clock_tick = 0;


#endif //__DELAY_H__
