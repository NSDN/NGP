#include "base.h"

#ifndef __GE_H
#define __GE_H

/*
12864的显存存放格式如下:
[0]0 1 2 3 ... 127
[1]0 1 2 3 ... 127
[2]0 1 2 3 ... 127
[3]0 1 2 3 ... 127
[4]0 1 2 3 ... 127
[5]0 1 2 3 ... 127
[6]0 1 2 3 ... 127
[7]0 1 2 3 ... 127
*/

/*
12832的显存存放格式如下:
[1]0 1 2 3 ... 127
[3]0 1 2 3 ... 127
[5]0 1 2 3 ... 127
[7]0 1 2 3 ... 127
*/

void _SCREEN_PRINT_();

/*
目前采用的是32x16的显存大小
从上到下为低到高位
*/
static uint8_t _SCREEN_BUF[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //0-3
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //4-7
};
void _SCREEN_BUF_PRINT_();

#ifdef USE_BIG_FONT
#define GE_FONT_BIG 2
#define GE_FONT_SMALL 1
#endif

typedef struct _GE_VERTEX2 {
	uint8_t x;
	uint8_t y;
	uint8_t color;
	struct geV2* next;
} geV2;

#define GE_COLOR_WHITE 1
#define GE_COLOR_BLACK 0
#define _GE_COLOR_DEFAULT GE_COLOR_BLACK
static uint8_t _GE_COLOR_BUF = 0;

static uint8_t _GE_VERTEX_BUF_MAX = 32;
static geV2 _GE_VERTEX_BUF;
static geV2* _GE_VERTEX_BUF_POINTER = null;

#define GE_DRAWTYPE_POINT 0x01
#define GE_DRAWTYPE_LINE 0x02
#define GE_DRAWTYPE_TRIANGLE 0x04
#define GE_DRAWTYPE_TRIANGLE_STRIP 0x08
#define GE_DRAWTYPE_QUAD 0x10
#define GE_DRAWTYPE_QUAD_STRIP 0x20
static uint8_t _GE_DRAW_TYPE = 0;

#define GE_FILLMODE_NONE 0x00
#define GE_FILLMODE_SOLID 0x01
static uint8_t _GE_FILL_MODE = 0;

#define GE_CLEARTYPE_COLOR 0x01
#define GE_CLEARTYPE_VERTEX 0x02
#define GE_CLEARTYPE_SCREEN 0x04

void _GE_VERTEX_BUF_INIT_(geV2* VertexBuf, uint8_t Count);
void _GE_VERTEX_BUF_CLEAR_(geV2* VertexBuf, uint8_t Count);

void _GE_SET_POS_(uint8_t x, uint8_t y);

void geDevice(uint8_t Address);
void geInit();
void geOpen();
void geClose();
void geBrightness(uint8_t Brightness);
void geClear(uint8_t Color);
void geBegin(uint8_t DrawType);
void geEnd();
void geFlush();
void geVertex2(uint8_t x, uint8_t y);
void geColor(uint8_t Color);
void geFill(uint8_t FillMode);
void gePoint(uint8_t x, uint8_t y);
#ifndef USE_BIG_FONT
void geChar(uint8_t x, uint8_t y, char Char);
void geString(uint8_t x, uint8_t y, char* Str);
#endif
#ifdef USE_BIG_FONT
void geChar(uint8_t x, uint8_t y, char Char, uint8_t Size);
void geString(uint8_t x, uint8_t y, char* Str, uint8_t Size);
#endif
void geData(uint8_t* Data);


#endif // !__GE_H
