

#ifndef	_TM1638_H
#define	_TM1638_H

//共阴数码管显示代码


//TM1638模块引脚定义

sbit	STB=P3^6;
sbit	CLK=P3^3;
sbit	DIO=P5^5;

#define LED_OFF 0


enum key_id
{
	s1_id_run,
	s2_id_stop,
	s3_id_set,
	s4_id_up,
	s5_id_down,
	s6_id_ent,
};

enum KEY_FLAG
{
	nomal,
	KEY_RUN,
	KEY_STOP,
	KEY_SET,
	KEY_UP,
	KEY_DOWN,
	KEY_ENT,
	
	KEY_RUN_RELEASE,
	KEY_STOP_RELEASE,
	KEY_SET_RELEASE,
	KEY_UP_RELEASE,
	KEY_DOWN_RELEASE,
	KEY_ENT_RELEASE
};


void Delay5us();		//@11.0592MHz
//void TM1638_Write(unsigned char	DATA);			//写数据函数
//unsigned char TM1638_Read(void)	;				//读数据函数
//void Write_COM(unsigned char cmd);		//发送命令字
//unsigned char Read_key(void);
void Write_DATA(unsigned char add,unsigned char DATA);		//指定地址写入数据
void Write_allLED(unsigned char LED_flag)		;			//控制全部LED函数，LED_flag表示各个LED状态
void init_TM1638(void);
//void value_to_seg(unsigned int value,uint8_t *p);
void refresh_display(unsigned int value,uint8_t led_bit,uint8_t dis_mode);
unsigned char key_scan(void);




#endif


