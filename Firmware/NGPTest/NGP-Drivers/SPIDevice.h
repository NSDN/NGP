#include <stdlib.h>

#include "stm32f1xx_hal.h"

#ifndef __SPIDEVICE_H_
#define __SPIDEVICE_H_


typedef struct {
	SPI_HandleTypeDef* spi;
	GPIO_TypeDef* DCPortGroup;
	uint16_t DCPortIndex;
	GPIO_TypeDef* CSPortGroup;
	uint16_t CSPortIndex;
} pSPIDevice;

typedef struct {
	pSPIDevice* p;
	void (*start)(pSPIDevice* p);
	void (*stop)(pSPIDevice* p);
	void (*dcs)(pSPIDevice* p, uint8_t data);
	void (*send)(pSPIDevice* p, uint8_t byte);
	uint8_t (*read)(pSPIDevice* p);
} SPIDevice;

void _spi_dev_start(pSPIDevice* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_RESET);
}

void _spi_dev_dcs(pSPIDevice* p, uint8_t data) {
	HAL_GPIO_WritePin(p->DCPortGroup, p->DCPortIndex, (data > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void _spi_dev_send(pSPIDevice* p, uint8_t byte) {
	//HAL_SPI_Transmit_DMA(p->spi, &byte, 1);
	HAL_SPI_Transmit(p->spi, &byte, 1, 1);
}

uint8_t _spi_dev_read(pSPIDevice* p) {
	uint8_t byte = 0;
	//HAL_SPI_Receive_DMA(p->spi, &byte, 1);
	HAL_SPI_Receive(p->spi, &byte, 1, 1);
	return byte;
}

void _spi_dev_stop(pSPIDevice* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_SET);
}

SPIDevice* SPIDeviceInit(
		SPI_HandleTypeDef* pspi, 
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex, 
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex) {
	pSPIDevice* p = malloc(sizeof(pSPIDevice));
	p->spi = pspi;
	p->DCPortGroup = pDCPortGroup;
	p->DCPortIndex = pDCPortIndex;
	p->CSPortGroup = pCSPortGroup;
	p->CSPortIndex = pCSPortIndex;
	
	SPIDevice* c = malloc(sizeof(SPIDevice));
	c->p = p;
	c->start = &_spi_dev_start;
	c->stop = &_spi_dev_stop;
	c->dcs = &_spi_dev_dcs;
	c->send = &_spi_dev_send;
	c->read = &_spi_dev_read;
	
	return c;
}


#endif
