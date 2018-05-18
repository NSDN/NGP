#include "NGPEmulator.h"

#pragma region STG
#define STG_BACK 1
#define STG_MAIN 2
#define STG_COLORB 0x000000

#define RANGE_X1 4
#define RANGE_Y1 4
#define RANGE_X2 92
#define RANGE_Y2 123

void onInitPlayer();
void onUpdatePlayer();
struct Player {
	uint32_t color;
	uint8_t size;
	float x;
	float y;
	float prevX;
	float prevY;
	void (*onInit)();
	void (*onUpdate)();
} player = { 0xFF9800, 1,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	&onInitPlayer,
	&onUpdatePlayer
};
void onInitPlayer() {
	player.x = (RANGE_X2 + RANGE_X1) / 2;
	player.y = 108;
}
void onUpdatePlayer() {
	player.prevX = player.x; player.prevY = player.y;
	if (checkKey(LPAD_LEFT)) { player.x -= 0.02; }
	if (checkKey(LPAD_RIGHT)) { player.x += 0.02; }
	if (checkKey(LPAD_UP)) { player.y -= 0.02; }
	if (checkKey(LPAD_DOWN)) { player.y += 0.02; }

	if (player.x > RANGE_X2 - player.size) player.x = RANGE_X2 - player.size;
	if (player.x < RANGE_X1 + player.size) player.x = RANGE_X1 + player.size;
	if (player.y > RANGE_Y2 - player.size) player.y = RANGE_Y2 - player.size;
	if (player.y < RANGE_Y1 + player.size) player.y = RANGE_Y1 + player.size;

	if ((char) player.x != (char) player.prevX || (char) player.y != (char) player.prevY) {
		lcd->colorf(lcd->p, STG_COLORB);
		lcd->rect(lcd->p,
			player.prevX - player.size, player.prevY - player.size,
			player.prevX + player.size, player.prevY + player.size,
			1
		);
	}
	lcd->colorf(lcd->p, player.color);
	lcd->rect(lcd->p,
		player.x - player.size, player.y - player.size,
		player.x + player.size, player.y + player.size,
		1
	);
}


void STG(uint8_t type) {
	if (type == STG_BACK) {
		lcd->colorf(lcd->p, 0xFFFFFF);
		lcd->rect(lcd->p, RANGE_X1 - 4, RANGE_Y1 - 4, RANGE_X2 + 4, RANGE_Y2 + 4, 1);
		lcd->colorf(lcd->p, STG_COLORB);
		lcd->rect(lcd->p, RANGE_X1 - 2, RANGE_Y1 - 2, RANGE_X2 + 2, RANGE_Y2 + 2, 1);
		lcd->colorf(lcd->p, 0xFFFFFF);
		lcd->rect(lcd->p, RANGE_X1 - 1, RANGE_Y1 - 1, RANGE_X2 + 1, RANGE_Y2 + 1, 0);
		lcd->colorf(lcd->p, 0x7986CB);
		lcd->font(lcd->p, Big);
		lcd->draw(lcd->p, RANGE_X2 + 12, RANGE_Y1 + 4, 'T');
		lcd->draw(lcd->p, RANGE_X2 + 12 + 8, RANGE_Y1 + 4, 'H');
		lcd->font(lcd->p, Small);
		lcd->colorf(lcd->p, 0xFFFFFF);
		
		player.onInit();
	} else if (type == STG_MAIN) {
		player.onUpdate();
	}
}
#pragma endregion


uint8_t index;

const unsigned char test[] = {
	/* 0xB2BF [²¿]   [0]*/
	0x00,0x40,0x40,0x40,0x40,0x40,0x42,0x46,0x7E,0x7E,0x7C,0xD8,0xC0,0xE0,0x70,0x70,0x60,0x40,0x00,0xF8,0xF8,0xF0,0xF0,0x10,0x10,0x10,0xF8,0xF8,0xF8,0x70,0x30,0x00,
	0x80,0x80,0x80,0x81,0x83,0xBE,0xBE,0xBE,0x98,0x80,0xF0,0xBF,0x9F,0x8F,0xC3,0xE2,0xE0,0xC0,0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x70,0x9F,0x87,0x03,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0xFC,0xFC,0xF8,0x18,0x10,0x10,0x10,0x10,0x0C,0xFC,0xFC,0xF8,0x18,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x83,0xFF,0xFE,0xFC,0xF0,0x00,
	0x00,0x00,0x00,0x7F,0x7F,0x7F,0x08,0x08,0x08,0x08,0x08,0x08,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x7F,0x01,0x01,0x07,0x0F,0x0F,0x07,0x07,0x03,0x00,
};

void setup() {
	// put your setup code here, to run once:

	index = 0;

	lcd->init(lcd->p);
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);
	lcd->bitmapsc(lcd->p, 63, 63, 64, 64, __NYAGAME_LOGO_);
	delay(2000);
	lcd->colorb(lcd->p, 0x000000);
	lcd->colorf(lcd->p, 0xFFFFFF);
	lcd->clear(lcd->p);
}

void loop() {
	// put your main code here, to run repeatedly:

	if (index < 10) {
		/* MENU BUILD BEGIN */
		lcd->colorb(lcd->p, 0x000000);
		lcd->colorf(lcd->p, 0xFF9800);
		lcd->printfc(lcd->p, 8, "MENU");
		lcd->colorb(lcd->p, 0x000000);
		lcd->colorf(lcd->p, 0xFFFFFF);
		lcd->printfc(lcd->p, 24, "Sample");
		lcd->printfc(lcd->p, 32, "STG Sample");
		lcd->printfc(lcd->p, 40, "Flash Test");
		lcd->printfc(lcd->p, 48, "NSDN-Beeper");
		lcd->printfc(lcd->p, 56, "About this NGP");

		lcd->draw(lcd->p, 10, (index + 2) * 8, ' ');
		lcd->draw(lcd->p, 10, (index + 3) * 8, '>');
		lcd->draw(lcd->p, 10, (index + 4) * 8, ' ');
		lcd->draw(lcd->p, 112, (index + 2) * 8, ' ');
		lcd->draw(lcd->p, 112, (index + 3) * 8, '<');
		lcd->draw(lcd->p, 112, (index + 4) * 8, ' ');

		if (waitKeyUp(LPAD_UP)) index = ((index > 0) ? index - 1 : 0);
		if (waitKeyUp(LPAD_DOWN)) index = ((index < 4) ? index + 1 : 4);
		if (waitKeyUp(RPAD_UP)) {
			index += 10;
			lcd->clear(lcd->p);

			switch (index) {
			case 10:
				break;
			case 11:
				STG(STG_BACK);
				break;
			case 12:
				break;
			case 13:
				break;
			case 14:
				lcd->colorb(lcd->p, 0xFFFFFF);
				lcd->colorf(lcd->p, 0x000000);
				lcd->clear(lcd->p);
				break;
			default:
				break;
			}
		}
	} else if (index < 20) {
		switch (index) {
		case 10:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "Sample");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->line(lcd->p, 16, 24, 112, 96);
			lcd->line(lcd->p, 16, 96, 112, 24);
			break;
		case 11:
			STG(STG_MAIN);
			break;
		case 12:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "Flash Test");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0x3F51B5);
			lcd->printfc(lcd->p, 24, "Flashing: 0x3F51B5");
			lcd->rect(lcd->p, 16, 48, 112, 112, 1);
			lcd->colorf(lcd->p, 0xB10000);
			lcd->printfc(lcd->p, 24, "Flashing: 0xB10000");
			lcd->rect(lcd->p, 16, 48, 112, 112, 1);
			lcd->colorf(lcd->p, 0xFFFFFF);
			break;
		case 13:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "NSDN-Beeper");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->printfc(lcd->p, 32, "NOW Playing...");
			lcd->printfc(lcd->p, 48, "Remilia");
			lcd->printfc(lcd->p, 56, "Scarlet");
			break;
		case 14:
			lcd->colorb(lcd->p, 0xFFFFFF);
			lcd->colorf(lcd->p, 0x000000);
			lcd->bitmapsc(lcd->p, 63, 48, 64, 64, __NYAGAME_LOGO_);
			lcd->printfc(lcd->p, 86, "NyaGame Portable");
			lcd->printfc(lcd->p, 98, "dev161206");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			break;
		default:
			break;
		}

		if (waitKeyUp(RPAD_RIGHT)) {
			if (index == 11) {
				if (!checkKey(LPAD_LEFT)) {
					return;
				}
			}
			index -= 10;
			lcd->clear(lcd->p);

			switch (index) {
			case 10:
				break;
			case 11:
				break;
			case 12:
				break;
			case 13:
				break;
			case 14:
				break;
			default:
				break;
			}
		}
		
	} else {
		index = 0;
		lcd->clear(lcd->p);
	}

}
