#include "./Include/keypad.h"

#include "main.h"

uint8_t scanKeys() {
	uint8_t value = 0;

	HAL_GPIO_WritePin(LPAD_GPIO_Port, LPAD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RPAD_GPIO_Port, RPAD_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(PADS0_GPIO_Port, PADS0_Pin) == GPIO_PIN_RESET) value |= LPAD_DOWN;
	if (HAL_GPIO_ReadPin(PADS1_GPIO_Port, PADS1_Pin) == GPIO_PIN_RESET) value |= LPAD_LEFT;
	if (HAL_GPIO_ReadPin(PADS2_GPIO_Port, PADS2_Pin) == GPIO_PIN_RESET) value |= LPAD_UP;
	if (HAL_GPIO_ReadPin(PADS3_GPIO_Port, PADS3_Pin) == GPIO_PIN_RESET) value |= LPAD_RIGHT;

	HAL_GPIO_WritePin(RPAD_GPIO_Port, RPAD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LPAD_GPIO_Port, LPAD_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(PADS0_GPIO_Port, PADS0_Pin) == GPIO_PIN_RESET) value |= RPAD_DOWN;
	if (HAL_GPIO_ReadPin(PADS1_GPIO_Port, PADS1_Pin) == GPIO_PIN_RESET) value |= RPAD_RIGHT;
	if (HAL_GPIO_ReadPin(PADS2_GPIO_Port, PADS2_Pin) == GPIO_PIN_RESET) value |= RPAD_UP;
	if (HAL_GPIO_ReadPin(PADS3_GPIO_Port, PADS3_Pin) == GPIO_PIN_RESET) value |= RPAD_LEFT;

	return value;
}

uint8_t checkKey(uint8_t key) {
	return scanKeys() & key;
}

uint8_t waitKeyUp(uint8_t key) {
	if (checkKey(key)) {
		while (checkKey(key));
		return 1;
	}
	return 0;
}
