#ifndef __KEYPAD_H_
#define __KEYPAD_H_


#include "halinc.h"

#define LPAD_UP		0x01
#define LPAD_DOWN	0x02
#define LPAD_LEFT	0x04
#define LPAD_RIGHT	0x08

#define RPAD_UP		0x10
#define RPAD_DOWN	0x20
#define RPAD_LEFT	0x40
#define RPAD_RIGHT	0x80

uint8_t scanKeys();
uint8_t checkKey(uint8_t key);
uint8_t waitKeyUp(uint8_t key);


#endif
