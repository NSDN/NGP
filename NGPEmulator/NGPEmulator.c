#include "NGPEmulator.h"

uint8_t index = 0;

void setup() {
	// put your setup code here, to run once:

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
		lcd->printfc(lcd->p, 24, "Sample 1");
		lcd->printfc(lcd->p, 32, "Flash Test 1");
		lcd->printfc(lcd->p, 40, "Flash Test 2");
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
	}
	else if (index < 20) {
		switch (index) {
		case 10:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "Sample 1");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->printfc(lcd->p, 48, "This is a sample.");
			lcd->printfc(lcd->p, 64, "32's HEX: %X", 32);
			break;
		case 11:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "Flash Test 1");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->tri(lcd->p, 64, 24, 112, 96, 16, 96, 0);
			break;
		case 12:
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "Flash Test 2");
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
		
	}
	else {
		index = 0;
		lcd->clear(lcd->p);
	}

}
