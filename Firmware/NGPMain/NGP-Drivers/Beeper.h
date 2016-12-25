#include "main.h"
#include "stm32f4xx_hal.h"
#include "Music.h"
#include "Symbol.h"

#ifndef __BEEPER_H_
#define __BEEPER_H_


#define OUT_A LBEEP_GPIO_Port,LBEEP_Pin
#define OUT_B RBEEP_GPIO_Port,RBEEP_Pin

static short OUT_A_COUNT = 0;
static short OUT_B_COUNT = 0;

static short OUT_A_COUNT_POS = 0;
static short OUT_B_COUNT_POS = 0;

static short OUT_A_COUNT_SUM = 0;
static short OUT_B_COUNT_SUM = 0;

void beeperInerrupt() {
	if (OUT_A_COUNT <= OUT_A_COUNT_POS) HAL_GPIO_WritePin(OUT_A, GPIO_PIN_SET);
	else if (OUT_A_COUNT <= OUT_A_COUNT_SUM) HAL_GPIO_WritePin(OUT_A, GPIO_PIN_RESET);
	else OUT_A_COUNT = 0; 
	OUT_A_COUNT += 1;
	
	if (OUT_B_COUNT <= OUT_B_COUNT_POS) HAL_GPIO_WritePin(OUT_B, GPIO_PIN_SET);
	else if (OUT_B_COUNT <= OUT_B_COUNT_SUM) HAL_GPIO_WritePin(OUT_B, GPIO_PIN_RESET);
	else OUT_B_COUNT = 0;
	OUT_B_COUNT += 1;
}

static uint8_t mode = 1;
static uint8_t state = 1;
static uint8_t jump = 0;
void jumpOut() { jump = 1; }

static uint8_t trans = 0;
void trMode() { trans = 1; }

void setVolume(unsigned char vol) {
	OUT_A_COUNT_POS = OUT_B_COUNT_POS = vol > 5 ? 5 : vol;
}

void playMusic(const unsigned short* symbol, const char* music, unsigned short length, unsigned short patternLength, uint8_t withControl) {
	unsigned short i;
	char step;
	for (i = 0; i < length; i++) {
		while (!state && withControl);
		if ((jump && withControl) || trans) { 
			jump = 0; trans = 0;
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
			return; 
		}
		step = music[i];
		if (step != 0xFF) {
			OUT_A_COUNT_SUM = symbol[step];
			OUT_B_COUNT_SUM = symbol[step];
		} else {
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
		}
		HAL_Delay(patternLength);
	}
}

void playMusicWithSpace(const unsigned short* symbol, const char* music, unsigned short length, unsigned short patternLength, unsigned short spaceLength, uint8_t withControl) {
	unsigned short i;
	char step;
	for (i = 0; i < length; i++) {
		while (!state && withControl);
		if ((jump && withControl) || trans) { 
			jump = 0; trans = 0;
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
			return; 
		}
		step = music[i];
		if (step != 0xFF) {
			OUT_A_COUNT_SUM = symbol[step];
			OUT_B_COUNT_SUM = symbol[step];
		} else {
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
		}
		HAL_Delay(patternLength);
		OUT_A_COUNT_SUM = 0;
		OUT_B_COUNT_SUM = 0;
		HAL_Delay(spaceLength);		
	}
}


#endif
