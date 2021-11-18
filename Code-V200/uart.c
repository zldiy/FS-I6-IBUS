

#include "uart.h"
#include "main.h"
#include "eeprom.h"

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

bit rx_flag_bit =0 ;
bit rx_timer_bit = 0;

uint8_t  Rx_buff[32];
uint8_t  Rx_cnt = 0;
uint16_t CH_v[14]; //�洢��������

/*
void UartInit(void)		//19200bps@12MHz
{
	
//	ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//  P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	SCON = 0x50;		//8λ���ݣ��ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��Ϊfosc 1T
	T2L = 0x64;		  //�趨��ʱ����ֵ
	T2H = 0xff;		  //�趨��ʱ����ֵ
	AUXR |= 0x10;		//������ʱ��2
	TI = 1;
	ES = 1;
}
*/

void UartInit(void)		//115200bps@22.1184MHz
{
	SCON = 0x50;		//
	AUXR |= 0x01;		//
	AUXR |= 0x04;		//
	T2L = 0xD0;		//0xe8 @ 110592
	T2H = 0xFF;		//0xfe @ 110592
	AUXR |= 0x10;		//start timer2
	
	TI=0;
	RI=0;
	
	ES=1;
}




/*----------------------------
���ڷ���һ������
----------------------------*/
void SendData(unsigned char dat)
{
    TI = 0;                      //������ͱ�־λ
    SBUF = dat;                  //��������
   	 while (!TI);                 //�ȴ����ݷ������
   // return dat;
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
	//ET0 = 0;    
	//EA = 0;
    while (*s)                  //����ַ���������־λ
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
	//ET0 =1;    
	//EA = 1;
}

/*----------------------------
���ڽ����жϷ������ 
-----------------------------*/
void Uart() interrupt 4 //using 1
{
	static uint8_t rx_flag = 0;
	//rx_flag_bit = 1;
	if (RI)
    {
      RI = 0;                 //�����жϣ�����������жϱ�־
			rx_timer_bit = 1;
			if(rx_flag == 0)
			{
				if(SBUF == 0x20)
				rx_flag = 1;
			}
			else if(rx_flag == 1)
			{
				if(SBUF == 0x40)
				{
					rx_flag = 3;
					Rx_cnt = 0;
				}
			}
			else if(rx_flag == 3)
			{
				Rx_buff[Rx_cnt] = SBUF;            //����˿�����ָʾ��������
				Rx_cnt ++;			
				if(Rx_cnt == 30) //recive 0-29 ��30������
				{	
					Rx_cnt = 0;
					rx_flag = 0;
					rx_timer_bit = 0; //ֹͣ��ʱ��ʱ��
					rx_flag_bit = 1;
				}
			}
    }
    if (TI)
    {
        TI = 0;                 //�����жϣ�����������жϱ�־λ
      //  busy = 0;               //���æ��־λ	
    }
}


//--------------  printf ------------------
void printf_d(uint16_t num)
{
	char str[6];

	str[0] = (num / 10000) + 0x30;
	str[1] = (num % 10000 / 1000) + 0x30;
	str[2]  = (num %1000 / 100) + 0x30;
	str[3]  = (num % 100 / 10) + 0x30;
	str[4] = (num % 10) + 0x30;
	str[5] = 0;
	
	//ET0 = 0;    
	//EA = 0;
	SendString(str);
	//ET0 = 1;    //������ʱ��0�ж�ʹ��
	//EA = 1;
}
/*
void analayze_rx(uint8_t *p)
{
	uint8_t buff;
   
	buff = ((p[1]-0x30))*10 + (p[2]-0x30);
	if(p[0]=='H')
	{
	 //buff = ((p[1]-0x30))*10 + (p[2]-0x30);

		if(buff >= 99)
			buff = 100;
		else if(buff<50)
			buff = 50;
		Run_High_value = buff;
	}
	else if(p[0]=='L')
	{
		//buff = ((p[1]-0x30)*10) + (p[2]-0x30);
		if(buff > 50)
			buff = 50;
		else if(buff<0)
			buff = 1;
		Run_Low_value = buff; 
	}
	else if(p[0]=='M')
	{
		//buff = ((p[1]-0x30)*10) + (p[2]-0x30);
		if(buff >= 99)
			buff = 100;
		else if(buff<0)
			buff = 1;
		Run_Dimming = buff; 
		timer_cnt = Run_Dimming * 30;
	}
	else 
	{
		SendString("Rx Err\n");
	}
	for(buff = 0;buff<3;buff++)
	{
		SendData(p[buff]);
	}	
	p[0]=0;
	p[1]=0;
	p[2]=0;
}

*/
/*
void analayze_rx(uint8_t *p)
{
	if((p[0] == '')&&(p[3] == 0x5a))
	{
		switch(p[1])
		{
			case 0x01 : //set mode
				if(p[2] > 4)
					p[2] = 4;
				work_mode = p[2];
				//SendString("Mode SET:");
				break; 
			case 0x02 : //High light value
				if(p[2] > 100)
					p[2] = 100;
				Run_High_value = p[2];
				//SendString("HIGH SET:");
				break;
			case 0x03 : //Low light value
				if(p[2] > 100)
					p[2] = 100;
				Run_Low_value = p[2];
				//SendString("LOW SET:");
				break;
			case 0x04 : //Adj light value
				if(p[2] > 100)
					p[2] = 100;
				Run_Dimming = p[2];
				//SendString("adj SET:");
				break;
		}
		SendString(p);
	  SendData('\n');
	}
	else 
	{
		SendString("Rx Err\n");
	}
}*/


//----------------------------------------------
//-- ����14��ͨ����ibus�ź�
//----------------------------------------------
void rx_decode(uint16_t *ch,uint8_t *val)
{
	uint8_t i = 0;
	uint16_t check_sum = 0;
	uint16_t check_sum_rx = 0;
	
	for(i=0;i<30;i++)
	{
		check_sum += val[i];
	}
	
	check_sum = check_sum^0xffff;
	check_sum_rx = val[30]|(val[31]<<8);
	
	//if(check_sum == check_sum_rx)
	{
		//SendString("Check_OK\r\n");
		
		for(i=0;i<14;i++)
		{
			ch[i]=val[i*2]|((uint16_t)(val[i*2+1]<<8)&0xf00); //decode recivce code 
		}
	}
}




//--------------------End----------------------------