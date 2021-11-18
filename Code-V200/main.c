/*------------------------------------------------------------------------------------
//- 20211111 ���� ���˻�LED�źŵƳ��򹤳�
//- 202111121828 �������decode���� ������
//- 202111131520 �������ͨ���޸Ľ������������� �仯��Χ 1000~2000�м��Ϊ1500���ر�ң��������Ȼ�����������
//- 							����ң������ʧ�ر���ʱ�������ʧң�����źţ����ջ��������趨�ı���ֵ���PWMֵ��ʧ�ر���������
//-								ÿ��ͨ�����ǿ�������ʧ�ر������ܡ�
//-								�������ӣ�ң������ͨ������ָʾ�Ƶ���˸���رգ�ǰ������˸��ȫ��
//- 202111132118	�����������ͨ�����ݵĲɼ��Ϳ��ƣ�CH8��������ݸߵ�ƽ��ͨ��9����������أ�ͨ��10�������ƹر�
//-								������˸��
-------------------------------------------------------------------------------------*/

#include "STC15Wxx.h"
#include "uart.h"
#include "main.h"
#include "time.h"
#include "gpio.h"
#include "eeprom.h"

uint8_t uart_cnt = 0;
uint16_t led_cnt = 0;
uint8_t led_flash_status = 0;
bit g_c1_flag = 0;
bit c1_out_flag = 0;

bit g_c2_flag = 0;
bit c2_out_flag = 0;

int main(void)
{
	uint8_t j;
	
	Run_Dimming = 100;

	Timer0Init();
	init_gpio();	
	UartInit();

	EA = 1;	
	
	Read_8byte_eeprom(eeprom_data);
	
	SendString(VERSION);
	SendString("\n");
//	
//	for(j=0;j<4;j++)
//	{
//		printf_d(eeprom_data[j]);
//		SendString("\r");
//	}
//	
//	SendString("\n");
	
	if(Run_Time == 0xffff)
	{
		Run_Time = 0;
		Run_Dimming = 100;
		Run_High_value = 100;
		Run_Low_value = 10;
		save_eeprom();
	}
	
	for(j=0;j<14;j++)
	{
		CH_v[j]=0;
	}
	for(j = 0;j<32;j++)
	{
		Rx_buff[j] = 0;
	}
	
	rx_flag_bit = 0;

	led_flash_status = 4;

	while(1)
	{	
		//-------------   0.01s  -----------------	
		if(flag_01bit)
		{	
			flag_01bit = 0;
			
			//--------- timer 
			led_cnt++;
			if(led_cnt >= 50)
			{
				led_cnt = 0;	
					
				switch(led_flash_status)
				{
					case 0:
						LED_R = 0;
						LED_G = 0;
						led_flash_status = 1;
					  
						break;
					
					case 1: //�Ѿ�ȫ���ر�LEDָʾ��
						break;
					
					case 2:
						LED_G = 0;
						LED_R = 1;
						led_flash_status = 3;
						break;
					
					case 3: //ǰ������˸
						LED_R =~LED_R ;
						LED_G =~LED_G ;
						break;
					
					case 4:
						LED_R = 1;
						LED_G = 1;
						led_flash_status = 5;
						break;
					
					case 5:
						LED_R =~ LED_R;
						LED_G =~ LED_G;
						break;
				}
				//---------------------------MC1 output
				if((g_c1_flag)&&(c1_out_flag==0))
				{
					MC_1 = 1;
					c1_out_flag = 1;
				}
				else MC_1 = 0;
				//---------------------------MC2 output
				if((g_c2_flag)&&(c2_out_flag==0))
				{
					MC_2 = 1;
					c2_out_flag = 1;
				}
				else if((c2_out_flag)&&(g_c2_flag==0))
				{
					MC_2 = 0;
					c2_out_flag = 0;
				}
			}
			//--------------------------- ���ڳ�ʱ�ж�
			if(rx_timer_bit) //���ճ�ʱ�ж�
			{
				uart_cnt ++;
				if(uart_cnt > 100)
				{
					Rx_cnt = 0;
					uart_cnt = 0;
				}
			}
			else
			{
				uart_cnt = 0;
			}
			//-----------------
		} //end of 0.1s timer 
		
		//-------------- ���ڽ������
		if(rx_flag_bit)
		{
			rx_decode(CH_v,Rx_buff);
			rx_flag_bit = 0;
			
			if((g_ch10_v < 1200)&&(led_flash_status != 1))
				led_flash_status = 0; 	//�ر�ָʾ��
			else if((1450 < g_ch10_v)&&(g_ch10_v < 1550)&&(led_flash_status != 3))
			{
				led_flash_status = 2;  //LED��˸
			}
			else if((g_ch10_v > 1850)&&(led_flash_status != 5))
			{
				led_flash_status = 4;  //ȫ��LED
			}
			//--------------------------
			if((g_c1_flag == 0)&&(g_ch8_v > 1850))
				g_c1_flag = 1;
			else if(g_ch8_v < 1100)
			{
				g_c1_flag = 0;
				c1_out_flag = 0;
			}
			//--------------------------
			if((g_c2_flag == 0)&&(g_ch9_v > 1850))
				g_c2_flag = 1;
			else if(g_ch9_v < 1100)
			{
				g_c2_flag = 0;
				//c2_out_flag = 0;
				//MC_2 = 0;
			}
		}
		//--------------------------
		
	}
}







//------------------------------- End -----------------------------



