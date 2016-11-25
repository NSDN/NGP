#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#ifndef __LCD_H_
#define __LCD_H_


#include "SPIDevice.h"
#include "Fonts.h"

#define IOBUF_SIZE 128

typedef enum {
	FontSmall = 1,
	FontBig = 2
} LCDFont;

typedef struct {
	SPIDevice* base;
	GPIO_TypeDef* BKPortGroup;
	uint16_t BKPortIndex;
	LCDFont Font;
} pLCD;

typedef struct {
	pLCD* p;
	void (*writeCommand)(pLCD* p, uint8_t cmd);
	void (*writeData)(pLCD* p, uint8_t data);
	void (*setPosition)(pLCD* p, uint8_t x, uint8_t y);
	void (*color)(pLCD* p, LCDColor color);
	void (*font)(pLCD* p, LCDFont f);
	void (*clear)(pLCD* p);
	void (*init)(pLCD* p);
	void (*draw)(pLCD* p, uint8_t x, uint8_t y, char character);
	void (*print)(pLCD* p, uint8_t x, uint8_t y, char* string);
	int (*printf)(pLCD* p, uint8_t x, uint8_t y, const char* format, ...);
	int (*printfc)(pLCD* p, uint8_t y, const char* format, ...);
} LCD;


void _lcd_writeCommand(pLCD* p, uint8_t cmd) {
	p->base->start(p->base->p);
	p->base->send(p->base->p, p->base->p->IICAddress);
	p->base->wait(p->base->p);
	p->base->send(p->base->p, 0x00);
	p->base->wait(p->base->p);
	p->base->send(p->base->p, cmd);
	p->base->wait(p->base->p);
	p->base->stop(p->base->p);
}

void _lcd_writeData(pLCD* p, uint8_t data) {
	p->base->start(p->base->p);
	p->base->send(p->base->p, p->base->p->IICAddress);
	p->base->wait(p->base->p);
	p->base->send(p->base->p, 0x40);
	p->base->wait(p->base->p);
	p->base->send(p->base->p, data);
	p->base->wait(p->base->p);
	p->base->stop(p->base->p);
}

void _lcd_setPosition(pLCD* p, uint8_t x, uint8_t y) { 
	_lcd_writeCommand(p, 0xb0 + y);
	_lcd_writeCommand(p, ((x & 0xf0) >> 4) | 0x10);
	_lcd_writeCommand(p, x & 0x0f);
}

void _lcd_color(pLCD* p, LCDColor color) { p->Color = color; }

void _lcd_font(pLCD* p, LCDFont f) { p->Font = f; }

void _lcd_clear(pLCD* p) {
	uint8_t max = (p->base->p->IICAddress == LCD_SCREEN_BIG) ? 8 : 4;
	for (uint8_t m = 0; m < max; m++) {
		_lcd_setPosition(p, 0, m);
		for (uint8_t n = 0; n < 128; n++) {
			_lcd_writeData(p, 0x00);
		}
	}
}

void _lcd_init(pLCD* p) {
	_lcd_writeCommand(p, 0xAE);//--display off
	_lcd_writeCommand(p, 0x00);//---set low column address
	_lcd_writeCommand(p, 0x10);//---set high column address
	_lcd_writeCommand(p, 0x40);//--set start line address  
	_lcd_writeCommand(p, 0xB0);//--set page address
	_lcd_writeCommand(p, 0x81); // contract control
	_lcd_writeCommand(p, 0xFF);//--128   
	_lcd_writeCommand(p, 0xA1);//set segment remap 
	_lcd_writeCommand(p, 0xA6);//--normal / reverse

	_lcd_writeCommand(p, 0xA8);//--set multiplex ratio(1 to 64)
	if (p->base->p->IICAddress == LCD_SCREEN_BIG)
		_lcd_writeCommand(p, 0x3F);//--1/32 duty
	else _lcd_writeCommand(p, 0x1F);

	_lcd_writeCommand(p, 0xC8);//Com scan direction
	_lcd_writeCommand(p, 0xD3);//-set display offset
	_lcd_writeCommand(p, 0x00);//

	_lcd_writeCommand(p, 0xD5);//set osc division
	_lcd_writeCommand(p, 0x80);//

	_lcd_writeCommand(p, 0xD8);//set area color mode off
	_lcd_writeCommand(p, 0x05);//

	_lcd_writeCommand(p, 0xD9);//Set Pre-Charge Period
	_lcd_writeCommand(p, 0xF1);//

	_lcd_writeCommand(p, 0xDA);//set com pin configuartion
	if (p->base->p->IICAddress == LCD_SCREEN_BIG)
		_lcd_writeCommand(p, 0x12);
	else _lcd_writeCommand(p, 0x02);

	_lcd_writeCommand(p, 0xDB);//set Vcomh
	_lcd_writeCommand(p, 0x30);//

	_lcd_writeCommand(p, 0x8D);//set charge pump enable
	_lcd_writeCommand(p, 0x14);//

	_lcd_writeCommand(p, 0xAF);//--turn on lcd panel

	_lcd_writeCommand(p, 0x21); //Set column address range
	_lcd_writeCommand(p, 0x00);
	_lcd_writeCommand(p, 0x7F); //127

	_lcd_writeCommand(p, 0x22); //Set page address range
	_lcd_writeCommand(p, 0x00);
	if (p->base->p->IICAddress == LCD_SCREEN_BIG)
		_lcd_writeCommand(p, 0x07); //7
	else _lcd_writeCommand(p, 0x03); //3

	_lcd_clear(p);
}

void _lcd_draw(pLCD* p, uint8_t x, uint8_t y, char character) {
	char c = character - ' ';
	if (p->FontSize == FontBig)
	{
		if (x >= 128) { x = 0; y = y + 2; }
		_lcd_setPosition(p, x, y);
		for (uint8_t i = 0; i < 8; i++)
			_lcd_writeData(p, (p->Color == White) ? __FONTS_BIG_[c * 16 + i] : ~__FONTS_BIG_[c * 16 + i]);
		_lcd_setPosition(p, x, y + 1);
		for (uint8_t i = 0; i < 8; i++)
			_lcd_writeData(p, (p->Color == White) ? __FONTS_BIG_[c * 16 + i + 8] : ~__FONTS_BIG_[c * 16 + i + 8]);
	} else {
		if (x >= 128) { x = 0; y = y + 1; }
		_lcd_setPosition(p, x, y);
		for (uint8_t i = 0; i < 6; i++)
			_lcd_writeData(p, (p->Color == White) ? __FONTS_SMALL_[c][i] : ~__FONTS_SMALL_[c][i]);
	}
}

void _lcd_print(pLCD* p, uint8_t x, uint8_t y, char* string) {
	int i = 0;
	if (p->FontSize == FontBig) {
		while (string[i] != '\0') {
			if (string[i] == '\n') {
				i++; x = 0; y += 2;
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 8;
			if (x > 120) { x = 0; y += 2; }
			i++;
		}
	}
	else {
		while (string[i] != '\0') {
			if (string[i] == '\n') {
				i++; x = 0; y++;
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 6;
			if (x > 120) { x = 0; y += 1; }
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
	uint8_t x = (128 - strlen(iobuf) * ((p->FontSize == FontBig) ? 8 : 6)) / 2;
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
	p->base = SPIDeviceInit(pDCPortGroup, pDCPortIndex, pCSPortGroup, pCSPortIndex, pRSTPortGroup, pRSTPortIndex);
	p->BKPortGroup = pBKPortGroup;
	p->BKPortIndex = pBKPortIndex;
	p->Font = FontSmall;
	
	LCD* c = malloc(sizeof(LCD));
	c->p = p;
	c->writeCommand = &_lcd_writeCommand;
	c->writeData = &_lcd_writeData;
	c->setPosition = &_lcd_setPosition;
	c->color = &_lcd_color;
	c->font = &_lcd_font;
	c->clear = &_lcd_clear;
	c->init = &_lcd_init;
	c->draw = &_lcd_draw;
	c->print = &_lcd_print;
	c->printf = &_lcd_printf;
	c->printfc = &_lcd_printfc;
	
	return c;
}


#endif
