

#ifndef	_TM1638_H
#define	_TM1638_H

//�����������ʾ����


//TM1638ģ�����Ŷ���

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
//void TM1638_Write(unsigned char	DATA);			//д���ݺ���
//unsigned char TM1638_Read(void)	;				//�����ݺ���
//void Write_COM(unsigned char cmd);		//����������
//unsigned char Read_key(void);
void Write_DATA(unsigned char add,unsigned char DATA);		//ָ����ַд������
void Write_allLED(unsigned char LED_flag)		;			//����ȫ��LED������LED_flag��ʾ����LED״̬
void init_TM1638(void);
//void value_to_seg(unsigned int value,uint8_t *p);
void refresh_display(unsigned int value,uint8_t led_bit,uint8_t dis_mode);
unsigned char key_scan(void);




#endif


