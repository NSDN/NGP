#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#ifndef __LCD_H_
#define __LCD_H_


#include "SPIDevice.h"
#include "Fonts.h"

#define IOBUF_SIZE 128

//#define LCD_USE_PRIVATE_FUN

typedef enum {
	Small = 1,
	Big = 2
} LCDFont;

typedef struct {
	SPIDevice* base;
	GPIO_TypeDef* RSTPortGroup;
	uint16_t RSTPortIndex;
	GPIO_TypeDef* BKPortGroup;
	uint16_t BKPortIndex;
	LCDFont Font;
	uint16_t backColor;
	uint16_t foreColor;
} pLCD;

typedef struct {
	pLCD* p;
	#ifdef LCD_USE_PRIVATE_FUN
	void (*writeCommand)(pLCD* p, uint8_t cmd);
	void (*writeData)(pLCD* p, uint8_t data);
	void (*writeData16)(pLCD* p, uint16_t data);
	void (*setPosition)(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
	#endif
	void (*init)(pLCD* p);
	void (*reset)(pLCD* p);
	void (*colorb)(pLCD* p, uint32_t color);
	void (*colorf)(pLCD* p, uint32_t color);
	void (*font)(pLCD* p, LCDFont f);
	void (*clear)(pLCD* p);
	void (*pixel)(pLCD* p, uint8_t x, uint8_t y);
	void (*line)(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
	void (*tri)(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t fill);
	void (*rect)(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill);
	void (*bitmap)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* data);
	void (*bitmapc)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* data);
	void (*bitmapt)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t trans, uint16_t* data);
	void (*bitmaptc)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t trans, uint16_t* data);
	void (*bitmaps)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const unsigned char* data);
	void (*bitmapsc)(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const unsigned char* data);
	void (*draw)(pLCD* p, uint8_t x, uint8_t y, char character);
	void (*print)(pLCD* p, uint8_t x, uint8_t y, char* string);
	int (*printf)(pLCD* p, uint8_t x, uint8_t y, const char* format, ...);
	int (*printfc)(pLCD* p, uint8_t y, const char* format, ...);
} LCD;


void _lcd_writeCommand(pLCD* p, uint8_t cmd) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 0);
	p->base->send(p->base->p, cmd);
	p->base->stop(p->base->p);
}

void _lcd_writeData(pLCD* p, uint8_t data) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->send(p->base->p, data);
	p->base->stop(p->base->p);
}

void _lcd_writeData16(pLCD* p, uint16_t data) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->write(p->base->p, &data, 2);
	p->base->stop(p->base->p);
}

void _lcd_writeData16s(pLCD* p, uint16_t* data, uint32_t length) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->write(p->base->p, data, length * 2);
	p->base->stop(p->base->p);
}

void _lcd_flashData16(pLCD* p, uint16_t data, uint32_t count) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->writes(p->base->p, data, count);
	p->base->stop(p->base->p);
}

void _lcd_setPosition(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { 
	_lcd_writeCommand(p, 0x2a);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, x1 + 2);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, x2 + 2);

	_lcd_writeCommand(p, 0x2b);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, y1 + 3);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, y2 + 3);
	
	_lcd_writeCommand(p, 0x2c);
}

void _lcd_reset(pLCD* p) {
	HAL_GPIO_WritePin(p->RSTPortGroup, p->RSTPortIndex, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(p->RSTPortGroup, p->RSTPortIndex, GPIO_PIN_SET);
	HAL_Delay(50);
}

void _lcd_backLight(pLCD* p, uint8_t state) {
	HAL_GPIO_WritePin(p->BKPortGroup, p->BKPortIndex, (state > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void _lcd_init(pLCD* p) {
	_lcd_reset(p);
	_lcd_backLight(p, 1);
	
	_lcd_writeCommand(p, 0x11); //Sleep exit
	HAL_Delay(500);
		
	//ST7735R Frame Rate
	_lcd_writeCommand(p, 0xB1); //Frame rate ctrl - normal mode
	_lcd_writeData(p, 0x01); 
	_lcd_writeData(p, 0x2C); 
	_lcd_writeData(p, 0x2D); 

	_lcd_writeCommand(p, 0xB2); //Frame rate control - idle mode
	_lcd_writeData(p, 0x01); 
	_lcd_writeData(p, 0x2C); 
	_lcd_writeData(p, 0x2D); 

	_lcd_writeCommand(p, 0xB3); //Frame rate ctrl - partial mode
	_lcd_writeData(p, 0x01); 
	_lcd_writeData(p, 0x2C); 
	_lcd_writeData(p, 0x2D); 
	_lcd_writeData(p, 0x01); 
	_lcd_writeData(p, 0x2C); 
	_lcd_writeData(p, 0x2D); 
	
	_lcd_writeCommand(p, 0xB4); //Column inversion 
	_lcd_writeData(p, 0x07); 
	
	//ST7735R Power Sequence
	_lcd_writeCommand(p, 0xC0); 
	_lcd_writeData(p, 0xA2); 
	_lcd_writeData(p, 0x02); 
	_lcd_writeData(p, 0x84); 
	_lcd_writeCommand(p, 0xC1); 
	_lcd_writeData(p, 0xC5); 

	_lcd_writeCommand(p, 0xC2); 
	_lcd_writeData(p, 0x0A); 
	_lcd_writeData(p, 0x00); 

	_lcd_writeCommand(p, 0xC3); 
	_lcd_writeData(p, 0x8A); 
	_lcd_writeData(p, 0x2A); 
	_lcd_writeCommand(p, 0xC4); 
	_lcd_writeData(p, 0x8A); 
	_lcd_writeData(p, 0xEE); 
	
	_lcd_writeCommand(p, 0xC5); //VCOM 
	_lcd_writeData(p, 0x0E); 
	
	_lcd_writeCommand(p, 0x36); //MX, MY, RGB mode 
	_lcd_writeData(p, 0xC8); 
	
	//ST7735R Gamma Sequence
	_lcd_writeCommand(p, 0xe0); //Magical unicorn dust
	_lcd_writeData(p, 0x0f); 
	_lcd_writeData(p, 0x1a); 
	_lcd_writeData(p, 0x0f); 
	_lcd_writeData(p, 0x18); 
	_lcd_writeData(p, 0x2f); 
	_lcd_writeData(p, 0x28); 
	_lcd_writeData(p, 0x20); 
	_lcd_writeData(p, 0x22); 
	_lcd_writeData(p, 0x1f); 
	_lcd_writeData(p, 0x1b); 
	_lcd_writeData(p, 0x23); 
	_lcd_writeData(p, 0x37); 
	_lcd_writeData(p, 0x00); 	
	_lcd_writeData(p, 0x07); 
	_lcd_writeData(p, 0x02); 
	_lcd_writeData(p, 0x10); 

	_lcd_writeCommand(p, 0xe1); //Sparkles and rainbows
	_lcd_writeData(p, 0x0f); 
	_lcd_writeData(p, 0x1b); 
	_lcd_writeData(p, 0x0f); 
	_lcd_writeData(p, 0x17); 
	_lcd_writeData(p, 0x33); 
	_lcd_writeData(p, 0x2c); 
	_lcd_writeData(p, 0x29); 
	_lcd_writeData(p, 0x2e); 
	_lcd_writeData(p, 0x30); 
	_lcd_writeData(p, 0x30); 
	_lcd_writeData(p, 0x39); 
	_lcd_writeData(p, 0x3f); 
	_lcd_writeData(p, 0x00); 
	_lcd_writeData(p, 0x07); 
	_lcd_writeData(p, 0x03); 
	_lcd_writeData(p, 0x10);  
	
	_lcd_writeCommand(p, 0x2a); //Column address set
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x7f);

	_lcd_writeCommand(p, 0x2b); //Row address set
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x00);
	_lcd_writeData(p, 0x7f);
	
	_lcd_writeCommand(p, 0xF0); //Enable test command  
	_lcd_writeData(p, 0x01); 
	_lcd_writeCommand(p, 0xF6); //Disable ram power save mode 
	_lcd_writeData(p, 0x00); 
	
	_lcd_writeCommand(p, 0x3A); //65k mode 
	_lcd_writeData(p, 0x05); 
	
	_lcd_writeCommand(p, 0x29); //Display on
}

uint16_t _color_conv(uint32_t color) {
	uint16_t c = ((uint16_t)((float)((color & 0xFF0000) >> 16) * 0.122) << 11) |
				 ((uint16_t)((float)((color & 0x00FF00) >> 8 ) * 0.247) << 5 ) |
				 ((uint16_t)((float)((color & 0x0000FF)      ) * 0.122)      ),
		   tmp = c & 0x00FF;
	return (c >> 8) | (tmp << 8);
}

void _lcd_back_color(pLCD* p, uint32_t color) { p->backColor = _color_conv(color); }

void _lcd_fore_color(pLCD* p, uint32_t color) { p->foreColor = _color_conv(color); }

void _lcd_font(pLCD* p, LCDFont f) { p->Font = f; }

void _lcd_clear(pLCD* p) {
	_lcd_setPosition(p, 0, 0, 127, 127);
	_lcd_writeCommand(p, 0x2C);
	_lcd_flashData16(p, p->backColor, 128 * 128);
}

float _lcd_abs(float v) {
	return v > 0 ? v : -v;
}

void _lcd_pixel(pLCD* p, uint8_t x, uint8_t y) {  
	_lcd_setPosition(p, x, y, x, y);
	_lcd_writeCommand(p, 0x2C);
	_lcd_writeData16(p, p->foreColor);
}

void _lcd_line(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	float k = (float)(y2 - y1) / (float)(x2 - x1), tmp = 0;
	if (y2 < y1 || x2 < x1) {
		tmp = y2; y2 = y1; y1 = tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		k = (float)(y2 - y1) / (float)(x2 - x1);
	}
	if (x1 == x2) {
		for (float dy = 0; dy <= y2 - y1; dy += 1) {
			_lcd_setPosition(p, x1, y1 + dy, x1, y1 + dy);
			_lcd_writeCommand(p, 0x2C);
			_lcd_writeData16(p, p->foreColor);
		}
	} else {
		for (float dx = 0, dy = 0; dx <= x2 - x1; dx += 1, dy += k) {
			_lcd_setPosition(p, x1 + dx, y1 + dy, x1 + dx, y1 + dy);
			_lcd_writeCommand(p, 0x2C);
			_lcd_writeData16(p, p->foreColor);
		}
	}
}

void _lcd_tri(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t fill) {
	if (fill) {
		uint8_t maxy = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y1 > y3) ? (y2) : ((y2 > y3) ? y2 : y3));
		uint8_t miny = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y1 < y3) ? (y2) : ((y2 < y3) ? y2 : y3));
		uint8_t midy = (y1 == maxy) ? ((y2 == miny) ? y3 : y2) : ((y1 == miny) ? ((y2 == maxy) ? y3 : y2) : y1);
		uint8_t maxx = (x1 > x2) ? ((x1 > x3) ? x1 : x3) : ((x1 > x3) ? (x2) : ((x2 > x3) ? x2 : x3));
		uint8_t minx = (x1 < x2) ? ((x1 < x3) ? x1 : x3) : ((x1 < x3) ? (x2) : ((x2 < x3) ? x2 : x3));
		uint8_t midx = (x1 == maxx) ? ((x2 == minx) ? x3 : x2) : ((x1 == minx) ? ((x2 == maxx) ? x3 : x2) : x1);
		
		float k1, k2; uint8_t xs, xe, tmp;
		k1 = (float)(maxy - miny) / (float)(maxx - minx);
		
		k2 = (float)(midy - miny) / (float)(midx - minx);
		for (uint8_t i = miny; i <= midy - miny; i++) {
			xs = (float)(i - miny) / k1 + (float)minx;
			xe = (float)(i - miny) / k2 + (float)minx;
			if (xe < xs) { tmp = xe; xe = xs; xs = tmp; }
			_lcd_setPosition(p, xs, i, xe, i);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j <= xe - xs; j++) {
				_lcd_writeData16(p, p->foreColor);
			}
		}
		
		k2 = (float)(maxy - midy) / (float)(maxx - midx);
		for (uint8_t i = midy; i <= maxy - midy; i++) {
			xs = (i - miny) / k1 + minx;
			xe = (i - midy) / k2 + midx;
			if (xe < xs) { tmp = xe; xe = xs; xs = tmp; }
			_lcd_setPosition(p, xs, i, xe, i);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j <= xe - xs; j++) {
				_lcd_writeData16(p, p->foreColor);
			}
		}
	} else {
		_lcd_line(p, x1, y1, x2, y2);
		_lcd_line(p, x2, y2, x3, y3);
		_lcd_line(p, x3, y3, x1, y1);
	}
}

void _lcd_rect(pLCD* p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill) {
	if (fill) {
		_lcd_setPosition(p, x1, y1, x2, y2);
		_lcd_writeCommand(p, 0x2C);
		_lcd_flashData16(p, p->foreColor, 
		(_lcd_abs((char)x2 - (char)x1) + 1) * (_lcd_abs((char)y2 - (char)y1) + 1));
	} else {
		_lcd_line(p, x1, y1, x2, y1);
		_lcd_line(p, x2, y1, x2, y2);
		_lcd_line(p, x2, y2, x1, y2);
		_lcd_line(p, x1, y2, x1, y1);
	}
}

void _lcd_bitmap(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* data) {
	_lcd_setPosition(p, x, y, x + w, y + h);
	_lcd_writeCommand(p, 0x2C);
	uint32_t c = 0;
	for (uint8_t i = 0; i < w; i++) {
		for (uint8_t j = 0; j < h; j++) {
			_lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmapc(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* data) {
	_lcd_setPosition(p, x - w / 2, y - h / 2, x + w / 2, y + h / 2);
	_lcd_writeCommand(p, 0x2C);
	uint32_t c = 0;
	for (uint8_t i = 0; i < w; i++) {
		for (uint8_t j = 0; j < h; j++) {
			_lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmapt(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t trans, uint16_t* data) {
	_lcd_setPosition(p, x, y, x + w, y + h);
	_lcd_writeCommand(p, 0x2C);
	uint32_t c = 0; uint16_t colort = _color_conv(trans);
	for (uint8_t i = 0; i < w; i++) {
		for (uint8_t j = 0; j < h; j++) {
			if (data[c] != colort) _lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmaptc(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t trans, uint16_t* data) {
	_lcd_setPosition(p, x - w / 2, y - h / 2, x + w / 2, y + h / 2);
	_lcd_writeCommand(p, 0x2C);
	uint32_t c = 0; uint16_t colort = _color_conv(trans);
	for (uint8_t i = 0; i < w; i++) {
		for (uint8_t j = 0; j < h; j++) {
			if (data[c] != colort) _lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmaps(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const unsigned char* data) {
	uint32_t c = 0;
	for (uint8_t m = 0; m < h; m += 8) {
		for (uint8_t i = 0; i < w; i++) {
			_lcd_setPosition(p, x + i, y + m, x + i, y + m + 8);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j < 8; j++)
				if (data[c] & (1 << j))
					_lcd_writeData16(p, p->foreColor);
				else 
					_lcd_writeData16(p, p->backColor);
			c++;
		}
	}
}

void _lcd_bitmapsc(pLCD* p, uint8_t x, uint8_t y, uint8_t w, uint8_t h, const unsigned char* data) {
	uint32_t c = 0;
	for (uint8_t m = 0; m < h; m += 8) {
		for (uint8_t i = 0; i < w; i++) {
			_lcd_setPosition(p, x - w / 2 + i, y - h / 2 + m, x - w / 2 + i, y - h / 2 + m + 7);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j < 8; j++) {
				if (data[c] & (1 << j))
					_lcd_writeData16(p, p->foreColor);
				else 
					_lcd_writeData16(p, p->backColor);
			}
			c++;
		}
	}
}

void _lcd_draw(pLCD* p, uint8_t x, uint8_t y, char character) {
	char c = character - ' ';
	if (p->Font == Big)
	{
		if (x >= 128) { x = 0; y = y + 16; }
		for (uint8_t i = 0; i < 8; i++) {
			_lcd_setPosition(p, x + i, y, x + i, y + 7);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j < 8; j++)
				if (__FONTS_BIG_[c * 16 + i] & (1 << j))
					_lcd_writeData16(p, p->foreColor);
				else 
					_lcd_writeData16(p, p->backColor);
		}
		for (uint8_t i = 0; i < 8; i++) {
			_lcd_setPosition(p, x + i, y + 8, x + i, y + 15);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j < 8; j++)
				if (__FONTS_BIG_[c * 16 + i + 8] & (1 << j))
					_lcd_writeData16(p, p->foreColor);
				else 
					_lcd_writeData16(p, p->backColor);
		}
	} else {
		if (x >= 128) { x = 0; y = y + 8; }
		for (uint8_t i = 0; i < 6; i++) {
			_lcd_setPosition(p, x + i, y, x + i, y + 7);
			_lcd_writeCommand(p, 0x2C);
			for (uint8_t j = 0; j < 8; j++)
				if (__FONTS_SMALL_[c][i] & (1 << j))
					_lcd_writeData16(p, p->foreColor);
				else 
					_lcd_writeData16(p, p->backColor);
		}
	}
}

void _lcd_print(pLCD* p, uint8_t x, uint8_t y, char* string) {
	int i = 0;
	if (p->Font == Big) {
		while (string[i] != '\0') {
			if (y > 120) {
				y -= 120;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++; x = 0; y += 16;
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 8;
			if (x > 120) { x = 0; y += 16; }
			i++;
		}
	} else {
		while (string[i] != '\0') {
			if (y > 120) {
				y -= 120;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++; x = 0; y += 8;
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 6;
			if (x > 120) { x = 0; y += 8; }
			i++;
		}
	}
}

int _lcd_printf(pLCD* p, uint8_t x, uint8_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfc(pLCD* p, uint8_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	uint8_t x = (128 - strlen(iobuf) * ((p->Font == Big) ? 8 : 6)) / 2;
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

LCD* LCDInit(
		SPI_HandleTypeDef* pspi, 
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex, 
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex,
		GPIO_TypeDef* pRSTPortGroup, uint16_t pRSTPortIndex,
		GPIO_TypeDef* pBKPortGroup, uint16_t pBKPortIndex) {
	pLCD* p = malloc(sizeof(pLCD));
	p->base = SPIDeviceInit(pspi, pDCPortGroup, pDCPortIndex, pCSPortGroup, pCSPortIndex);
	p->RSTPortGroup = pRSTPortGroup;
	p->RSTPortIndex = pRSTPortIndex;
	p->BKPortGroup = pBKPortGroup;
	p->BKPortIndex = pBKPortIndex;

	p->Font = Small;
	p->backColor = 0x0000;
	p->foreColor = 0xFFFF;
	
	LCD* c = malloc(sizeof(LCD));
	c->p = p;
	#ifdef LCD_USE_PRIVATE_FUN
	c->writeCommand = &_lcd_writeCommand;
	c->writeData = &_lcd_writeData;
	c->writeData16 = &_lcd_writeData16;
	c->setPosition = &_lcd_setPosition;
	#endif
	c->init = &_lcd_init;
	c->reset = &_lcd_reset;
	c->colorb = &_lcd_back_color;
	c->colorf = &_lcd_fore_color;
	c->font = &_lcd_font;
	c->clear = &_lcd_clear;
	c->pixel = &_lcd_pixel;
	c->line = &_lcd_line;
	c->tri = &_lcd_tri;
	c->rect = &_lcd_rect;
	c->bitmap = &_lcd_bitmap;
	c->bitmapc = &_lcd_bitmapc;
	c->bitmapt = &_lcd_bitmapt;
	c->bitmaptc = &_lcd_bitmaptc;
	c->bitmaps = &_lcd_bitmaps;
	c->bitmapsc = &_lcd_bitmapsc;
	c->draw = &_lcd_draw;
	c->print = &_lcd_print;
	c->printf = &_lcd_printf;
	c->printfc = &_lcd_printfc;
	
	return c;
}


#endif
