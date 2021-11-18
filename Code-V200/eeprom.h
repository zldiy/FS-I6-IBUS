#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "STC15Wxx.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

//-----------------------------------------------

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//- 扇区地址
#define IAP_ADDRESS 0x0200
//- STC15W202S  3K EEPROM 起始0000h  0BFFh  512 为一个字节  根据文档851页 202S有6个扇区



void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
//BYTE Write_to_eeprom(WORD addr, BYTE *dat);
BYTE Read_from_eeprom(WORD addr);

void Read_8byte_eeprom(uint16_t *p);
void Write_8byte_eeprom(uint16_t *p);
void save_eeprom(void);

extern uint16_t eeprom_data[];

#define Run_Time     eeprom_data[0]
#define Run_Dimming  eeprom_data[1]
#define Run_High_value    eeprom_data[2]
#define Run_Low_value     eeprom_data[3]

#endif

