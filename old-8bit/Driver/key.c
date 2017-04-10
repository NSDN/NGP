#include "key.h"

/**********************************************
//Function to scan key
**********************************************/
uint8_t getKey()
{
    _KEY_L_COM = 0;
    _KEY_L_UP = 1; _KEY_L_DOWN = 1; _KEY_L_RIGHT = 1;
    
    _KEY_R_COM = 0;
    _KEY_R_LEFT = 1; _KEY_R_RIGHT = 1; _KEY_R_DOWN = 1;
    
    return (_KEY_L_UP == 0 ? KEY_L_UP : 0) | 
           (_KEY_L_DOWN == 0 ? KEY_L_DOWN : 0) | 
           (_KEY_L_RIGHT == 0 ? KEY_L_RIGHT : 0) | 
           (_KEY_R_LEFT == 0 ? KEY_R_LEFT : 0) | 
           (_KEY_R_RIGHT == 0 ? KEY_R_RIGHT : 0) | 
           (_KEY_R_DOWN == 0 ? KEY_L_DOWN : 0); 
}