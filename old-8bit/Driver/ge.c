#include "ge.h"	
#include "oledfont.h"

/**********************************************
//Set current position
//备注：X坐标的高四位在IIC传输的时候按五位传输
		即最高位为1
**********************************************/
void _GE_SET_POS_(uint8_t x, uint8_t y)
{
	IIC_Command(0xb0 + y);				   //Set page address(0~7)
	IIC_Command(((x & 0xf0) >> 4) | 0x10); //Set row high address
	IIC_Command((x & 0x0f));			   //Set row low address
}

/**********************************************
//Choose the target screen
**********************************************/
void geDevice(uint8_t Address)
{
	IIC_Set_Address(Address);
}

/**********************************************
//Initilize the target screen
**********************************************/
void geInit()
{
	IIC_Command(0xAE);//--display off
	IIC_Command(0x00);//---set low column address
	IIC_Command(0x10);//---set high column address
	IIC_Command(0x40);//--set start line address  
	IIC_Command(0xB0);//--set page address
	IIC_Command(0x81); // contract control
	IIC_Command(0xFF);//--128   
	IIC_Command(0xA1);//set segment remap 
	IIC_Command(0xA6);//--normal / reverse
	IIC_Command(0xA8);//--set multiplex ratio(1 to 64)
	if (IIC_Get_Address() == SCREEN_P)
		IIC_Command(0x3F);//--1/32 duty
	else if (IIC_Get_Address() == SCREEN_S)
		IIC_Command(0x1F);
	IIC_Command(0xC8);//Com scan direction
	IIC_Command(0xD3);//-set display offset
	IIC_Command(0x00);//

	IIC_Command(0xD5);//set osc division
	IIC_Command(0x80);//

	IIC_Command(0xD8);//set area color mode off
	IIC_Command(0x05);//

	IIC_Command(0xD9);//Set Pre-Charge Period
	IIC_Command(0xF1);//

	IIC_Command(0xDA);//set com pin configuartion
	if (IIC_Get_Address() == SCREEN_P)
		IIC_Command(0x12);//
	else if (IIC_Get_Address() == SCREEN_S)
		IIC_Command(0x02);

	IIC_Command(0xDB);//set Vcomh
	IIC_Command(0x30);//

	IIC_Command(0x8D);//set charge pump enable
	IIC_Command(0x14);//

	IIC_Command(0xAF);//--turn on oled panel

	IIC_Command(0x21); //Set column address range
	IIC_Command(0x00);
	IIC_Command(0x7F); //127

	IIC_Command(0x22); //Set page address range
	IIC_Command(0x00);
	if (IIC_Get_Address() == SCREEN_P)
		IIC_Command(0x07); //7
	else if	(IIC_Get_Address() == SCREEN_S)
		IIC_Command(0x03); //3
}

/**********************************************
//Open the target screen
**********************************************/
void geOpen()
{
	IIC_Command(0X8D);  //SET DCDC
	IIC_Command(0X14);  //DCDC ON
	IIC_Command(0XAF);  //DISPLAY ON
}

/**********************************************
//Close the target screen
**********************************************/
void geClose()
{
	IIC_Command(0X8D);  //SET DCDC
	IIC_Command(0X10);  //DCDC OFF
	IIC_Command(0XAE);  //DISPLAY OFF
}

/**********************************************
//Set brightness of the target screen
**********************************************/
void geBrightness(uint8_t Brightness)
{
	IIC_Command(0x81);       // contrast control
	IIC_Command(Brightness); //--128
}

/**********************************************
//Draw char without buffer with FontBig
**********************************************/
void geChar(uint8_t x, uint8_t y, char Char, uint8_t Size)
{
	unsigned char c = 0, i = 0;
	c = Char - ' ';		
	if (Size == GE_FONT_BIG)
	{
		if (x >= 128) { x = 0; y = y + 2; }
		_GE_SET_POS_(x, y);
		for (i = 0; i < 8; i++)
			IIC_Data(FontBig[c * 16 + i]);
		_GE_SET_POS_(x, y + 1);
		for (i = 0; i < 8; i++)
			IIC_Data(FontBig[c * 16 + i + 8]);
	} else {
		if (x >= 128) { x = 0; y = y + 1; }
		_GE_SET_POS_(x, y);
		for (i = 0; i < 6; i++)
			IIC_Data(FontSmall[c][i]);
	}
}

/**********************************************
//Draw string without buffer with FontBig
**********************************************/
void geString(uint8_t x, uint8_t y, char* Str, uint8_t Size)
{
	uint8_t j = 0;
	if (Size == GE_FONT_BIG) {
		while (Str[j] != '\0') {
			if (Str[j] == '\n') {
				j++; x = 0; y += 2;
				continue;
			}
			geChar(x, y, Str[j], Size);
			x += 8;
			if (x > 120) { x = 0; y += 2; }
			j++;
		}
	} else {
		while (Str[j] != '\0') {
			if (Str[j] == '\n') {
				j++; x = 0; y++;
				continue;
			}
			geChar(x, y, Str[j], Size);
			x += 6;
			if (x > 120) { x = 0; y += 1; }
			j++;
		}
	}
}

/**********************************************
//Fill the screen buffer (physical) 
**********************************************/
void geData(uint8_t Data[])
{
	uint32_t i = 0;
	uint8_t m, n;

	for (m = 0; m < 8; m++) {
		_GE_SET_POS_(0, m);
		for (n = 0; n < 128; n++) {
			IIC_Data(Data[i]);
			i++;
		}
	}
}