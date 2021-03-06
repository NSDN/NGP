#include <regscreen.h>

const unsigned short st7735[] = {
	0x11, 0, // Sleep Out
	LCD_DELAY, 500,

	0xB1, 3, 0x01, 0x2C, 0x2D, //Frame rate ctrl - normal mode
	0xB2, 3, 0x01, 0x2C, 0x2D, //Frame rate control - idle mode
	0xB3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D, //Frame rate ctrl - partial mode
	0xB4, 1, 0x07, //Column inversion

	0xC0, 3, 0xA2, 0x02, 0x84, //ST7735R Power Sequence
	0xC1, 1, 0xC5,
	0xC2, 2, 0x0A, 0x00,
	0xC3, 2, 0x8A, 0x2A,
	0xC4, 2, 0x8A, 0xEE,
	0xC5, 1, 0x0E, //VCOM
	0x36, 1, 0xC8, //MX, MY, RGB mode

	0xE0, 16, 0x0F, 0x1A, 0x0F, 0x18, 0x2F, 0x28, 0x20, 0x22, 0x1F, 0x1B, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10, //Magical unicorn dust
	0xE1, 16, 0x0F, 0x1B, 0x0F, 0x17, 0x33, 0x2C, 0x29, 0x2E, 0x30, 0x30, 0x39, 0x3F, 0x00, 0x07, 0x03, 0x10, //Sparkles and rainbows
	0x2A, 4, 0x00, 0x00, 0x00, 0x7F, //Column address set
	0x2B, 4, 0x00, 0x00, 0x00, 0x7F, //Row address set
	0xF0, 1, 0x01, //Enable test command
	0xF6, 1, 0x00, //Disable ram power save mode
	0x3A, 1, 0x05, //65k mode

	0x29, 0 // Display ON
};

const unsigned short st7789[] = {
	0x36, 1, 0x00,
	0x3A, 1, 0x05,

	0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33,
	0xB7, 1, 0x35,
	0xBB, 1, 0x19,

	0xC0, 1, 0x2C,
	0xC2, 1, 0x01,
	0xC3, 1, 0x12,
	0xC4, 1, 0x20,
	0xC6, 1, 0x0F,

	0xD0, 2, 0xA4, 0xA1,
	0xE0, 14, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23,
	0xE1, 14, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23,

	0x21, 0,

	0x11, 0, // Sleep Out

	0x29, 0 // Display ON
};

const unsigned short ssd1351[] = {
	SSD1351_CMD_COMMANDLOCK, 1, 0x12,
	SSD1351_CMD_COMMANDLOCK, 1, 0xB1,
	SSD1351_CMD_DISPLAYOFF, 0,
	SSD1351_CMD_CLOCKDIV, 1, 0xF1,
	SSD1351_CMD_MUXRATIO, 1, 127,
	SSD1351_CMD_DISPLAYOFFSET, 1, 0x00,
	SSD1351_CMD_STARTLINE, 1, 0x00,
	SSD1351_CMD_SETREMAP, 1, 0x74,

	SSD1351_CMD_SETCOLUMN, 2, 0x00, 0x7F,
	SSD1351_CMD_SETROW, 2, 0x00, 0x7F,

	SSD1351_CMD_SETGPIO, 1, 0x00,
	SSD1351_CMD_FUNCTIONSELECT, 1, 0x01,

	SSD1351_CMD_SETVSL, 3, 0xA0, 0xB5, 0x55,
	SSD1351_CMD_CONTRASTABC, 3, 0xC8, 0x80, 0xC8,
	SSD1351_CMD_CONTRASTMASTER, 1, 0x0F,

	SSD1351_CMD_SETGRAY, 63,
	0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,
	0xA,  0xB,  0xC,  0xD,  0xE,  0xF,  0x10, 0x11,
	0x12, 0x13, 0x15, 0x17, 0x19, 0x1B, 0x1D, 0x1F,
	0x21, 0x23, 0x25, 0x27, 0x2A, 0x2D, 0x30, 0x33,
	0x36, 0x39, 0x3C, 0x3F, 0x42, 0x45, 0x48, 0x4C,
	0x50, 0x54, 0x58, 0x5C, 0x60, 0x64, 0x68, 0x6C,
	0x70, 0x74, 0x78, 0x7D, 0x82, 0x87, 0x8C, 0x91,
	0x96, 0x9B, 0xA0, 0xA5, 0xAA, 0xAF, 0xB4,

	SSD1351_CMD_PRECHARGE, 1, 0x32,
	SSD1351_CMD_PRECHARGELEVEL, 1, 0x17,
	SSD1351_CMD_DISPLAYENHANCE, 3, 0xA4, 0x00, 0x00,
	SSD1351_CMD_PRECHARGE2, 1, 0x01,

	SSD1351_CMD_VCOMH, 1, 0x05,
	SSD1351_CMD_NORMALDISPLAY, 0,

	SSD1351_CMD_DISPLAYON, 0
};

unsigned short* getRegValues(unsigned char type) {
	switch (type) {
		case ST7735: return (unsigned short*) &st7735;
		case ST7789: return (unsigned short*) &st7789;
		case SSD1351: return (unsigned short*) &ssd1351;
	}
	return 0;
}

unsigned short getRegSize(unsigned char type) {
	switch (type) {
		case ST7735: return sizeof(st7735) / sizeof(unsigned short);
		case ST7789: return sizeof(st7789) / sizeof(unsigned short);
		case SSD1351: return sizeof(ssd1351) / sizeof(unsigned short);
	}
	return 0;
}
