#include "ge.h"
#include "ae.h"
#include "key.h"
#include "NyaGame.h"

uint8_t menuIndex = 1;
uint8_t funIndex = 0;

bit NGP_Paint_isLoaded = 0;
uint8_t NGP_Paint_x = 0, NGP_Paint_y = 0;

void doSplash()
{
	geDevice(SCREEN_S);
	geString(0,1,"     NGP Loading!\n         NSDN");

	geDevice(SCREEN_P);
	geData(NYAGAME_LOGO);

	aeInit();
	delay(2000);
	aeSpeaker(AE_SPEAKER_L);
	aeSound(1000, 200);
	aeSpeaker(AE_SPEAKER_R);
	aeSound(1000, 200);
	
	geDevice(SCREEN_P);
	geClear(GE_COLOR_BLACK);
	geFlush();

	geDevice(SCREEN_S);
	geClear(GE_COLOR_BLACK);
	geFlush();
}

char i = 0;
char tmp = 0;

void menuBuild()
{
	geDevice(SCREEN_S);
	geString(24, 1, "Copyright NSDN");
	//geString(33, 2, "2014 - 2016");
	
	for (i = 0; i < 8; i++) {
		tmp = ((getKey() << i) & 0x80) ? '1' : '0';
		geChar(i * 6, 2, ' ');
		geChar(i * 6, 2, tmp);
	}	

	geDevice(SCREEN_P);
	geString(24, 0, "NGP 8-bit v1.0");
	geChar(16, menuIndex + 1, '>');
	geString(24, 2, "1. NGP Paint");
	geString(24, 3, "2. NGP Brick");
	geString(24, 4, "3. NGP Tetris");
	geString(24, 5, "4. About NGP");

	if (getKey() & KEY_L_LEFT) {
		while (getKey() & KEY_L_LEFT);
		geChar(16, menuIndex + 1, ' ');
		menuIndex--;
	}
	if (menuIndex == 0) menuIndex = 4;
		
	if (getKey() & KEY_L_RIGHT) {
		while (getKey() & KEY_L_RIGHT);
		geChar(16, menuIndex + 1, ' ');
		menuIndex++;
	}
	if (menuIndex == 5) menuIndex = 1;
	
	if (getKey() & KEY_R_DOWN) {
		while (getKey() & KEY_R_DOWN);
		funIndex = menuIndex;
	}	
}

void NGP_Paint_onLoad()
{
	if (!NGP_Paint_isLoaded) {
		NGP_Paint_isLoaded = 1;
		
		geDevice(SCREEN_S);
		geClear(GE_COLOR_BLACK);
		geString(39, 1, "NGP Paint");
		geString(36, 2, "by drzzm32");
		geFlush();

		geDevice(SCREEN_P);
		geClear(GE_COLOR_BLACK);
		geFlush();
	}
}

void NGP_Paint()
{
	NGP_Paint_onLoad();

	if (getKey() & KEY_L_UP) {
		while (getKey() & KEY_L_UP);
		if (NGP_Paint_y == 0) NGP_Paint_y = 15;
		else NGP_Paint_y--;
	}
	if (getKey() & KEY_L_DOWN) {
		while (getKey() & KEY_L_DOWN);
		if (NGP_Paint_y == 15) NGP_Paint_y = 0;
		else NGP_Paint_y++;
	}
	if (getKey() & KEY_R_LEFT) {
		while (getKey() & KEY_R_LEFT);
		if (NGP_Paint_x == 0) NGP_Paint_y = 31;
		else NGP_Paint_x--;
	}
	if (getKey() & KEY_R_RIGHT) {
		while (getKey() & KEY_R_RIGHT);
		if (NGP_Paint_x == 31) NGP_Paint_y = 0;
		else NGP_Paint_x++;
	}
	if (getKey() & KEY_R_DOWN) {
		while (getKey() & KEY_R_DOWN);
		geColor(GE_COLOR_WHITE);
	}
	if (getKey() & KEY_L_RIGHT) {
		while (getKey() & KEY_L_RIGHT);
		geColor(GE_COLOR_BLACK);	
	}

	gePoint(NGP_Paint_x, NGP_Paint_y);

	geFlush();
}

void main()
{
	geDevice(SCREEN_P);	
	geInit();

	geDevice(SCREEN_S);
	geInit();

	doSplash();

	while (1) {
		switch (funIndex) {
			case 0:
				menuBuild();
				break;
			case 1:
				NGP_Paint();
				break;
			case 2:
				aeSpeaker(AE_SPEAKER_L);
				aeSound(500, 200);
				delay(50);
				aeSpeaker(AE_SPEAKER_R);
				aeSound(500, 200);
				funIndex = 0;
				break;
			case 3:
				aeSpeaker(AE_SPEAKER_L);
				aeSound(500, 200);
				delay(50);
				aeSpeaker(AE_SPEAKER_R);
				aeSound(500, 200);
				funIndex = 0;
				break;
			case 4:
				aeSpeaker(AE_SPEAKER_L);
				aeSound(500, 200);
				delay(50);
				aeSpeaker(AE_SPEAKER_R);
				aeSound(500, 200);
				funIndex = 0;
				break;
			default:
				break;
		} 
	}	
}











