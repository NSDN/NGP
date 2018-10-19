#ifndef __IICDEV_H_
#define __IICDEV_H_


#include "halinc.h"

typedef struct {
	GPIO_TypeDef* SDAPortGroup;
	uint16_t SDAPortIndex;
	GPIO_TypeDef* SCLPortGroup;
	uint16_t SCLPortIndex;
	uint8_t IICAddress;
} pSoftIIC;

typedef struct {
	pSoftIIC* p;
	void (*turn)(pSoftIIC* p, uint8_t addr);
	void (*start)(pSoftIIC* p);
	void (*stop)(pSoftIIC* p);
	void (*wait)(pSoftIIC* p);
	void (*send)(pSoftIIC* p, uint8_t byte);
} SoftIIC;

SoftIIC* SoftIICInit(
		GPIO_TypeDef* pSDAPortGroup, uint16_t pSDAPortIndex,
		GPIO_TypeDef* pSCLPortGroup, uint16_t pSCLPortIndex,
		uint8_t pIICAddress);


#endif
