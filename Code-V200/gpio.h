#ifndef __GPIO_H_
#define __GPIO_H_

#include "STC15Wxx.h"

#define LED_PWM P33
#define CMP_A   P32
#define LED_RUN P31
#define KEY     P30

#define LED_R P54
#define LED_G P55
#define MC_1  P32
#define MC_2  P33

void init_gpio();

#endif

