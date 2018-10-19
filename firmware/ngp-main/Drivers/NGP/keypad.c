#include "./Include/keypad.h"

#include "main.h"

uint8_t scanKeys() {
	uint8_t value = 0;

	HAL_GPIO_WritePin(KEY_OUTA_GPIO_Port, KEY_OUTA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_OUTB_GPIO_Port, KEY_OUTB_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(KEY_INA_GPIO_Port, KEY_INA_Pin) == GPIO_PIN_RESET) value |= LPAD_UP;
	if (HAL_GPIO_ReadPin(KEY_INB_GPIO_Port, KEY_INB_Pin) == GPIO_PIN_RESET) value |= LPAD_DOWN;
	if (HAL_GPIO_ReadPin(KEY_INC_GPIO_Port, KEY_INC_Pin) == GPIO_PIN_RESET) value |= LPAD_LEFT;
	if (HAL_GPIO_ReadPin(KEY_IND_GPIO_Port, KEY_IND_Pin) == GPIO_PIN_RESET) value |= LPAD_RIGHT;

	HAL_GPIO_WritePin(KEY_OUTB_GPIO_Port, KEY_OUTB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_OUTA_GPIO_Port, KEY_OUTA_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(KEY_INA_GPIO_Port, KEY_INA_Pin) == GPIO_PIN_RESET) value |= RPAD_UP;
	if (HAL_GPIO_ReadPin(KEY_INB_GPIO_Port, KEY_INB_Pin) == GPIO_PIN_RESET) value |= RPAD_DOWN;
	if (HAL_GPIO_ReadPin(KEY_INC_GPIO_Port, KEY_INC_Pin) == GPIO_PIN_RESET) value |= RPAD_LEFT;
	if (HAL_GPIO_ReadPin(KEY_IND_GPIO_Port, KEY_IND_Pin) == GPIO_PIN_RESET) value |= RPAD_RIGHT;

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
