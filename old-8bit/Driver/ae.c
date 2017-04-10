#include "ae.h"

/**********************************************
//Function to initilize audio engine
**********************************************/
void aeInit()
{
	_AE_VOLUME = 127;
	_AE_SPEAKER = AE_SPEAKER_L;
}

/**********************************************
//Function to modify volume
**********************************************/
void aeVolume(uint8_t Volume)
{
	_AE_VOLUME = Volume > _AE_VOLUME_MAX ? _AE_VOLUME_MAX : Volume;
}

/**********************************************
//Function to set current speaker
**********************************************/
void aeSpeaker(uint8_t Speaker)
{
	_AE_SPEAKER = (Speaker == AE_SPEAKER_L || Speaker == AE_SPEAKER_R) ? Speaker : AE_SPEAKER_L;
}

/**********************************************
//Function to play sound
**********************************************/
void aeSound(uint32_t Freq, uint32_t msLength)
{
	if (_AE_SPEAKER == AE_SPEAKER_L) {
		TEMP = 1000000 / Freq;
		while (msLength > 1000 / Freq)
		{
			SPEAKER_L = 1;
			delayMicroseconds(TEMP * _AE_VOLUME / _AE_VOLUME_MAX);
			SPEAKER_L = 0;
			delayMicroseconds(TEMP * (_AE_VOLUME_MAX - _AE_VOLUME) / _AE_VOLUME_MAX);
			msLength = msLength - 1000 / Freq;
		}
	} else {
		TEMP = 1000000 / Freq;
		while (msLength > 1000 / Freq)
		{
			SPEAKER_R = 1;
			delayMicroseconds(TEMP * _AE_VOLUME / _AE_VOLUME_MAX);
			SPEAKER_R = 0;
			delayMicroseconds(TEMP * (_AE_VOLUME_MAX - _AE_VOLUME) / _AE_VOLUME_MAX);
			msLength = msLength - 1000 / Freq;
		}
	}
	
}