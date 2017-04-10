#include "base.h"

#ifndef __AE_H
#define __AE_H


#define AE_SPEAKER_L 0x01
#define AE_SPEAKER_R 0x02

#define _AE_VOLUME_MAX 255

static uint8_t _AE_VOLUME = 0;
static uint8_t _AE_SPEAKER = 0;

void aeInit();
void aeVolume(uint8_t Volume);
void aeSpeaker(uint8_t Speaker);
void aeSound(uint32_t Freq, uint32_t msLength);


#endif // !__AE_H
