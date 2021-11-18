
#ifndef __UART_H_
#define __UART_H_

#include "STC15Wxx.h"

extern uint8_t  Rx_buff[];
extern uint8_t Rx_cnt;
extern uint16_t CH_v[]; //´æ´¢½âÂëÊý¾Ý


#define g_ch1_v CH_v[0]
#define g_ch2_v CH_v[1]
#define g_ch3_v CH_v[2]
#define g_ch4_v CH_v[3]
#define g_ch5_v CH_v[4]
#define g_ch6_v CH_v[5]
#define g_ch7_v CH_v[6]
#define g_ch8_v CH_v[7]
#define g_ch9_v CH_v[8]
#define g_ch10_v CH_v[9]
#define g_ch11_v CH_v[10]
#define g_ch12_v CH_v[11]
#define g_ch13_v CH_v[12]
#define g_ch14_v CH_v[13]



extern bit rx_flag_bit;
extern bit rx_timer_bit;


void UartInit(void);
void SendData(unsigned char dat);
void SendString(char *s);
void printf_d(uint16_t num);
void analayze_rx(uint8_t *p);
void rx_decode(uint16_t *ch,uint8_t *val);


#endif



