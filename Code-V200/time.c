
#include "time.h"
//#include "uart.h"
#include "main.h"
#include "gpio.h"
#include "STC15Wxx.h"
#include "intrins.h"
#include "eeprom.h"
//#include "comparator.h"
//#include "uart.h"

//------------------------------------------------
//- 定时器0初始化
//------------------------------------------------

bit pwm_bit = 0;
bit flag_01bit = 0;
uint8_t PWM_value = 0;
/*
void Timer0Init(void)		//10us@12MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模块
	TL0 = 0x88;		  //设置定时器初值
	TH0 = 0xff;		  //设置定时器初值  
	
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始工作
	PT0 = 1;     
	ET0 = 1;    //开启定时器0中断使能
}
*/

void Timer0Init(void)		//50us@22.1184MHz
{
	AUXR |= 0x80;		//
	TMOD &= 0xF0;		//
	TL0 = 0xAE;		//5C@100US
	TH0 = 0xFB;		//F7@100US
	TF0 = 0;		//CLEAR TIMER FLAG
	TR0 = 1;		//START TIMER0 
	
	PT0 = 1;     
	ET0 = 1;    //开启定时器0中断使能
}

//---------------------------------------------------------
/* Timer0 interrupt routine */
//- 1/1000hz = 0.001  pwm 200阶梯 0.001/200 = 0.000005
//---------------------------------------------------------
void tm0_isr() interrupt 1 using 1
{
	static uint16_t cont_01s = 0;
//	static pwm_cont = 0;
  
	cont_01s ++;
	if(cont_01s > 100)
	{
		flag_01bit = 1;
		cont_01s = 0;
	}
//------------- PWM 5us----------------	
//	PWM_value = 50;
	//pwm_cont ++;
/*	if(PWM_value >= PWM_MAX)
		pwm_cont = 0;
	else pwm_cont ++;
	if(pwm_cont < PWM_value)
	{
		if(pwm_bit == 0)
		{
			pwm_bit = 1;
			LED_PWM = 1;
		}
	}
	else 
	{
		if(pwm_cont >= PWM_MAX)
			pwm_cont = 0;
		
		if(pwm_bit == 1)
		{
			pwm_bit = 0;
			LED_PWM = 0;
		}
  }*/
//------------------------------------
}


/*
//-------------- genrate SOS signal --------------
//- 输出PWM value
//- 0.01中断入口
//------------------------------------------------
bit sos_signal(void)
{
	static bit pwm_buff = 0; //最大200
	static uint16_t count = 0;
	count ++;
	switch(count)
	{
		case 33: pwm_buff = 1; break; 
		case 66: pwm_buff = 0; break; 
		case 99: pwm_buff = 1; break; 
		case 132: pwm_buff = 0; break; 
		case 165: pwm_buff = 1; break; 
		case 198: pwm_buff = 0; break;  //	间隔2s 
																		    
		case 431: pwm_buff = 1; break;  // 亮1s          198+33+200 
		case 531: pwm_buff = 0; break;  // 灭0.3s 41+10  431+100 
	  case 564: pwm_buff = 1; break;  // 亮1s   51+3   531+33
		case 664: pwm_buff = 0; break;  // 灭0.3s 54+10  564+100
		case 697: pwm_buff = 1; break;  // 亮1s   64+3   664+33  
		case 797: pwm_buff = 0; break;  // 'O'灭 0.3s  67+10 697+100   
		
	  case 1030: pwm_buff = 1; break;  // 亮0.3s 间隔2s  77+3+20  797+33+200
		case 1063: pwm_buff = 0; break;  // 灭0.3s 100+3   1030+33
	  case 1096: pwm_buff = 1; break;  // 亮0.3s 103+3  1063+33
		case 1129: pwm_buff = 0; break;  // 灭0.3s 106+3  1096+33 
		case 1162: pwm_buff = 1; break;  // 亮0.3s 109+3  1129+33 
		case 1195: pwm_buff = 0; break;  // 灭0.3s 112+3  1162+33
		case 1528: count = 0; break;    //复位一组SOS发送完成 和下一组间隔3S  115+3+30  1195+33+300
	}
	
	return pwm_buff ;
}
*/

//----------------------- end -----------------------------

