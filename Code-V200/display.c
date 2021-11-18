#include "STC15Wxx.h"
#include "tm1638.h"
#include "display.h"
#include "uart.h"
#include "time.h"

//unsigned int code dis_item[27];  //���ڴ洢��Ҫ��ȡ����ֵ

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
	static bit run_bit = 0;   //����ֹͣ��־
	static bit loc_control_bit = 0; // ����Զ�̿���
	static bit init_loc_bit = 0;

	key_cmd_value = g_key_cmd;
	g_key_cmd = 0xff;
	
	switch(work_mod)
	{
		case STANDBY_MODE : //����
		    
		READ_REG_ADDR	= dis_num;
		key_enable = 0;//�������ģʽ�ɹ� �����Զ���ѯ����
			
			switch(key_cmd_value)
			{
				case KEY_RUN: //���յ����а���ָ��
					modbus_cmd = 0x06;
					READ_REG_ADDR = 40062;
					modbus_value = 1;  //��������
					run_bit = 1; 
				Read_reg_ok = 1;
				  break;
				
				case KEY_STOP: //���յ����а���ָ��
					modbus_cmd = 0x06;
					READ_REG_ADDR = 40062;
					modbus_value = 0;  //��������
					run_bit = 0;
				Read_reg_ok = 1;
					break;
				
				case KEY_ENT: //������ʾ�л�ָ��
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
				case KEY_SET: //���յ���������ָ��
					work_mod = SET_MODE;
				  modbus_value = 0;
					break;
			}
			if((work_mod != SET_MODE)&&(rec_data[0] != FAULT_NUM_P)&&(Read_reg_ok == 0))
			{
				modbus_value = rec_data[1];  //��ȡ���صļĴ�������ֵ
				switch(rec_data[0])
				{
					case IN_VOT:       //��ȡ�����ѹ
						led_flag_bit = VOLTAGE_V;
						break;
					case IN_CURRENT:   //��ȡ�������
						led_flag_bit = CURRENT_A;
						break;
					case OUT_VOT:      //��ȡ�����ѹ
						led_flag_bit = VOLTAGE_KV;
						break;
					case OUT_CURRENT:  //��ȡ�������
						led_flag_bit = CURRENT_MA;
						break;
					case DC_BUS_VOT:   //��ȡֱ��ĸ�ߵ�ѹ
						led_flag_bit = VOLTAGE_V;
						break;
					case TRANS_TEMP_D ://��ȡ��ѹ���¶�
						led_flag_bit = TEMP_LED;
						break;
					case IGBT_TEMP_D:  //��ȡIGBT�¶�
						led_flag_bit = TEMP_LED;
						break;
					case FREQUENCY_D:  //��ȡƵ��
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
					//case FAULT_NUM_P:  //��ȡ������  ������������ʱ����ʾ
					//	modbus_value = FAULT_NUM;
					//	break;
					//default:
					}	
				}
			
			break;
							
		case RUN_MODE:      //������
			break;
		
		
		case SET_MODE:        //������ ����ͨ�����¼�������Ŀ ���԰���ENT�����������״̬
      	led_flag_bit = 0xc0;  //�ұ�������ͬʱ��
		    switch(key_cmd_value)
			  {
					case KEY_ENT :
						reg_addr_buff = modbus_value;  //�ݴ��趨�ĵ�ֵַ ���ڷ���ʱ
					  
					  Read_reg_ok = 1;
						work_mod = VALUE_ADJ_MODE;
					  READ_REG_ADDR = modbus_value + 40000 ;  //��ȡ��Ӧ��ַ�Ĵ���ֵ
					  modbus_readhold_reg(11,READ_REG_ADDR);  //��ȡһ��ֵ
						break;
					case KEY_SET :
						work_mod = STANDBY_MODE;
						break;
					case KEY_UP :  //��Ŀ������
						if(modbus_value < 100)
						  modbus_value ++;
						break;
					case KEY_DOWN :  //��Ŀ������
						if(modbus_value > 0)
							modbus_value --;
						break;
				}
			break;
		
		
		case VALUE_ADJ_MODE:  //��������   ͨ���������¼�������Ŀ����  ���԰���ENT��д����� �����ص�����״̬
			if(Read_reg_ok == 0)//&&(read_lock_flag ==0)) //���յ���������
			{
				//rec_data[0];
				modbus_value = rec_data[1];  //��ȡ���صļĴ�������ֵ
				Read_reg_ok = 1;
			}
			switch(key_cmd_value)
			  {
					case KEY_ENT :
					  READ_REG_ADDR = rec_data[0] ;  //��ȡ��Ӧ��ַ�Ĵ���ֵ
					  modbus_value = reg_addr_buff;  //���غ��ȡ�ݴ�ֵ
					  modbus_cmd = 0x06;  //д�Ĵ�������
					  work_mod = SET_MODE; // �������ӱ��غʹӻ�ͬ�����������ֵ
						
					  switch(READ_REG_ADDR)
						{
							case LOCAL_D :
								if(rec_data[1] == 0)
									loc_control_bit = 0;
								else loc_control_bit = 1;
							break;
							case SLAVE_ADDR_D : //�ӻ���ַ�޸�
								modbus_slave_add = rec_data[1];
								break;
						}
						
						break;
					case KEY_SET :
						work_mod = SET_MODE;
					  modbus_value = reg_addr_buff;  //���غ��ȡ�ݴ�ֵ
						break;
					case KEY_UP :  //��Ŀ������
						if(modbus_value < 5000)
						  modbus_value ++;
						rec_data[1] = modbus_value;
						break;
					case KEY_DOWN :  //��Ŀ������
						if(modbus_value > 0)
							modbus_value --;
						rec_data[1] = modbus_value;
						break;
				}
			
			break;
		
		
		case FAULT_MODE:       //������
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
				modbus_value = 0;  //�������
				work_mod = STANDBY_MODE;  //�ָ�������״̬
				key_enable =1;
			}
			break;
			
			//default ;
	}
			if((FAULT_NUM > 0)&&(work_mod != FAULT_MODE))//ͨ���жϹ�����������״̬
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
							READ_REG_ADDR = FAULT_NUM_P;//��2�ζ�������
						}
					}
					else
					{
						read_reg_bit = 1;
						if(work_mod != FAULT_MODE)//��һ�ζ���Ҫ��ֵ
							READ_REG_ADDR = dis_num;
						if(run_bit == 0)
							led_flag_bit &= 0x81;
					}
					modbus_readhold_reg(11,READ_REG_ADDR);
				}
					
				if(run_bit)   //��һ��LED ����ʾ������  
					led_flag_bit |= 0x01 ;
				else led_flag_bit &= 0xfe;
				
				if(loc_control_bit)  //����Զ�̿���
					led_flag_bit |= 0x80;
				else led_flag_bit &= 0x7f;
				
					refresh_display(modbus_value,led_flag_bit,work_mod);
			}

}


//------------------------------------------------
//- �ⲿ�˿ڳ�ʼ��
/*
sbit	STB=P3^6;  //�������  ���� OC
sbit	CLK=P3^3;  // �������  ���� OC
sbit	DIO=P5^5;  // Ĭ��
*/
//------------------------------------------------
void init_gpio()
{
	//p1.2 p1.3 p1.4 p1.5 ���迹����
	//p1.0 p1.1 �������
	
	P1M1 = 0xF8; // # 1111 1100; 
	P1M0 = 0xC7; // # 1100 0011 //p1.0 p1.1 �������  P1.2 P1.3 P1.4 P1.5 ���迹����
	
	// p3.2 sysled p3.3 shutter2  p3.6 shutter1  p3.7 led_bl  P3.0 RXD  P3.1 TXD
  P3M1 = 0x01; // # 0111 1101;    // P3.2 P3.3 P3.6 OPEN DRAIN
  P3M0 = 0xFF; // # 1111 1111;	  // p3.7 p3.1 ������� P3.0����������
	
	//P5M1 = 0x30;  //# 0011 0000     // p5.4 shutter3  p5.5 shutter4   OC��
	//P5M0 = 0x30;  //# 0011 0000
		
}

///---------------- End ---------------------