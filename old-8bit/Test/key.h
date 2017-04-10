#include "base.h"

#ifndef __KEY_H
#define __KEY_H


#define _KEY_COM_H	P13
#define _KEY_COM_L	P14

#define _KEY_L_UL	P11
#define _KEY_L_RD	P12

#define _KEY_R_UR	P10
#define _KEY_R_LD	P15

#define KEY_L_UP	0x01
#define KEY_L_DOWN	0x02
#define KEY_L_LEFT	0x04
#define KEY_L_RIGHT	0x08
#define KEY_R_UP	0x10
#define KEY_R_DOWN	0x20
#define KEY_R_LEFT	0x40
#define KEY_R_RIGHT	0x80

uint8_t getKey();


#endif // !__KEY_H