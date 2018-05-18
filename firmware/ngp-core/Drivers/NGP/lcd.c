#include "./Include/lcd.h"
#include "./Include/fonts.h"
#include "./Include/reglcd.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static uint16_t _buf[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

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

void _lcd_writeReg8(pLCD* p, uint8_t cmd, uint32_t data) {
	_lcd_writeCommand(p, cmd);
	_lcd_writeData(p, data);
}

void _lcd_writeReg32(pLCD* p, uint8_t cmd, uint32_t data) {
	_lcd_writeCommand(p, cmd);
	_lcd_writeData(p, (data >> 24) & 0xFF);
	_lcd_writeData(p, (data >> 16) & 0xFF);
	_lcd_writeData(p, (data >> 8 ) & 0xFF);
	_lcd_writeData(p, (data      ) & 0xFF);
}

void _lcd_setPosition(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) { 
	uint32_t t;

    t = (x1 + 2);
    t <<= 16;
    t |= (x2 + 2);
    _lcd_writeReg32(p, LCD_CADDR, t);
    t = (y1 + 3);
    t <<= 16;
    t |= (y2 + 3);
    _lcd_writeReg32(p, LCD_PADDR, t);
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

void _lcd_rotate(pLCD* p, uint8_t r) {
	uint8_t t = 0; uint16_t tmp;
	p->rotate = r;
	switch (r) {
		case LCD_PORTRAIT:
			t = 0;
			break;
		case LCD_LANDSCAPE:
			t = LCD_MADCTL_MV | LCD_MADCTL_HF;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
		case LCD_PORTRAIT_ANTI:
			t = LCD_MADCTL_MY | LCD_MADCTL_HF;
			break;
		case LCD_LANDSCAPE_ANTI:
			t = LCD_MADCTL_MV | LCD_MADCTL_MY ;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
	}
	_lcd_writeReg8(p, LCD_MADCTL, t | LCD_MADCTL_BGR);
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
}

void _lcd_init(pLCD* p) {
	_lcd_reset(p);
	_lcd_backLight(p, 1);
	
	uint32_t i = 0;
	uint16_t r = 0, len = 0, x = 0;
	uint16_t size = sizeof(_regValues) / sizeof(unsigned short);
	while(i < size) {
		r = _regValues[i++];
		len = _regValues[i++];
		if(r == LCD_DELAY) {
			HAL_Delay(len);
		} else {
			_lcd_writeCommand(p, r & 0xFF);
			for (uint16_t d = 0; d < len; d++) {
				x = _regValues[i++];
				_lcd_writeData(p, x & 0xFF);
			}
		}
    }
	
	//_lcd_rotate(p, p->rotate);
}

uint16_t _color_conv(uint32_t color) {
	uint16_t c = ((uint16_t)((double)((color & 0xFF0000) >> 16) * 0.122) << 11) |
				 ((uint16_t)((double)((color & 0x00FF00) >> 8 ) * 0.247) << 5 ) |
				 ((uint16_t)((double)((color & 0x0000FF)      ) * 0.122)      ),
		   tmp = c & 0x00FF;
	return (c >> 8) | (tmp << 8);
}

void _lcd_back_color(pLCD* p, uint32_t color) { p->backColor = _color_conv(color); }

void _lcd_fore_color(pLCD* p, uint32_t color) { p->foreColor = _color_conv(color); }

void _lcd_font(pLCD* p, LCDFont f) { p->Font = f; }

void _lcd_clear(pLCD* p) {
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_flashData16(p, p->backColor, p->width * p->height);
	p->ptrX = p->ptrY = 0;
}

float _lcd_abs(float v) {
	return v > 0 ? v : -v;
}

void _lcd_pixel(pLCD* p, uint16_t x, uint16_t y) {  
	_lcd_setPosition(p, x, y, x, y);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_writeData16(p, p->foreColor);
}

void _lcd_line(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 == x2) {
		float absY = _lcd_abs(y2 - y1), sig = (y2 - y1) / absY;
		for (float dy = 0; _lcd_abs(dy) <= absY; dy += sig) {
			_lcd_setPosition(p, x1, y1 + dy, x1, y1 + dy);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData16(p, p->foreColor);
		}
	} else {
		float k = (float)(y2 - y1) / (float)(x2 - x1), absX = _lcd_abs(x2 - x1);
		float sig = (x2 - x1) / absX;
		for (float dx = 0, dy = 0; _lcd_abs(dx) <= absX; dx += sig, dy += k) {
			for (float dk = 0; _lcd_abs(dk) <= _lcd_abs(k); dk += (k > 0 ? 1 : -1)) {
				_lcd_setPosition(p, x1 + dx, y1 + dy + dk, x1 + dx, y1 + dy + dk);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_writeData16(p, p->foreColor);
			}
		}
	}
}

void _lcd_tri(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill) {
	if (fill) {
		uint16_t maxy = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y1 > y3) ? (y2) : ((y2 > y3) ? y2 : y3));
		uint16_t miny = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y1 < y3) ? (y2) : ((y2 < y3) ? y2 : y3));
		uint16_t midy = (y1 == maxy) ? ((y2 == miny) ? y3 : y2) : ((y1 == miny) ? ((y2 == maxy) ? y3 : y2) : y1);
		uint16_t maxx = (x1 > x2) ? ((x1 > x3) ? x1 : x3) : ((x1 > x3) ? (x2) : ((x2 > x3) ? x2 : x3));
		uint16_t minx = (x1 < x2) ? ((x1 < x3) ? x1 : x3) : ((x1 < x3) ? (x2) : ((x2 < x3) ? x2 : x3));
		uint16_t midx = (x1 == maxx) ? ((x2 == minx) ? x3 : x2) : ((x1 == minx) ? ((x2 == maxx) ? x3 : x2) : x1);
		
		float k1, k2; uint16_t xs, xe, tmp;
		k1 = (float)(maxy - miny) / (float)(maxx - minx);
		
		k2 = (float)(midy - miny) / (float)(midx - minx);
		for (uint16_t i = miny; i <= midy - miny; i++) {
			xs = (float)(i - miny) / k1 + (float)minx;
			xe = (float)(i - miny) / k2 + (float)minx;
			if (xe < xs) { tmp = xe; xe = xs; xs = tmp; }
			_lcd_setPosition(p, xs, i, xe, i);
			_lcd_writeCommand(p, LCD_MEMWR);
			for (uint16_t j = 0; j <= xe - xs; j++) {
				_lcd_writeData16(p, p->foreColor);
			}
		}
		
		k2 = (float)(maxy - midy) / (float)(maxx - midx);
		for (uint16_t i = midy; i <= maxy - midy; i++) {
			xs = (i - miny) / k1 + minx;
			xe = (i - midy) / k2 + midx;
			if (xe < xs) { tmp = xe; xe = xs; xs = tmp; }
			_lcd_setPosition(p, xs, i, xe, i);
			_lcd_writeCommand(p, LCD_MEMWR);
			for (uint16_t j = 0; j <= xe - xs; j++) {
				_lcd_writeData16(p, p->foreColor);
			}
		}
	} else {
		_lcd_line(p, x1, y1, x2, y2);
		_lcd_line(p, x2, y2, x3, y3);
		_lcd_line(p, x3, y3, x1, y1);
	}
}

void _lcd_rect(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill) {
	if (fill) {
		_lcd_setPosition(p, x1, y1, x2, y2);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_flashData16(p, p->foreColor,
		(_lcd_abs((char)x2 - (char)x1) + 1) * (_lcd_abs((char)y2 - (char)y1) + 1));
	} else {
		_lcd_line(p, x1, y1, x2, y1);
		_lcd_line(p, x2, y1, x2, y2);
		_lcd_line(p, x2, y2, x1, y2);
		_lcd_line(p, x1, y2, x1, y1);
	}
}

void _lcd_bitmap(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data) {
	_lcd_setPosition(p, x, y, x + w, y + h);
	_lcd_writeCommand(p, LCD_MEMWR);
	uint32_t c = 0;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			_lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmapc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data) {
	_lcd_setPosition(p, x - w / 2, y - h / 2, x + w / 2, y + h / 2);
	_lcd_writeCommand(p, LCD_MEMWR);
	uint32_t c = 0;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			_lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmapt(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint32_t* data) {
	_lcd_setPosition(p, x, y, x + w, y + h);
	_lcd_writeCommand(p, LCD_MEMWR);
	uint32_t c = 0; uint32_t colort = trans & 0xFFFFFF;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			if (data[c] != colort) _lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmaptc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint32_t* data) {
	_lcd_setPosition(p, x - w / 2, y - h / 2, x + w / 2, y + h / 2);
	_lcd_writeCommand(p, LCD_MEMWR);
	uint32_t c = 0; uint32_t colort = trans & 0xFFFFFF;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			if (data[c] != colort) _lcd_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _lcd_bitmaps(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	uint32_t c = 0;
	for (uint16_t m = 0; m < h; m += 8) {
		for (uint16_t n = 0; n < w; n += 8) {
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++)
					if (data[c] & (1 << j))
						_buf[i + j * 8] = p->foreColor;
					else 
						_buf[i + j * 8] = p->backColor;
				c++;
			}
			_lcd_setPosition(p, x + n, y + m, x + n + 7, y + m + 7);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData16s(p, _buf, 64);
		}
	}
}

void _lcd_bitmapsc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	_lcd_bitmaps(p, x - w / 2, y - h / 2, w, h, data);
}

void _lcd_draw(pLCD* p, uint16_t x, uint16_t y, char character) {
	char c = character - ' ';
	if (p->Font == Big) {
		if (x >= p->width) { x = 0; y = y + 16; }
		for (uint8_t i = 0; i < 8; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				if (getFont(1)[c * 16 + i] & (1 << j))
					_buf[i + j * 8] = p->foreColor;
				else 
					_buf[i + j * 8] = p->backColor;
			}
		}
		for (uint8_t i = 0; i < 8; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				if (getFont(1)[c * 16 + i + 8] & (1 << j))
					_buf[i + j * 8 + 64] = p->foreColor;
				else 
					_buf[i + j * 8 + 64] = p->backColor;
			}
		}
		_lcd_setPosition(p, x, y, x + 7, y + 15);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_writeData16s(p, _buf, 128);
	} else {
		if (x >= p->width) { x = 0; y = y + 8; }
		for (uint8_t i = 0; i < 6; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				if (getFont(0)[c *6 + i] & (1 << j))
					_buf[i + j * 6] = p->foreColor;
				else 
					_buf[i + j * 6] = p->backColor;
			}
		}
		_lcd_setPosition(p, x, y, x + 5, y + 7);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_writeData16s(p, _buf, 48);
	}
}

void _lcd_scroll(pLCD* p, uint16_t pos) {
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_flashData16(p, p->backColor, p->width * p->height);
	if (pos > 60) pos = 60;
	if (p->Font == Big) {
		for (uint16_t i = 0; i < p->height / 16 - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= IOBUF_WIDTH) break;
				_lcd_draw(p, j * 8, i * 16, p->buffer[i][j]);
			}
		}
		memset(p->buffer[p->height / 16 - pos], 0, IOBUF_WIDTH * pos);
	} else {
		for (uint16_t i = 0; i < p->height / 8 - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= IOBUF_WIDTH) break;
				_lcd_draw(p, j * 6, i * 8, p->buffer[i][j]);
			}
		}
		memset(p->buffer[p->height / 8 - pos], 0, IOBUF_WIDTH * pos);
	}
}

void _lcd_printa_(pLCD* p, char* string) {
	int i = 0;
	if (p->Font == Big) {
		while (string[i] != '\0') {
			if (p->ptrY > p->height - 16) {
				p->ptrX = 0;
				p->ptrY = p->height - 16;
				_lcd_scroll(p, 1);
			}
			if (string[i] == '\n') {
				i++;
				p->ptrX = 0;
				p->ptrY += 16;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (p->ptrX >= 8) p->ptrX -= 8;
				else {
					p->ptrX = p->width - 8;
					p->ptrY -= 16;
				}
				_lcd_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_lcd_draw(p, p->ptrX, p->ptrY, string[i]);
			p->buffer[p->ptrY / 16][p->ptrX / 8] = string[i];
			p->ptrX += 8;
			if (p->ptrX > p->width - 8) {
				p->ptrX = 0;
				p->ptrY += 16;
			}
			i++;
		}
		if (string[i] == '\0') p->buffer[p->ptrY / 16][p->ptrX / 8] = string[i];
	} else {
		while (string[i] != '\0') {
			if (p->ptrY > p->height - 8) {
				p->ptrX = 0;
				p->ptrY = p->height - 8;
				_lcd_scroll(p, 1);
			}
			if (string[i] == '\n') {
				i++;
				p->ptrX = 0;
				p->ptrY += 8;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (p->ptrX >= 6) p->ptrX -= 6;
				else {
					p->ptrX = p->width - 6;
					p->ptrY -= 8;
				}
				_lcd_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_lcd_draw(p, p->ptrX, p->ptrY, string[i]);
			p->buffer[p->ptrY / 8][p->ptrX / 6] = string[i];
			p->ptrX += 6;
			if (p->ptrX >= p->width - 6) {
				p->ptrX = 0;
				p->ptrY += 8;
			}
			i++;
		}
		if (string[i] == '\0') p->buffer[p->ptrY / 8][p->ptrX / 6] = string[i];
	}
}

void _lcd_print(pLCD* p, uint16_t x, uint16_t y, char* string) {
	int i = 0;
	if (p->Font == Big) {
		while (string[i] != '\0') {
			if (y > p->height - 16) {
				x = 0;
				y = 0;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++;
				x = 0;
				y += 16;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (x >= 8) x -= 8;
				else {
					x = p->width - 8;
					y -= 16;
				}
				_lcd_draw(p, x, y, ' ');
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 8;
			if (x > p->width - 8) {
				x = 0;
				y += 16;
			}
			i++;
		}
	} else {
		while (string[i] != '\0') {
			if (y > p->height - 8) {
				x = 0;
				y = 0;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++;
				x = 0;
				y += 8;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (x >= 6) x -= 6;
				else {
					x = p->width - 6;
					y -= 8;
				}
				_lcd_draw(p, x, y, ' ');
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 6;
			if (x >= p->width - 6) {
				x = 0;
				y += 8;
			}
			i++;
		}
	}
}

int _lcd_printf(pLCD* p, uint16_t x, uint16_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfc(pLCD* p, uint16_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	uint16_t x = (p->width - strlen(iobuf) * ((p->Font == Big) ? 8 : 6)) / 2;
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfa(pLCD* p, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_lcd_printa_(p, iobuf);
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

	p->width = 128;
	p->height = 128;
	p->Font = Small;
	p->backColor = 0x0000;
	p->foreColor = 0xFFFF;
	p->rotate = LCD_PORTRAIT;
	p->ptrX = p->ptrY = 0;
	memset(p->buffer, 0, IOBUF_WIDTH * IOBUF_HEIGHT);
			
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
	c->scroll = *_lcd_scroll;
	c->rotate = &_lcd_rotate;
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
	c->printfa = &_lcd_printfa;
	
	return c;
}
