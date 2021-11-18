
//#include "eeprom.h"
//#include "intrins.h"
//#include "uart.h"
#include "STC15Wxx.h"
#include "comparator.h"
#include "gpio.h"

//--------------------------------------
//-  比较器初始化

void init_cmpt(void)
{
//	CMPCR1 = 0x04;  //0B0000 0100
//	CMPCR2 = 0x3f;  //0B0011 1111  

//	CMPCR2 |= CMPEN;
//	//while(!(CMPCR1 & CMPIF));
//	CMPCR1 &=~CMPIF;
	
	  CMPCR2 = 0x00;
    CMPCR2 &= ~0x80;                            //???????
//  CMPCR2 |= 0x80;                             //???????
    CMPCR2 &= ~0x40;                            //??0.1us??
//  CMPCR2 |= 0x40;                             //??0.1us??
//  CMPCR2 &= ~0x3f;                            //?????????
    CMPCR2 |= 0x3f;                             //最长时间延时
    CMPCR1 = 0x00;
  //  CMPCR1 |= 0x30;                             //使能上升沿下降沿中断
//  CMPCR1 &= ~0x20;                            //??????????
//  CMPCR1 |= 0x20;                             //??????????
//  CMPCR1 &= ~0x10;                            //??????????
//  CMPCR1 |= 0x10;                             //??????????
    CMPCR1 &= ~0x08;                            //P5.5作为正比较输入
//  CMPCR1 |= 0x08;                             //ADC????CMP+???
//  CMPCR1 &= ~0x04;                            //???????CMP-???
    CMPCR1 |= 0x04;                             //P5.4作为负比较输入
    CMPCR1 &= ~0x02;                              //禁止输出到P12管脚
  //  CMPCR1 |= 0x02;                           //允许输出到P12管脚
    CMPCR1 |= 0x80;                             //???????
}

/*//--------------------------------------
//- 电池电量测量  A点电压 3.2V  B点电压3.0V
//- 返回电池电量 1 2 3
uint8_t battery_test(void)
{
	//uint8_t bat_sta = 0;
	uint16_t delay_a = 65535;
	
	CMP_A = 0;        //断开第二比较电压
	if(get_CMP()==0)  //空载电池电压高于A点电压
	{
		LED_PWM = 1; 
		CMP_A = 1;         //-判断空载电压是否高于B点电压
		while(delay_a --);  //带载测试    
		if(get_CMP()==0) 	//电池空载高于B点
		{
			LED_PWM = 0; 
		  CMP_A = 0;         //-判断空载电压是否高于B点电压
		  return 1; //新电池
		}		
	}
	else  //空载电池电压低于A点电压 
	{
		CMP_A = 1;        //-判断空载电压是否高于B点电压
		if(get_CMP()==0) 	//电池空载高于B点
		{
		  	LED_PWM = 1; 
				CMP_A = 1;         //-判断空载电压是否高于B点电压
				while(delay_a --);  //带载测试    
				if(get_CMP()==0) 	//电池空载高于B点
				{
					LED_PWM = 0; 
					CMP_A = 0;         //-判断空载电压是否高于B点电压
					return 1; //新电池
				}	
		}
		else  //电池空载低于B点
		{
			return 0; //不是新电池
		}
	}
}
*/



