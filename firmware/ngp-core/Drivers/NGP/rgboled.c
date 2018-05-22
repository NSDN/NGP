#include <regscreen.h>
#include "./Include/rgboled.h"
#include "./Include/fonts.h"
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

void _rgb_writeCommand(pRGBOLED* p, uint8_t cmd) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 0);
	p->base->send(p->base->p, cmd);
	p->base->stop(p->base->p);
}

void _rgb_writeData(pRGBOLED* p, uint8_t data) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->send(p->base->p, data);
	p->base->stop(p->base->p);
}

void _rgb_writeData16(pRGBOLED* p, uint16_t data) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->write(p->base->p, &data, 2);
	p->base->stop(p->base->p);
}

void _rgb_writeData16s(pRGBOLED* p, uint16_t* data, uint32_t length) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->write(p->base->p, data, length * 2);
	p->base->stop(p->base->p);
}

void _rgb_flashData16(pRGBOLED* p, uint16_t data, uint32_t count) {
	p->base->start(p->base->p);
	p->base->dcs(p->base->p, 1);
	p->base->writes(p->base->p, data, count);
	p->base->stop(p->base->p);
}

void _rgb_writeReg8(pRGBOLED* p, uint8_t cmd, uint32_t data) {
	_rgb_writeCommand(p, cmd);
	_rgb_writeData(p, data);
}

void _rgb_writeReg16(pRGBOLED* p, uint8_t cmd, uint16_t data) {
	_rgb_writeCommand(p, cmd);
	_rgb_writeData(p, (data >> 8 ) & 0xFF);
	_rgb_writeData(p, (data      ) & 0xFF);
}

void _rgb_setPosition(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;

    t = (x1 & 0x7F);
    t <<= 8;
    t |= (x2 & 0x7F);
    _rgb_writeReg16(p, SSD1351_CMD_SETCOLUMN, t);
    t = (y1 & 0x7F);
    t <<= 8;
    t |= (y2 & 0x7F);
    _rgb_writeReg16(p, SSD1351_CMD_SETROW, t);
}

void _rgb_reset(pRGBOLED* p) {
	HAL_GPIO_WritePin(p->RSTPortGroup, p->RSTPortIndex, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(p->RSTPortGroup, p->RSTPortIndex, GPIO_PIN_SET);
	HAL_Delay(500);
}

void _rgb_rotate(pRGBOLED* p, uint8_t r) {
	uint8_t t = 0; uint16_t tmp;
	p->rotate = r;
	switch (r) {
		case RGB_PORTRAIT:
			t = 0;
			break;
		case RGB_LANDSCAPE:
			t = 0;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
		case RGB_PORTRAIT_ANTI:
			t = 0;
			break;
		case RGB_LANDSCAPE_ANTI:
			t = 0;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
	}
	t |= SSD1351_REMAP_CR | SSD1351_REMAP_SD | SSD1351_REMAP_OE | SSD1351_COLOR_16;
	_rgb_writeReg8(p, SSD1351_CMD_SETREMAP, t);
	_rgb_setPosition(p, 0, 0, p->width - 1, p->height - 1);
}

void _rgb_init(pRGBOLED* p) {
	_rgb_reset(p);
	
	uint32_t i = 0;
	uint16_t r = 0, len = 0, x = 0;
	uint16_t size = getRegSize(SSD1351);
	uint16_t* _regValues = getRegValues(SSD1351);
	while(i < size) {
		r = _regValues[i++];
		len = _regValues[i++];
		if(r == LCD_DELAY) {
			HAL_Delay(len);
		} else {
			_rgb_writeCommand(p, r & 0xFF);
			for (uint16_t d = 0; d < len; d++) {
				x = _regValues[i++];
				_rgb_writeData(p, x & 0xFF);
			}
		}
    }
	
	_rgb_rotate(p, p->rotate);
}

uint16_t _rgb_color_conv(uint32_t color) {
	uint16_t c;
	c =  (((color & 0xFF0000) >> 16) & 0xFF) >> 3;
	c <<= 6;
	c |= (((color & 0x00FF00) >> 8 ) & 0xFF) >> 2;
	c <<= 5;
	c |= (((color & 0x0000FF)      ) & 0xFF) >> 3;
	return c << 8 | c >> 8;
}

void _rgb_back_color(pRGBOLED* p, uint32_t color) { p->backColor = _rgb_color_conv(color); }

void _rgb_fore_color(pRGBOLED* p, uint32_t color) { p->foreColor = _rgb_color_conv(color); }

void _rgb_font(pRGBOLED* p, RGBLEDFont f) { p->Font = f; }

void _rgb_clear(pRGBOLED* p) {
	_rgb_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	_rgb_flashData16(p, p->backColor, p->width * p->height);
	p->ptrX = p->ptrY = 0;
}

float _rgb_abs(float v) {
	return v > 0 ? v : -v;
}

void _rgb_pixel(pRGBOLED* p, uint16_t x, uint16_t y) {
	_rgb_setPosition(p, x, y, x, y);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	_rgb_writeData16(p, p->foreColor);
}

void _rgb_line(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 == x2) {
		float absY = _rgb_abs(y2 - y1), sig = (y2 - y1) / absY;
		for (float dy = 0; _rgb_abs(dy) <= absY; dy += sig) {
			_rgb_setPosition(p, x1, y1 + dy, x1, y1 + dy);
			_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
			_rgb_writeData16(p, p->foreColor);
		}
	} else {
		float k = (float)(y2 - y1) / (float)(x2 - x1), absX = _rgb_abs(x2 - x1);
		float sig = (x2 - x1) / absX;
		for (float dx = 0, dy = 0; _rgb_abs(dx) <= absX; dx += sig, dy += k) {
			for (float dk = 0; _rgb_abs(dk) <= _rgb_abs(k); dk += (k > 0 ? 1 : -1)) {
				_rgb_setPosition(p, x1 + dx, y1 + dy + dk, x1 + dx, y1 + dy + dk);
				_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
				_rgb_writeData16(p, p->foreColor);
			}
		}
	}
}

void _rgb_tri(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill) {
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
			_rgb_setPosition(p, xs, i, xe, i);
			_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
			for (uint16_t j = 0; j <= xe - xs; j++) {
				_rgb_writeData16(p, p->foreColor);
			}
		}
		
		k2 = (float)(maxy - midy) / (float)(maxx - midx);
		for (uint16_t i = midy; i <= maxy - midy; i++) {
			xs = (i - miny) / k1 + minx;
			xe = (i - midy) / k2 + midx;
			if (xe < xs) { tmp = xe; xe = xs; xs = tmp; }
			_rgb_setPosition(p, xs, i, xe, i);
			_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
			for (uint16_t j = 0; j <= xe - xs; j++) {
				_rgb_writeData16(p, p->foreColor);
			}
		}
	} else {
		_rgb_line(p, x1, y1, x2, y2);
		_rgb_line(p, x2, y2, x3, y3);
		_rgb_line(p, x3, y3, x1, y1);
	}
}

void _rgb_rect(pRGBOLED* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill) {
	if (fill) {
		_rgb_setPosition(p, x1, y1, x2, y2);
		_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
		_rgb_flashData16(p, p->foreColor,
		(_rgb_abs((char)x2 - (char)x1) + 1) * (_rgb_abs((char)y2 - (char)y1) + 1));
	} else {
		_rgb_line(p, x1, y1, x2, y1);
		_rgb_line(p, x2, y1, x2, y2);
		_rgb_line(p, x2, y2, x1, y2);
		_rgb_line(p, x1, y2, x1, y1);
	}
}

void _rgb_bitmap(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data) {
	_rgb_setPosition(p, x, y, x + w - 1, y + h - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	uint32_t c = 0;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			_rgb_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _rgb_bitmapc(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data) {
	_rgb_setPosition(p, x - w / 2, y - h / 2, x + w / 2 - 1, y + h / 2 - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	uint32_t c = 0;
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			_rgb_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _rgb_bitmapt(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint16_t* data) {
	_rgb_setPosition(p, x, y, x + w - 1, y + h - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	uint32_t c = 0; uint16_t colort = _rgb_color_conv(trans & 0xFFFFFF);
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			if (data[c] != colort) _rgb_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _rgb_bitmaptc(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint16_t* data) {
	_rgb_setPosition(p, x - w / 2, y - h / 2, x + w / 2 - 1, y + h / 2 - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	uint32_t c = 0; uint16_t colort = _rgb_color_conv(trans & 0xFFFFFF);
	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			if (data[c] != colort) _rgb_writeData16(p, data[c]);
			c += 1;
		}
	}
}

void _rgb_bitmaps(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
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
			_rgb_setPosition(p, x + n, y + m, x + n + 7, y + m + 7);
			_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
			_rgb_writeData16s(p, _buf, 64);
		}
	}
}

void _rgb_bitmapsc(pRGBOLED* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	_rgb_bitmaps(p, x - w / 2, y - h / 2, w, h, data);
}

void _rgb_draw(pRGBOLED* p, uint16_t x, uint16_t y, char character) {
	char c = character - ' ';
	if (p->Font == RGBBig) {
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
		_rgb_setPosition(p, x, y, x + 7, y + 15);
		_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
		_rgb_writeData16s(p, _buf, 128);
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
		_rgb_setPosition(p, x, y, x + 5, y + 7);
		_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
		_rgb_writeData16s(p, _buf, 48);
	}
}

void _rgb_scroll(pRGBOLED* p, uint16_t pos) {
	_rgb_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_rgb_writeCommand(p, SSD1351_CMD_WRITERAM);
	_rgb_flashData16(p, p->backColor, p->width * p->height);
	if (pos > 60) pos = 60;
	if (p->Font == RGBBig) {
		for (uint16_t i = 0; i < p->height / 16 - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], RGB_IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= RGB_IOBUF_WIDTH) break;
				_rgb_draw(p, j * 8, i * 16, p->buffer[i][j]);
			}
		}
		memset(p->buffer[p->height / 16 - pos], 0, RGB_IOBUF_WIDTH * pos);
	} else {
		for (uint16_t i = 0; i < p->height / 8 - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], RGB_IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= RGB_IOBUF_WIDTH) break;
				_rgb_draw(p, j * 6, i * 8, p->buffer[i][j]);
			}
		}
		memset(p->buffer[p->height / 8 - pos], 0, RGB_IOBUF_WIDTH * pos);
	}
}

void _rgb_printa_(pRGBOLED* p, char* string) {
	int i = 0;
	if (p->Font == RGBBig) {
		while (string[i] != '\0') {
			if (p->ptrY > p->height - 16) {
				p->ptrX = 0;
				p->ptrY = p->height - 16;
				_rgb_scroll(p, 1);
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
				_rgb_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_rgb_draw(p, p->ptrX, p->ptrY, string[i]);
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
				_rgb_scroll(p, 1);
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
				_rgb_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_rgb_draw(p, p->ptrX, p->ptrY, string[i]);
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

void _rgb_print(pRGBOLED* p, uint16_t x, uint16_t y, char* string) {
	int i = 0;
	if (p->Font == RGBBig) {
		while (string[i] != '\0') {
			if (y > p->height - 16) {
				x = 0;
				y = 0;
				_rgb_clear(p);
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
				_rgb_draw(p, x, y, ' ');
				continue;
			}
			_rgb_draw(p, x, y, string[i]);
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
				_rgb_clear(p);
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
				_rgb_draw(p, x, y, ' ');
				continue;
			}
			_rgb_draw(p, x, y, string[i]);
			x += 6;
			if (x >= p->width - 6) {
				x = 0;
				y += 8;
			}
			i++;
		}
	}
}

int _rgb_printf(pRGBOLED* p, uint16_t x, uint16_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * RGB_IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_rgb_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _rgb_printfc(pRGBOLED* p, uint16_t y, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * RGB_IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	uint16_t x = (p->width - strlen(iobuf) * ((p->Font == RGBBig) ? 8 : 6)) / 2;
	_rgb_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _rgb_printfa(pRGBOLED* p, const char* format, ...) {
	char* iobuf = malloc(sizeof(char) * RGB_IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_rgb_printa_(p, iobuf);
	return result;
}

RGBOLED* RGBOLEDInit(
		SPI_HandleTypeDef* pspi,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex,
		GPIO_TypeDef* pRSTPortGroup, uint16_t pRSTPortIndex) {
	pRGBOLED* p = malloc(sizeof(pRGBOLED));
	p->base = SPIDeviceInit(pspi, pDCPortGroup, pDCPortIndex, pCSPortGroup, pCSPortIndex);
	p->RSTPortGroup = pRSTPortGroup;
	p->RSTPortIndex = pRSTPortIndex;

	p->width = 128;
	p->height = 128;
	p->Font = RGBSmall;
	p->backColor = 0x0000;
	p->foreColor = 0xFFFF;
	p->rotate = RGB_PORTRAIT;
	p->ptrX = p->ptrY = 0;
	memset(p->buffer, 0, RGB_IOBUF_WIDTH * RGB_IOBUF_HEIGHT);
			
	RGBOLED* c = malloc(sizeof(RGBOLED));
	c->p = p;
	#ifdef RGB_USE_PRIVATE_FUN
	c->writeCommand = &_rgb_writeCommand;
	c->writeData = &_rgb_writeData;
	c->writeData16 = &_rgb_writeData16;
	c->setPosition = &_rgb_setPosition;
	#endif
	c->init = &_rgb_init;
	c->reset = &_rgb_reset;
	c->colorb = &_rgb_back_color;
	c->colorf = &_rgb_fore_color;
	c->font = &_rgb_font;
	c->clear = &_rgb_clear;
	c->scroll = *_rgb_scroll;
	c->rotate = &_rgb_rotate;
	c->pixel = &_rgb_pixel;
	c->line = &_rgb_line;
	c->tri = &_rgb_tri;
	c->rect = &_rgb_rect;
	c->bitmap = &_rgb_bitmap;
	c->bitmapc = &_rgb_bitmapc;
	c->bitmapt = &_rgb_bitmapt;
	c->bitmaptc = &_rgb_bitmaptc;
	c->bitmaps = &_rgb_bitmaps;
	c->bitmapsc = &_rgb_bitmapsc;
	c->draw = &_rgb_draw;
	c->print = &_rgb_print;
	c->printf = &_rgb_printf;
	c->printfc = &_rgb_printfc;
	c->printfa = &_rgb_printfa;
	
	return c;
}

RGBOLED* SoftRGBInit(
		GPIO_TypeDef* pSDAPortGroup, uint16_t pSDAPortIndex,
		GPIO_TypeDef* pSCLPortGroup, uint16_t pSCLPortIndex,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex,
		GPIO_TypeDef* pRSTPortGroup, uint16_t pRSTPortIndex) {
	pRGBOLED* p = malloc(sizeof(pRGBOLED));
	p->base = SoftSPIInit(
			pSDAPortGroup, pSDAPortIndex,
			pSCLPortGroup, pSCLPortIndex,
			pDCPortGroup, pDCPortIndex,
			pCSPortGroup, pCSPortIndex);
	p->RSTPortGroup = pRSTPortGroup;
	p->RSTPortIndex = pRSTPortIndex;

	p->width = 128;
	p->height = 128;
	p->Font = RGBSmall;
	p->backColor = 0x0000;
	p->foreColor = 0xFFFF;
	p->rotate = RGB_PORTRAIT;
	p->ptrX = p->ptrY = 0;
	memset(p->buffer, 0, RGB_IOBUF_WIDTH * RGB_IOBUF_HEIGHT);

	RGBOLED* c = malloc(sizeof(RGBOLED));
	c->p = p;
	#ifdef RGB_USE_PRIVATE_FUN
	c->writeCommand = &_rgb_writeCommand;
	c->writeData = &_rgb_writeData;
	c->writeData16 = &_rgb_writeData16;
	c->setPosition = &_rgb_setPosition;
	#endif
	c->init = &_rgb_init;
	c->reset = &_rgb_reset;
	c->colorb = &_rgb_back_color;
	c->colorf = &_rgb_fore_color;
	c->font = &_rgb_font;
	c->clear = &_rgb_clear;
	c->scroll = *_rgb_scroll;
	c->rotate = &_rgb_rotate;
	c->pixel = &_rgb_pixel;
	c->line = &_rgb_line;
	c->tri = &_rgb_tri;
	c->rect = &_rgb_rect;
	c->bitmap = &_rgb_bitmap;
	c->bitmapc = &_rgb_bitmapc;
	c->bitmapt = &_rgb_bitmapt;
	c->bitmaptc = &_rgb_bitmaptc;
	c->bitmaps = &_rgb_bitmaps;
	c->bitmapsc = &_rgb_bitmapsc;
	c->draw = &_rgb_draw;
	c->print = &_rgb_print;
	c->printf = &_rgb_printf;
	c->printfc = &_rgb_printfc;
	c->printfa = &_rgb_printfa;

	return c;
}
