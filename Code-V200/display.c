#include "STC15Wxx.h"
#include "tm1638.h"
#include "display.h"
#include "uart.h"
#include "time.h"

//unsigned int code dis_item[27];  //用于存储需要读取的数值

bit read_reg_bit = 0;

//uint16_t SEC_POWER_P ;
///uint16_t FRI_POWER_D ;
//uint16_t SEC_POWER_D ;
//uint16_t SEC_VOLT_P	;
//uint16_t SEC_CURRENT_P ;
//uint16_t FRI_VOLT    ;
//uint16_t FRI_CURRENT ;
//uint16_t SEC_VOLT    ;
//uint16_t SEC_CURRENT ;
//uint16_t DC_BUS      ;
////uint16_t RESERVE1    ;
//uint16_t TRAN_TEMP   ;
//uint16_t IGBT_TEMP   ;
////uint16_t RESERVE2    ;
////uint16_t CONTROL_STATE ;
////uint16_t RUN_STATE   ;
////uint16_t FLASH_TIME  ;
////uint16_t BEFOR_FLASH  ;
uint16_t FAULT_NUM   ; 
////uint16_t STOP_FAULT  ;
////uint16_t WARNING_FAULT ;
//uint16_t FREQUENCY    ;
////uint16_t ELE_VALUE   ; 
////uint16_t LOW_PROTECT ;
////uint16_t OUTPUT_STATE ; 
////uint16_t SHORT_TIME ;   
uint16_t READ_REG_ADDR ;


void display_process(void)
{
  uint8_t modbus_cmd;
	static uint8_t led_flag_bit = 0;
	uint8_t key_cmd_value = 0xff;
	
	static uint16_t modbus_value;
	static uint16_t dis_num = 40074;
	static uint8_t work_mod = STANDBY_MODE;
	static uint8_t reg_addr_buff = 0;
	//static uint8_t time_cunt = 0;
	 bit key_enable = 0;
	static bit run_bit = 0;   //运行停止标志
	static bit loc_control_bit = 0; // 本地远程控制
	static bit init_loc_bit = 0;

	key_cmd_value = g_key_cmd;
	g_key_cmd = 0xff;
	
	switch(work_mod)
	{
		case STANDBY_MODE : //待机
		    
		READ_REG_ADDR	= dis_num;
		key_enable = 0;//进入待机模式成功 允许自动查询数据
			
			switch(key_cmd_value)
			{
				case KEY_RUN: //接收到运行按键指令
					modbus_cmd = 0x06;
					READ_REG_ADDR = 40062;
					modbus_value = 1;  //启动运行
					run_bit = 1; 
				Read_reg_ok = 1;
				  break;
				
				case KEY_STOP: //接收到运行按键指令
					modbus_cmd = 0x06;
					READ_REG_ADDR = 40062;
					modbus_value = 0;  //启动运行
					run_bit = 0;
				Read_reg_ok = 1;
					break;
				
				case KEY_ENT: //接收显示切换指令
					//modebus_salve = 11;//test
					modbus_cmd = 0x03;
					
					if(dis_num < 40090)
					{
						dis_num ++;
						if(dis_num > 40081)
							dis_num = 40090;
					}
					else dis_num = 40074;
						
					READ_REG_ADDR = dis_num;
										
					break;
				case KEY_SET: //接收到参数设置指令
					work_mod = SET_MODE;
				  modbus_value = 0;
					break;
			}
			if((work_mod != SET_MODE)&&(rec_data[0] != FAULT_NUM_P)&&(Read_reg_ok == 0))
			{
				modbus_value = rec_data[1];  //读取返回的寄存器参数值
				switch(rec_data[0])
				{
					case IN_VOT:       //读取输入电压
						led_flag_bit = VOLTAGE_V;
						break;
					case IN_CURRENT:   //读取输入电流
						led_flag_bit = CURRENT_A;
						break;
					case OUT_VOT:      //读取输出电压
						led_flag_bit = VOLTAGE_KV;
						break;
					case OUT_CURRENT:  //读取输出电流
						led_flag_bit = CURRENT_MA;
						break;
					case DC_BUS_VOT:   //读取直流母线电压
						led_flag_bit = VOLTAGE_V;
						break;
					case TRANS_TEMP_D ://读取变压器温度
						led_flag_bit = TEMP_LED;
						break;
					case IGBT_TEMP_D:  //读取IGBT温度
						led_flag_bit = TEMP_LED;
						break;
					case FREQUENCY_D:  //读取频率
						led_flag_bit = FREQUENCY_HZ;
						break;
					case RESERVE1:  //
						led_flag_bit = 0;
						break;
					case LOCAL_D :
						if(modbus_value == 0)
							loc_control_bit = 0;
						else loc_control_bit = 1;
						break;
					//case FAULT_NUM_P:  //读取故障码  故障码在正常时不显示
					//	modbus_value = FAULT_NUM;
					//	break;
					//default:
					}	
				}
			
			break;
							
		case RUN_MODE:      //运行中
			break;
		
		
		case SET_MODE:        //设置中 可以通过上下键调整项目 可以按动ENT进入参数设置状态
      	led_flag_bit = 0xc0;  //右边两个灯同时亮
		    switch(key_cmd_value)
			  {
					case KEY_ENT :
						reg_addr_buff = modbus_value;  //暂存设定的地址值 用于返回时
					  
					  Read_reg_ok = 1;
						work_mod = VALUE_ADJ_MODE;
					  READ_REG_ADDR = modbus_value + 40000 ;  //读取对应地址寄存器值
					  modbus_readhold_reg(11,READ_REG_ADDR);  //读取一次值
						break;
					case KEY_SET :
						work_mod = STANDBY_MODE;
						break;
					case KEY_UP :  //项目调整上
						if(modbus_value < 100)
						  modbus_value ++;
						break;
					case KEY_DOWN :  //项目调整下
						if(modbus_value > 0)
							modbus_value --;
						break;
				}
			break;
		
		
		case VALUE_ADJ_MODE:  //参数调整   通过按动上下键调整项目参数  可以按动ENT键写入参数 并返回到设置状态
			if(Read_reg_ok == 0)//&&(read_lock_flag ==0)) //接收到完整数据
			{
				//rec_data[0];
				modbus_value = rec_data[1];  //读取返回的寄存器参数值
				Read_reg_ok = 1;
			}
			switch(key_cmd_value)
			  {
					case KEY_ENT :
					  READ_REG_ADDR = rec_data[0] ;  //读取对应地址寄存器值
					  modbus_value = reg_addr_buff;  //返回后读取暂存值
					  modbus_cmd = 0x06;  //写寄存器操作
					  work_mod = SET_MODE; // 这里增加本地和从机同步保存的数据值
						
					  switch(READ_REG_ADDR)
						{
							case LOCAL_D :
								if(rec_data[1] == 0)
									loc_control_bit = 0;
								else loc_control_bit = 1;
							break;
							case SLAVE_ADDR_D : //从机地址修改
								modbus_slave_add = rec_data[1];
								break;
						}
						
						break;
					case KEY_SET :
						work_mod = SET_MODE;
					  modbus_value = reg_addr_buff;  //返回后读取暂存值
						break;
					case KEY_UP :  //项目调整上
						if(modbus_value < 5000)
						  modbus_value ++;
						rec_data[1] = modbus_value;
						break;
					case KEY_DOWN :  //项目调整下
						if(modbus_value > 0)
							modbus_value --;
						rec_data[1] = modbus_value;
						break;
				}
			
			break;
		
		
		case FAULT_MODE:       //故障中
			if(read_reg_bit)
			{ 
				led_flag_bit = 0xff;
			}
			else
			{
				led_flag_bit = 0;
			}
      modbus_value = FAULT_NUM;
		
			if(key_cmd_value == KEY_STOP)
			{
				modbus_cmd = 0x06;
				READ_REG_ADDR = FAULT_NUM_P;
				FAULT_NUM = 0;
				modbus_value = 0;  //清除故障
				work_mod = STANDBY_MODE;  //恢复到待机状态
				key_enable =1;
			}
			break;
			
			//default ;
	}
			if((FAULT_NUM > 0)&&(work_mod != FAULT_MODE))//通过判断故障码进入故障状态
			{
				work_mod = FAULT_MODE;
				modbus_value = FAULT_NUM;
			}
			
			if(modbus_cmd == 0x06)
			{
				if(work_mod == SET_MODE)
					modbus_writehold_reg(11,READ_REG_ADDR,rec_data[1]);
				else modbus_writehold_reg(11,READ_REG_ADDR,modbus_value);
				modbus_cmd = 0;
			}
			else
			if(key_enable == 0)
			{
				if((work_mod != SET_MODE)&&(work_mod != VALUE_ADJ_MODE))
				{
					if(read_reg_bit)
					{
						read_reg_bit = 0;   
						if(init_loc_bit == 0)
						{
							READ_REG_ADDR = LOCAL_D;
							init_loc_bit = 1;
						}
						else
						{
							READ_REG_ADDR = FAULT_NUM_P;//第2次读故障码
						}
					}
					else
					{
						read_reg_bit = 1;
						if(work_mod != FAULT_MODE)//第一次读需要的值
							READ_REG_ADDR = dis_num;
						if(run_bit == 0)
							led_flag_bit &= 0x81;
					}
					modbus_readhold_reg(11,READ_REG_ADDR);
				}
					
				if(run_bit)   //第一个LED 亮表示运行中  
					led_flag_bit |= 0x01 ;
				else led_flag_bit &= 0xfe;
				
				if(loc_control_bit)  //本地远程控制
					led_flag_bit |= 0x80;
				else led_flag_bit &= 0x7f;
				
					refresh_display(modbus_value,led_flag_bit,work_mod);
			}

}


//------------------------------------------------
//- 外部端口初始化
/*
sbit	STB=P3^6;  //推挽输出  或者 OC
sbit	CLK=P3^3;  // 推挽输出  或者 OC
sbit	DIO=P5^5;  // 默认
*/
//------------------------------------------------
void init_gpio()
{
	//p1.2 p1.3 p1.4 p1.5 高阻抗输入
	//p1.0 p1.1 推挽输出
	
	P1M1 = 0xF8; // # 1111 1100; 
	P1M0 = 0xC7; // # 1100 0011 //p1.0 p1.1 推挽输出  P1.2 P1.3 P1.4 P1.5 高阻抗输入
	
	// p3.2 sysled p3.3 shutter2  p3.6 shutter1  p3.7 led_bl  P3.0 RXD  P3.1 TXD
  P3M1 = 0x01; // # 0111 1101;    // P3.2 P3.3 P3.6 OPEN DRAIN
  P3M0 = 0xFF; // # 1111 1111;	  // p3.7 p3.1 推挽输出 P3.0高主抗输入
	
	//P5M1 = 0x30;  //# 0011 0000     // p5.4 shutter3  p5.5 shutter4   OC门
	//P5M0 = 0x30;  //# 0011 0000
		
}

///---------------- End ---------------------