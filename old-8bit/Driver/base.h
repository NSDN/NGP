#include "reg15.h"

#ifndef __BASE_H
#define __BASE_H


#define uint8_t unsigned char
#define uint32_t unsigned int
#define null 0

void delay(uint32_t t);
void delayMicroseconds(uint32_t t);
uint32_t pow(uint32_t x, uint32_t a);

#define DEBUG_PORT P55
#define DebugHigh() DEBUG_PORT=1
#define DebugLow() DEBUG_PORT=0
static uint32_t TEMP = 0;

#define SPEAKER_L P31
#define SPEAKER_R P30

#define SCREEN_P 0x7A
#define SCREEN_S 0x78

static uint8_t IIC_ADDRESS = 0x00;

#define IIC_CMD  0
#define IIC_DATA 1
#define IIC_SCL P33
#define IIC_SDA P32

#define IIC_SCL_Clr() IIC_SCL=0
#define IIC_SCL_Set() IIC_SCL=1

#define IIC_SDA_Clr() IIC_SDA=0
#define IIC_SDA_Set() IIC_SDA=1

void IIC_Set_Address(uint8_t IIC_Address);
uint8_t IIC_Get_Address();
void IIC_Start();
void IIC_Stop();
void IIC_Command(uint8_t IIC_Command);
void IIC_Data(uint8_t IIC_Data);
void IIC_Byte(uint8_t IIC_Byte);
void IIC_Wait_Ack();


#endif // !__BASE_H
