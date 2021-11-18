/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 内部EEPROM举例--------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/* 如果要在文章中应用此代码,请在文章中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//若无特别说明,工作频率一般为11.0592MHz


#include "eeprom.h"
#include "intrins.h"
//#include "uart.h"


uint16_t eeprom_data[4];

/*----------------------------
软件延时
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
关闭IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令寄存器
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //数据缓冲区

    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IapIdle();                      //关闭IAP功能

    return dat;                     //返回
}

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/*----------------------------
扇区擦除
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/*------------------------------------------------
调用IAP函数，完成固定地址的一个字节的写操作
1. 先擦除扇区
2. 写入数据
3. 返回状态：0写入正常  1 写入失败   2擦写失败
-------------------------------------------------*/
//BYTE Write_to_eeprom(WORD addr, BYTE *dat)
//{	
//	uint8_t i = 0;
//	
//	IapEraseSector(IAP_ADDRESS);    //扇区擦除
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
调用IAP函数，完成一个固定地址的读操作
1. 输入地址 uint类型
2. 返回读到的数据 uchar类型
---------------------------------------------------*/
BYTE Read_from_eeprom(WORD addr)
{
	return IapReadByte(IAP_ADDRESS+addr) ;
}

/*------------------------------------------------
调用IAP函数读取8个字节数据到内存中
直接返回无符号整形数据
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
				buff[a] = Read_from_eeprom(a+(i*2)); //从0地址开始读取数据
		     //= ak;
				//printf_d(ak);
			//	SendString("  ");
			}
		  p[i] = ((uint16_t)buff[0]<<8)|buff[1];
	 }
	// SendString("\n");
}

/*------------------------------------------------
调用IAP函数保存8个字节数据到内存中
输入参数为无符号整形 4个字
------------------------------------------------*/
void Write_8byte_eeprom(uint16_t *p)
{
	BYTE i;
	BYTE a;
	uint8_t buff[2];
	
	IapEraseSector(IAP_ADDRESS);    //扇区擦除
	
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
//-  保存4个参数
void save_eeprom(void)
{
	Write_8byte_eeprom(eeprom_data); //设置全局标志变量在MAIN函数中保存数值
}


