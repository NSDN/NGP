#ifndef __BINOLED_H_
#define __BINOLED_H_


#include "iicdev.h"

#define IOBUF_SIZE 128

/*
12864:
[0]0 1 2 3 ... 127
[1]0 1 2 3 ... 127
[2]0 1 2 3 ... 127
[3]0 1 2 3 ... 127
[4]0 1 2 3 ... 127
[5]0 1 2 3 ... 127
[6]0 1 2 3 ... 127
[7]0 1 2 3 ... 127
*/
#define OLED_SCREEN_BIG 0x7A

/*
12832:
[1]0 1 2 3 ... 127
[3]0 1 2 3 ... 127
[5]0 1 2 3 ... 127
[7]0 1 2 3 ... 127
*/
#define OLED_SCREEN_SMALL 0x78

//#define OLED_USE_PRIVATE_FUN

typedef enum {
	Black = 0,
	White = 1
} OLEDColor;

typedef enum {
	FontSmall = 1,
	FontBig = 2
} OLEDFontSize;

typedef struct {
	SoftIIC* base;
	OLEDColor Color;
	OLEDFontSize FontSize;
} pOLED;

typedef struct {
	pOLED* p;
	void (*trans)(pOLED* p, uint8_t addr);
	#ifdef OLED_USE_PRIVATE_FUN
	void (*writeCommand)(pOLED* p, uint8_t cmd);
	void (*writeData)(pOLED* p, uint8_t data);
	void (*setPosition)(pOLED* p, uint8_t x, uint8_t y);
	#endif
	void (*color)(pOLED* p, OLEDColor color);
	void (*font)(pOLED* p, OLEDFontSize size);
	void (*clear)(pOLED* p);
	void (*init)(pOLED* p);
	void (*bright)(pOLED* p, uint8_t brightness);
	void (*open)(pOLED* p);
	void (*close)(pOLED* p);
	void (*flash)(pOLED* p, const unsigned char* data);
	void (*draw)(pOLED* p, uint8_t x, uint8_t y, char character);
	void (*print)(pOLED* p, uint8_t x, uint8_t y, char* string);
	int (*printf)(pOLED* p, uint8_t x, uint8_t y, const char* format, ...);
	int (*printfc)(pOLED* p, uint8_t y, const char* format, ...);
} OLED;

OLED* OLEDInit(
		GPIO_TypeDef* SDAPortGroup, uint16_t SDAPortIndex,
		GPIO_TypeDef* SCLPortGroup, uint16_t SCLPortIndex,
		uint8_t address);


#endif
