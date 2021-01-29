
#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>
#include "pfs173.h"

void delay_ms(uint32_t msec)
{
  msec<<=10; //*1024, only works correct @8MHz SYSCLOCK
  for( ; msec>0; msec-- );
}

void delay_us(uint32_t usec)
{
  usec<<=7; //*128, only works correct @8MHz SYSCLOCK
  for( ; usec>0; usec-- );
}

#endif //__DELAY_H__
