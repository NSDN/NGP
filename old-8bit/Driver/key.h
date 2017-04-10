#include "base.h"

#ifndef __KEY_H
#define __KEY_H


#define _KEY_L_COM   P12
#define _KEY_L_UP    P11
#define _KEY_L_DOWN  P14
#define _KEY_L_RIGHT P13

#define _KEY_R_COM   P15
#define _KEY_R_LEFT  P13
#define _LEY_R_RIGHT P10
#define _KEY_R_DOWN  P14

#define KEY_L_UP     0x01
#define KEY_L_DOWN   0x02
#define KEY_L_RIGHT  0x04
#define KEY_R_LEFT   0x10
#define KEY_R_RIGHT  0x20
#define KEY_R_DOWN   0x40

uint8_t getKey();


#endif // !__KEY_H