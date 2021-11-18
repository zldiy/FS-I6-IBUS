/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 ϵ�� �ڲ�EEPROM����--------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

//��ʾ����Keil������������ѡ��Intel��8058оƬ�ͺŽ��б���
//�����ر�˵��,����Ƶ��һ��Ϊ11.0592MHz


#include "eeprom.h"
#include "intrins.h"
//#include "uart.h"


uint16_t eeprom_data[4];

/*----------------------------
�����ʱ
----------------------------*/
//void Delay(BYTE n)
//{
//    WORD x;

//    while (n--)
//    {
//        x = 0;
//        while (++x);
//    }
//}

/*----------------------------
�ر�IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //�ر�IAP����
    IAP_CMD = 0;                    //�������Ĵ���
    IAP_TRIG = 0;                   //��������Ĵ���
    IAP_ADDRH = 0x80;               //����ַ���õ���IAP����
    IAP_ADDRL = 0;
}

/*----------------------------
��ISP/IAP/EEPROM�����ȡһ�ֽ�
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //���ݻ�����

    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_READ;             //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
    IapIdle();                      //�ر�IAP����

    return dat;                     //����
}

/*----------------------------
дһ�ֽ����ݵ�ISP/IAP/EEPROM����
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}

/*----------------------------
��������
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    IapIdle();
}

/*------------------------------------------------
����IAP��������ɹ̶���ַ��һ���ֽڵ�д����
1. �Ȳ�������
2. д������
3. ����״̬��0д������  1 д��ʧ��   2��дʧ��
-------------------------------------------------*/
//BYTE Write_to_eeprom(WORD addr, BYTE *dat)
//{	
//	uint8_t i = 0;
//	
//	IapEraseSector(IAP_ADDRESS);    //��������
//	for(i=0;i<2;i++)
//		{
//			
//		//	SendData(dat[i]);
//	   IapProgramByte(IAP_ADDRESS + addr + i, dat[i]);
////	   if (IapReadByte(IAP_ADDRESS+addr+i) != dat[i])
////		    return 1;
////	   else return 0;
//	  }
//		return 0;
//}

/*-------------------------------------------------
����IAP���������һ���̶���ַ�Ķ�����
1. �����ַ uint����
2. ���ض��������� uchar����
---------------------------------------------------*/
BYTE Read_from_eeprom(WORD addr)
{
	return IapReadByte(IAP_ADDRESS+addr) ;
}

/*------------------------------------------------
����IAP������ȡ8���ֽ����ݵ��ڴ���
ֱ�ӷ����޷�����������
------------------------------------------------*/
void Read_8byte_eeprom(uint16_t *p)
{
	BYTE i = 0;	
	BYTE a = 0;
	BYTE buff[2];
//	uint8_t ak=0;
	
	//SendString("BB:");
	for(i = 0;i<4;i++)
	 {
		 for(a=0;a<2;a++)
			{
				buff[a] = Read_from_eeprom(a+(i*2)); //��0��ַ��ʼ��ȡ����
		     //= ak;
				//printf_d(ak);
			//	SendString("  ");
			}
		  p[i] = ((uint16_t)buff[0]<<8)|buff[1];
	 }
	// SendString("\n");
}

/*------------------------------------------------
����IAP��������8���ֽ����ݵ��ڴ���
�������Ϊ�޷������� 4����
------------------------------------------------*/
void Write_8byte_eeprom(uint16_t *p)
{
	BYTE i;
	BYTE a;
	uint8_t buff[2];
	
	IapEraseSector(IAP_ADDRESS);    //��������
	
	for(i=0;i<4;i++)
		{
			buff[0] = (p[i] >> 8) & 0x00ff;
			buff[1] = p[i] & 0x00ff;
			
			for(a=0;a<2;a++)
			{
				IapProgramByte(IAP_ADDRESS + (i*2) + a, buff[a]);
			}
		}
}

//------------------------------------------------
//-  ����4������
void save_eeprom(void)
{
	Write_8byte_eeprom(eeprom_data); //����ȫ�ֱ�־������MAIN�����б�����ֵ
}


