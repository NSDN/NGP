#include "./Include/iicdev.h"

void _soft_iic_turn(pSoftIIC* p, uint8_t addr) {
	p->IICAddress = addr;
}

void _soft_iic_start(pSoftIIC* p) {
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_RESET);
}

void _soft_iic_stop(pSoftIIC* p) {
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_SET);
}

void _soft_iic_wait(pSoftIIC* p) {
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_SET);
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_RESET);
}

void _soft_iic_send(pSoftIIC* p, uint8_t byte) {
	uint8_t clip, data;
	data = byte;
	HAL_GPIO_WritePin(p->SCLPortGroup, p->SCLPortIndex, GPIO_PIN_RESET);
	for (uint8_t i = 0; i < 8; i++)
	{
		clip = data;
		clip = clip & 0x80;
		if (clip == 0x80)
			HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(p->SDAPortGroup, p->SDAPortIndex, GPIO_PIN_RESET);
		data = data << 1;
		_soft_iic_wait(p);
	}
}

SoftIIC* SoftIICInit(
		GPIO_TypeDef* pSDAPortGroup, uint16_t pSDAPortIndex,
		GPIO_TypeDef* pSCLPortGroup, uint16_t pSCLPortIndex,
		uint8_t pIICAddress) {
	pSoftIIC* p = malloc(sizeof(pSoftIIC));
	p->SDAPortGroup = pSDAPortGroup;
	p->SDAPortIndex = pSDAPortIndex;
	p->SCLPortGroup = pSCLPortGroup;
	p->SCLPortIndex = pSCLPortIndex;
	p->IICAddress = pIICAddress;

	SoftIIC* c = malloc(sizeof(SoftIIC));
	c->p = p;
	c->turn = &_soft_iic_turn;
	c->start = &_soft_iic_start;
	c->stop = &_soft_iic_stop;
	c->wait = &_soft_iic_wait;
	c->send = &_soft_iic_send;

	return c;
}
