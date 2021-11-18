
#ifndef __MAIN_H_
#define __MAIN_H_

#include "STC15Wxx.h"

extern uint8_t PWM_k ;
extern uint16_t timer_cnt;

#define FIRMWARE_TYPE  1

//��ʱ���ж�ISR ��λus
#define TIM_ISR_VALUE  5

#define PWM_MAX 100

#define K_MAX  100
#define LIGHT_HIGH  K_MAX
#define LIGHT_LOW   K_MAX/10

// 30Sһ����λ  7H = 7*60*2
#define TIM_7HOUR  840

#define VERSION "Ver203"

#if FIRMWARE_TYPE == 1
bit battery_test(bit cmp_bit);
#endif

#endif


