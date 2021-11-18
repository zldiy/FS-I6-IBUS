
//#include "eeprom.h"
//#include "intrins.h"
//#include "uart.h"
#include "STC15Wxx.h"
#include "comparator.h"
#include "gpio.h"

//--------------------------------------
//-  �Ƚ�����ʼ��

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
    CMPCR2 |= 0x3f;                             //�ʱ����ʱ
    CMPCR1 = 0x00;
  //  CMPCR1 |= 0x30;                             //ʹ���������½����ж�
//  CMPCR1 &= ~0x20;                            //??????????
//  CMPCR1 |= 0x20;                             //??????????
//  CMPCR1 &= ~0x10;                            //??????????
//  CMPCR1 |= 0x10;                             //??????????
    CMPCR1 &= ~0x08;                            //P5.5��Ϊ���Ƚ�����
//  CMPCR1 |= 0x08;                             //ADC????CMP+???
//  CMPCR1 &= ~0x04;                            //???????CMP-???
    CMPCR1 |= 0x04;                             //P5.4��Ϊ���Ƚ�����
    CMPCR1 &= ~0x02;                              //��ֹ�����P12�ܽ�
  //  CMPCR1 |= 0x02;                           //���������P12�ܽ�
    CMPCR1 |= 0x80;                             //???????
}

/*//--------------------------------------
//- ��ص�������  A���ѹ 3.2V  B���ѹ3.0V
//- ���ص�ص��� 1 2 3
uint8_t battery_test(void)
{
	//uint8_t bat_sta = 0;
	uint16_t delay_a = 65535;
	
	CMP_A = 0;        //�Ͽ��ڶ��Ƚϵ�ѹ
	if(get_CMP()==0)  //���ص�ص�ѹ����A���ѹ
	{
		LED_PWM = 1; 
		CMP_A = 1;         //-�жϿ��ص�ѹ�Ƿ����B���ѹ
		while(delay_a --);  //���ز���    
		if(get_CMP()==0) 	//��ؿ��ظ���B��
		{
			LED_PWM = 0; 
		  CMP_A = 0;         //-�жϿ��ص�ѹ�Ƿ����B���ѹ
		  return 1; //�µ��
		}		
	}
	else  //���ص�ص�ѹ����A���ѹ 
	{
		CMP_A = 1;        //-�жϿ��ص�ѹ�Ƿ����B���ѹ
		if(get_CMP()==0) 	//��ؿ��ظ���B��
		{
		  	LED_PWM = 1; 
				CMP_A = 1;         //-�жϿ��ص�ѹ�Ƿ����B���ѹ
				while(delay_a --);  //���ز���    
				if(get_CMP()==0) 	//��ؿ��ظ���B��
				{
					LED_PWM = 0; 
					CMP_A = 0;         //-�жϿ��ص�ѹ�Ƿ����B���ѹ
					return 1; //�µ��
				}	
		}
		else  //��ؿ��ص���B��
		{
			return 0; //�����µ��
		}
	}
}
*/



