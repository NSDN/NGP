#include "ge.h"	
#include "oledfont.h"
#include <stdlib.h>

/**********************************************
//Function to print screen
**********************************************/
void _SCREEN_PRINT_()
{
	uint8_t i, j;
	for (i = 0; i < 4; i++) {
		_GE_SET_POS_(0, i);
		for (j = 0; j < 128; j++)
			IIC_Data(((_SCREEN_BUF[j / 4] & (1 << (i * 2))) ? 0x0F : 0x00) | ((_SCREEN_BUF[j / 4] & (1 << (i * 2 + 1))) ? 0xF0 : 0x00));
	   	_GE_SET_POS_(0, i + 4);
		for (j = 0; j < 128; j++)
			IIC_Data(((_SCREEN_BUF[32 + j / 4] & (1 << (i * 2))) ? 0x0F : 0x00) | ((_SCREEN_BUF[32 + j / 4] & (1 << (i * 2 + 1))) ? 0xF0 : 0x00));
	}
}

/**********************************************
//Function to write screen buffer
**********************************************/
void _SCREEN_BUF_PRINT_()
{
	switch (_GE_DRAW_TYPE) {
	case GE_DRAWTYPE_POINT:
		_GE_VERTEX_BUF_POINTER = &_GE_VERTEX_BUF;
		do
		{
			if (_GE_VERTEX_BUF_POINTER->x == 255) break;
			_SCREEN_BUF[_GE_VERTEX_BUF_POINTER->x + _GE_VERTEX_BUF_POINTER->y / 8 * 32] = _SCREEN_BUF[_GE_VERTEX_BUF_POINTER->x + _GE_VERTEX_BUF_POINTER->y / 8 * 32] | _GE_VERTEX_BUF_POINTER->color << (_GE_VERTEX_BUF_POINTER->y % 8);
			_GE_VERTEX_BUF_POINTER = _GE_VERTEX_BUF_POINTER->next;
		} while (_GE_VERTEX_BUF_POINTER != null);
		break;
	case GE_DRAWTYPE_LINE:

		break;
	case GE_DRAWTYPE_TRIANGLE:

		break;
	case GE_DRAWTYPE_TRIANGLE_STRIP:

		break;
	case GE_DRAWTYPE_QUAD:

		break;
	case GE_DRAWTYPE_QUAD_STRIP:

		break;
	default:
		break;
	}
}

/**********************************************
//Construct vertex buffer(linked list)
**********************************************/
void _GE_VERTEX_BUF_INIT_(geV2* VertexBuf, uint8_t Count)
{
	VertexBuf->x = 255;
	VertexBuf->y = 255;
	VertexBuf->color = _GE_COLOR_DEFAULT;
	if (Count > _GE_VERTEX_BUF_MAX) {
		VertexBuf->next = null;
		return;
	}
	VertexBuf->next = malloc(sizeof(geV2));
	_GE_VERTEX_BUF_INIT_(VertexBuf->next, Count + 1);
}

/**********************************************
//Clear vertex buffer(linked list)
**********************************************/
void _GE_VERTEX_BUF_CLEAR_(geV2* VertexBuf, uint8_t Count)
{
	VertexBuf->x = 255;
	VertexBuf->y = 255;
	VertexBuf->color = _GE_COLOR_DEFAULT;
	if (Count > _GE_VERTEX_BUF_MAX) return;
	_GE_VERTEX_BUF_CLEAR_(VertexBuf->next, Count + 1);
}

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

	_GE_COLOR_BUF = _GE_COLOR_DEFAULT;
	//_GE_VERTEX_BUF_POINTER = &_GE_VERTEX_BUF;
	//_GE_VERTEX_BUF_INIT_(&_GE_VERTEX_BUF, 0);
	geClear(GE_COLOR_BLACK);
	_SCREEN_PRINT_();
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
//Clear the target screen
**********************************************/
void geClear(uint8_t Color)
{
	uint8_t i;
	for (i = 0; i < 64; i++)
		_SCREEN_BUF[i] = Color ? 0xff : 0x00;
}

/**********************************************
//Begin the draw
**********************************************/
void geBegin(uint8_t DrawType)
{
	_GE_DRAW_TYPE = DrawType;
}

/**********************************************
//End the draw and print to screen buffer
**********************************************/
void geEnd()
{
    _GE_VERTEX_BUF_POINTER = &_GE_VERTEX_BUF;
	_SCREEN_BUF_PRINT_();
}

/**********************************************
//Print the buffer to screen
**********************************************/
void geFlush()
{
	_SCREEN_PRINT_();
    //_GE_VERTEX_BUF_POINTER = &_GE_VERTEX_BUF;
	//_GE_VERTEX_BUF_CLEAR_(&_GE_VERTEX_BUF, 0);
}

/**********************************************
//Set the vertex
**********************************************/
void geVertex2(uint8_t x, uint8_t y)
{
	_GE_VERTEX_BUF_POINTER->x = x;
	_GE_VERTEX_BUF_POINTER->y = y;
	_GE_VERTEX_BUF_POINTER->color = _GE_COLOR_BUF;
	_GE_VERTEX_BUF_POINTER = _GE_VERTEX_BUF_POINTER->next;
}

/**********************************************
//Set the color
**********************************************/
void geColor(uint8_t color)
{
	_GE_COLOR_BUF = color;
}

/**********************************************
//Set the fill mode
**********************************************/
void geFill(uint8_t FillMode)
{
	_GE_FILL_MODE = FillMode;
}

/**********************************************
//Draw a point without buffer
**********************************************/
void gePoint(uint8_t x, uint8_t y)
{
	_SCREEN_BUF[x + y / 8 * 32] = 
	_SCREEN_BUF[x + y / 8 * 32] | _GE_COLOR_BUF << (y % 8);
}

#ifndef USE_BIG_FONT
/**********************************************
//Draw char without buffer
**********************************************/
void geChar(uint8_t x, uint8_t y, char Char)
{
	unsigned char c = 0, i = 0;
	c = Char - ' ';		
	if (x >= 128) { x = 0; y = y + 1; }
	_GE_SET_POS_(x, y);
	for (i = 0; i < 6; i++)
		IIC_Data(FontSmall[c][i]);
	
}

/**********************************************
//Draw string without buffer
**********************************************/
void geString(uint8_t x, uint8_t y, char* Str)
{
	uint8_t j = 0;
	while (Str[j] != '\0') {
		if (Str[j] == '\n') {
			j++; x = 0; y++;
			continue;
		}
		geChar(x, y, Str[j]);
		x += 6;
		if (x > 120) { x = 0; y += 1; }
		j++;
	}
}
#endif

#ifdef USE_FONT_BIG
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
#endif

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