#include "key.h"

/**********************************************
//Function to scan key
**********************************************/
uint8_t getKey()
{
	uint8_t _KEY_TEMP = 0;

	_KEY_L_UL = 0; _KEY_L_RD = 1; _KEY_R_UR = 1; _KEY_R_LD = 1;
	_KEY_COM_H = 1; _KEY_COM_L = 0;
	_KEY_TEMP |= (_KEY_COM_H == 0 ? KEY_L_UP : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 0; _KEY_R_UR = 1; _KEY_R_LD = 1;
	_KEY_COM_H = 1; _KEY_COM_L = 0;
	_KEY_TEMP |= (_KEY_COM_H == 0 ? KEY_L_RIGHT : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 1; _KEY_R_UR = 0; _KEY_R_LD = 1;
	_KEY_COM_H = 1; _KEY_COM_L = 0;
	_KEY_TEMP |= (_KEY_COM_H == 0 ? KEY_R_UP : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 1; _KEY_R_UR = 1; _KEY_R_LD = 0;
	_KEY_COM_H = 1; _KEY_COM_L = 0;
	_KEY_TEMP |= (_KEY_COM_H == 0 ? KEY_R_LEFT : 0);

	_KEY_L_UL = 0; _KEY_L_RD = 1; _KEY_R_UR = 1; _KEY_R_LD = 1;
	_KEY_COM_H = 0; _KEY_COM_L = 1;
	_KEY_TEMP |= (_KEY_COM_L == 0 ? KEY_L_LEFT : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 0; _KEY_R_UR = 1; _KEY_R_LD = 1;
	_KEY_COM_H = 0; _KEY_COM_L = 1;
	_KEY_TEMP |= (_KEY_COM_L == 0 ? KEY_L_DOWN : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 1; _KEY_R_UR = 0; _KEY_R_LD = 1;
	_KEY_COM_H = 0; _KEY_COM_L = 1;
	_KEY_TEMP |= (_KEY_COM_L == 0 ? KEY_R_RIGHT : 0);

	_KEY_L_UL = 1; _KEY_L_RD = 1; _KEY_R_UR = 1; _KEY_R_LD = 0;
	_KEY_COM_H = 0; _KEY_COM_L = 1;
	_KEY_TEMP |= (_KEY_COM_L == 0 ? KEY_R_DOWN : 0);
    
   	return _KEY_TEMP; 
}