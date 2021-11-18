

#include "STC15Wxx.h"
#include "tm1638.h"
#include "uart.h"
#include "display.h"

#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0


unsigned char code seg[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0,0xf3,0xdc};

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}												 

void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		Delay5us();
		if(DATA&0X01)
			DIO=1;
		else
			DIO=0;
		DATA>>=1;
		
		Delay5us();
		CLK=1;
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DIO=1;	//设置为输入
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK=0;
		Delay5us();
		if(DIO)
			temp|=0x80;
		
		Delay5us();
		CLK=1;
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
	STB=0;
	TM1638_Write(cmd);
	STB=1;
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB=0;
	TM1638_Write(0x42);		 //读键扫数据 命令
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	STB=1;					       //4个字节数据合成一个字节
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
	STB=0;
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	STB=1;
}
/*
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}  	*/
void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				//Write_DATA(2*i+1,3);
				Write_DATA(2*i+1,1);
			else
				Write_DATA(2*i+1,0);
		}
}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);       //亮度 (0x88-0x8f)8级亮度可调
	Write_COM(0x40);       //采用地址自动加1
	STB=0;		           //
	TM1638_Write(0xc0);    //设置起始地址

	for(i=0;i<16;i++)	   //传送16个字节的数据
		TM1638_Write(0x00);
	STB=1;
	
	for(i=0;i<8;i++)
	   Write_DATA(i<<1,seg[0]);	               //初始化寄存器
	
}

//---------------------------------------------------
void value_to_seg(unsigned int value,uint8_t *p)
{
	uint8_t temp = 0;
	
	if(value < 9999)  //接收4位数值转换
	{
		p[0] = value / 1000;
		if(p[0] == 0)
	    p[0] = 16;
		
		p[1] = value % 1000 / 100;
		if((p[1] == 0)&&(p[0]==16))
			p[1] = 16;
		
		p[2] = value % 100 / 10;
		if((p[0]==16)&&(p[1]==16)&&(p[2] ==0))
			p[2] = 16;
		p[3] = value % 10;
	}
	else 	
	{
		p[0] = 10;
		p[1] = 10;
		p[2] = 10;
		p[3] = 10;  //显示a
	}
}
//----------------- 刷新显示 ------------------
void refresh_display(unsigned int value,uint8_t led_bit,uint8_t dis_mode)
{
	uint8_t tape[8];
	uint8_t j;
	
	value_to_seg(value,tape);
	
	//Write_allLED(1<<led_bit);            // LED 指示灯的位置	
	
	switch(dis_mode)
	{
		case STANDBY_MODE : 
			break;
		case RUN_MODE:
			break;
		case SET_MODE:
			tape[0] = 17;
			break;
		case FAULT_MODE:
			tape[0] = 14;
			tape[1]|= 18;
			break;
//		case :
//			break;
//		default ;
		
	}
	
	for(j = 0;j < 4;j++)
	{
		Write_DATA(j*2,seg[tape[j]]);   // 制定地址写入制定数值
	}
	for(j = 4;j<8;j++)
	{
		Write_DATA(j*2,LED_OFF);   // 关闭显示
	}
	Write_allLED(led_bit);
}
//-----------------------------------------------

//--------------------- key scan 1ms ---------------------
//- 该函数可以完成按键的时长验证，单次有效和多次有效的区分；
//- 但是存在一个问题，被isrelease 屏蔽的按键无法完成按键释放时的指令查询
unsigned char key_scan(void)
{
	static unsigned char key_back_id = 0;
	static unsigned char t1ms_cnt = 0;
	static uint8_t key_isrelease = 1;
	static unsigned char key_long_action = 0;
	
	uint8_t key_buff = 0;
	uint8_t key_cmd = 255;
	
	key_buff = Read_key();                          //读按键值
	
	if(key_buff < 8)
	{
			//SendString("key  c \n");
			
		 if(key_isrelease == 0)//长按键键值筛选
			{
				if((key_buff == s1_id_run)||(key_buff == s2_id_stop)||(key_buff == s3_id_set)||(key_buff == s6_id_ent))
					return 0xff;  // 以上按键只允许单次按键
			}
		 
		 key_back_id = key_buff;
		 
		 t1ms_cnt ++;
			
	 //  uart_send_num(t1ms_cnt);
			
		if(key_long_action > 250)  //长按按键计时  运行长按
		{
			key_long_action = 0;
		//	SendString("long\n");
			switch(key_back_id)
			{
			//	case s1_id:key_cmd = KEY_RUN; break;
			//	case s2_id:key_cmd = KEY_STOP; break;
				//case s3_id_set:key_cmd = KEY_SET;break;
				case s4_id_up:key_cmd = KEY_UP;break;
				case s5_id_down:key_cmd = KEY_DOWN;break;
//				case s6_id:key_cmd = KEY_ENT;break;
				default : key_cmd = 0xff; break;
			}
			  key_isrelease = (key_cmd==0xff)?1:0;//有效按键为0；无效按键为1
		}
		else if(t1ms_cnt > 100)
		{
			t1ms_cnt = 50;
			key_long_action ++;
		 // SendString("short\n");
			switch(key_back_id)
			{
				case s1_id_run:key_cmd = KEY_RUN; break;
				case s2_id_stop:key_cmd = KEY_STOP; break;
				case s3_id_set:key_cmd = KEY_SET;break;
				
				case s4_id_up:key_cmd = KEY_UP;break;
				case s5_id_down:key_cmd = KEY_DOWN;break;
				
				case s6_id_ent:key_cmd = KEY_ENT;break;
				
				default : key_cmd = 0xff; break;
			}
			 key_isrelease = (key_cmd==0xff)?1:0;//有效按键为0；无效按键为1
		}
	}
	else 
	{
		key_isrelease = 1;  //按键松开
		if(t1ms_cnt < 5)
		{
			//	SendString("key err\n");
			t1ms_cnt = 0;
			key_back_id = 0;
			key_long_action = 0;
			return 0xff;
		}
		//	SendString("key release\n");
		switch(key_back_id)
		{
				case s1_id_run:key_cmd = KEY_RUN_RELEASE; break;
				case s2_id_stop:key_cmd = KEY_STOP_RELEASE; break;
				case s4_id_up:key_cmd = KEY_UP_RELEASE;break;
				case s5_id_down:key_cmd = KEY_DOWN_RELEASE;break;
				case s3_id_set:key_cmd =KEY_SET_RELEASE; break;
				case s6_id_ent:key_cmd = KEY_ENT_RELEASE;break;
			
				default : key_cmd = 0xff;
		}
	}
	if(key_cmd != 0xff )//得到按键值清除标志位
	{
		 // SendString("aaa\n");
			t1ms_cnt = 0;
			key_back_id = 0;
			//uart_send_num(key_cmd);
	}
	 
    return key_cmd;
}







