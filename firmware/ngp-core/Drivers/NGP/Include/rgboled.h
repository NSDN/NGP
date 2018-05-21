#ifndef __RGBOLED_H_
#define __RGBOLED_H_


#include "spidev.h"

#define RGB_IOBUF_SIZE 128
#define RGB_IOBUF_WIDTH 32
#define RGB_IOBUF_HEIGHT 16

#define RGB_PORTRAIT 0
#define RGB_PORTRAIT_ANTI 1
#define RGB_LANDSCAPE 2
#define RGB_LANDSCAPE_ANTI 3

//#define RGB_USE_PRIVATE_FUN

typedef enum {
	RGBSmall = 1,
	RGBBig = 2
} RGBLEDFont;

typedef struct {
	SPIDevice* base;
	GPIO_TypeDef* RSTPortGroup;
	uint16_t RSTPortIndex;
	uint16_t width;
	uint16_t height;
	RGBLEDFont Font;
	uint16_t backColor;
	uint16_t foreColor;
	uint8_t rotate;
	uint16_t ptrX;
	uint16_t ptrY;
	char buffer[RGB_IOBUF_HEIGHT][RGB_IOBUF_WIDTH];
} pRGBOLED;

typedef struct {
	pRGBOLED* p;
	#ifdef RGB_USE_PRIVATE_FUN
	void (*writeCommand)(pRGBOLED* p, uint8_t cmd);
	void (*writeData)(pRGBOLED* p, uint8_t data);
	void (*writeData16)(pRGBOLED* p, uint32_t data);
	void (*setPosition)(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	#endif
	void (*init)(pRGBOLED* p);
	void (*reset)(pRGBOLED* p);
	void (*colorb)(pRGBOLED* p, uint32_t color);
	void (*colorf)(pRGBOLED* p, uint32_t color);
	void (*font)(pRGBOLED* p, RGBLEDFont f);
	void (*clear)(pRGBOLED* p);
	void (*scroll)(pRGBOLED* p, uint16_t pos);
	void (*rotate)(pRGBOLED* p, uint8_t r);
	void (*pixel)(pRGBOLED* p, uint16_t x, uint16_t y);
	void (*line)(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*tri)(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill);
	void (*rect)(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill);
	void (*bitmap)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data);
	void (*bitmapc)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data);
	void (*bitmapt)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint32_t* data);
	void (*bitmaptc)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint32_t* data);
	void (*bitmaps)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*bitmapsc)(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*draw)(pRGBOLED* p, uint16_t x, uint16_t y, char character);
	void (*print)(pRGBOLED* p, uint16_t x, uint16_t y, char* string);
	int (*printf)(pRGBOLED* p, uint16_t x, uint16_t y, const char* format, ...);
	int (*printfc)(pRGBOLED* p, uint16_t y, const char* format, ...);
	int (*printfa)(pRGBOLED* p, const char* format, ...);
} RGBOLED;

RGBOLED* RGBOLEDInit(
		SPI_HandleTypeDef* pspi,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex,
		GPIO_TypeDef* pRSTPortGroup, uint16_t pRSTPortIndex);


#endif
