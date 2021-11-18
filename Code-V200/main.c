/*------------------------------------------------------------------------------------
//- 20211111 建立 无人机LED信号灯程序工程
//- 202111121828 程序加入decode功能 待测试
//- 202111131520 程序测试通过修改解码程序测试正常 变化范围 1000~2000中间点为1500，关闭遥控器后依然有数据输出。
//- 							开启遥控器的失控保护时，如果丢失遥控器信号，接收机将按照设定的比例值输出PWM值，失控保护现在有
//-								每个通道都是可以设置失控保护功能。
//-								功能增加：遥控器的通道控制指示灯的闪烁：关闭，前后交替闪烁，全亮
//- 202111132118	程序完成三个通道数据的采集和控制，CH8控制输出暂高电平，通道9控制输出开关，通道10控制主灯关闭
//-								或者闪烁。
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
					
					case 1: //已经全部关闭LED指示灯
						break;
					
					case 2:
						LED_G = 0;
						LED_R = 1;
						led_flash_status = 3;
						break;
					
					case 3: //前后交替闪烁
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
			//--------------------------- 串口超时判断
			if(rx_timer_bit) //接收超时判断
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
		
		//-------------- 串口接收完成
		if(rx_flag_bit)
		{
			rx_decode(CH_v,Rx_buff);
			rx_flag_bit = 0;
			
			if((g_ch10_v < 1200)&&(led_flash_status != 1))
				led_flash_status = 0; 	//关闭指示灯
			else if((1450 < g_ch10_v)&&(g_ch10_v < 1550)&&(led_flash_status != 3))
			{
				led_flash_status = 2;  //LED闪烁
			}
			else if((g_ch10_v > 1850)&&(led_flash_status != 5))
			{
				led_flash_status = 4;  //全亮LED
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



