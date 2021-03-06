/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "usbd_core.h"

#include "IRQ.h"
#include "OLED.h"
#include "LCD.h"
#include "Keypad.h"
#include "Beeper.h"
#include "Flash.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
	LCD* lcd;
	Flash* flash;
	static uint8_t sdOK = 0;
	static uint8_t index = 0;
	static uint8_t music = 0;
	
	const uint8_t cmusic = 7;
	
	FATFS fileSystem;
	FIL testFile;
	uint8_t testBuffer[16] = "Hello Gensokyo!\0";
	UINT testBytes;
	FRESULT res;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/** STG Begin *********************************************************************/

#define STG_BACK 1
#define STG_MAIN 2
#define STG_COLORB 0x000000

#define RANGE_X1 4
#define RANGE_Y1 4
#define RANGE_X2 92
#define RANGE_Y2 123

void onInitPlayer(void);
void onUpdatePlayer(void);
struct Player {
	uint32_t color;
	uint8_t size;
	double x;
	double y;
	double prevX;
	double prevY; 
	void (*onInit)();
	void (*onUpdate)();
} player = { 0xFF9800, 1,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	(RANGE_X2 + RANGE_X1) / 2, 108,
	&onInitPlayer,
	&onUpdatePlayer
};
void onInitPlayer() {
	player.x = (RANGE_X2 + RANGE_X1) / 2;
	player.y = 108;
}
void onUpdatePlayer() {
	player.prevX = player.x; player.prevY = player.y;
	if (checkKey(LPAD_LEFT)) { player.x -= 0.4; }
	if (checkKey(LPAD_RIGHT)) { player.x += 0.4; }
	if (checkKey(LPAD_UP)) { player.y -= 0.4; }
	if (checkKey(LPAD_DOWN)) { player.y += 0.4; }

	if (player.x > RANGE_X2 - player.size) player.x = RANGE_X2 - player.size;
	if (player.x < RANGE_X1 + player.size) player.x = RANGE_X1 + player.size;
	if (player.y > RANGE_Y2 - player.size) player.y = RANGE_Y2 - player.size;
	if (player.y < RANGE_Y1 + player.size) player.y = RANGE_Y1 + player.size;

	if ((char) player.x != (char) player.prevX || (char) player.y != (char) player.prevY) {
		lcd->colorf(lcd->p, STG_COLORB);
		lcd->rect(lcd->p,
			player.prevX - player.size, player.prevY - player.size,
			player.prevX + player.size, player.prevY + player.size,
			1
		);
	}
	lcd->colorf(lcd->p, player.color);
	lcd->rect(lcd->p,
		player.x - player.size, player.y - player.size,
		player.x + player.size, player.y + player.size,
		1
	);
}

void STG(uint8_t type) {
	if (type == STG_BACK) {
		lcd->colorf(lcd->p, 0xFFFFFF);
		lcd->rect(lcd->p, RANGE_X1 - 4, RANGE_Y1 - 4, RANGE_X2 + 4, RANGE_Y2 + 4, 1);
		lcd->colorf(lcd->p, STG_COLORB);
		lcd->rect(lcd->p, RANGE_X1 - 2, RANGE_Y1 - 2, RANGE_X2 + 2, RANGE_Y2 + 2, 1);
		lcd->colorf(lcd->p, 0xFFFFFF);
		lcd->rect(lcd->p, RANGE_X1 - 1, RANGE_Y1 - 1, RANGE_X2 + 1, RANGE_Y2 + 1, 0);
		lcd->colorf(lcd->p, 0x7986CB);
		lcd->font(lcd->p, Big);
		lcd->draw(lcd->p, RANGE_X2 + 12, RANGE_Y1 + 4, 'T');
		lcd->draw(lcd->p, RANGE_X2 + 12 + 8, RANGE_Y1 + 4, 'H');
		lcd->font(lcd->p, Small);
		lcd->colorf(lcd->p, 0xFFFFFF);
		
		player.onInit();
	} else if (type == STG_MAIN) {
		player.onUpdate();
	}
}

/** STG End ***********************************************************************/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void checkSD() {
	if (HAL_GPIO_ReadPin(SDST_GPIO_Port, SDST_Pin) == GPIO_PIN_RESET && sdOK == 0) {
		HAL_SD_Init(&hsd, &SDCardInfo);
		sdOK = 1;
	} else if (HAL_GPIO_ReadPin(SDST_GPIO_Port, SDST_Pin) == GPIO_PIN_SET && sdOK == 1) {
		HAL_SD_DeInit(&hsd);
		sdOK = 0;
	}
	if (sdOK == 1) {
		lcd->colorb(lcd->p, 0x66CCFF);
		lcd->colorf(lcd->p, 0x000000);
		lcd->print(lcd->p, 113, 118, "SD");
		lcd->colorf(lcd->p, 0x66CCFF);
		lcd->rect(lcd->p, 113, 117, 125, 125, 0);
		lcd->colorb(lcd->p, 0x000000);
		lcd->colorf(lcd->p, 0xFFFFFF);
	} else {
		lcd->p->foreColor = lcd->p->backColor;
		lcd->rect(lcd->p, 113, 117, 125, 125, 1);
		lcd->colorf(lcd->p, 0xFFFFFF);
	}
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDIO_SD_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN 2 */
	USBD_Stop(&hUsbDeviceFS);
	
	if (HAL_GPIO_ReadPin(SDST_GPIO_Port, SDST_Pin) == GPIO_PIN_RESET && sdOK == 0) {
		HAL_SD_Init(&hsd, &SDCardInfo);
		sdOK = 1;
	} else if (HAL_GPIO_ReadPin(SDST_GPIO_Port, SDST_Pin) == GPIO_PIN_SET && sdOK == 1) {
		HAL_SD_DeInit(&hsd);
		sdOK = 0;
	}
  
	HAL_Delay(500);
	
	flash = FlashInit(&hspi1,
			FLASH_CS_GPIO_Port, FLASH_CS_Pin, W25Q64);
	
	lcd = LCDInit(&hspi1, 
			LCD_DC_GPIO_Port, LCD_DC_Pin, 
			LCD_CS_GPIO_Port, LCD_CS_Pin,
			LCD_RST_GPIO_Port, LCD_RST_Pin,
			LCD_BK_GPIO_Port, LCD_BK_Pin);
			
	lcd->reset(lcd->p);
	
	lcd->init(lcd->p);
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);
	lcd->bitmapsc(lcd->p, 63, 63, 64, 64, __NYAGAME_LOGO_);
	HAL_Delay(2000);
	lcd->colorb(lcd->p, 0x000000);
	lcd->colorf(lcd->p, 0xFFFFFF);
	lcd->clear(lcd->p);
	
	HAL_GPIO_WritePin(LEDA_GPIO_Port, LEDA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_SET);
	
	{
		
		f_mount(&fileSystem, SD_Path, 1);
		if(f_mount(&fileSystem, SD_Path, 1) == FR_OK) {
			uint8_t path[13] = "NYA_GAME.TXT";
			path[12] = '\0';
		 
			res = f_open(&testFile, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);
		 
			res = f_write(&testFile, testBuffer, 16, &testBytes);
		 
			res = f_close(&testFile);
		}
	
	}
	HAL_GPIO_WritePin(USB_PULLUP_GPIO_Port, USB_PULLUP_Pin, GPIO_PIN_SET);
	USBD_Start(&hUsbDeviceFS);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (index < 10) {
			/* MENU BUILD BEGIN */
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFF9800);
			lcd->printfc(lcd->p, 8, "MENU");
			lcd->colorb(lcd->p, 0x000000);
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->printfc(lcd->p, 24, "W25Q64 Test");
			lcd->printfc(lcd->p, 32, "STG Sample");
			lcd->printfc(lcd->p, 40, "Flash Test");
			lcd->printfc(lcd->p, 48, "NSDN-Beeper");
			lcd->printfc(lcd->p, 56, "About this NGP");
			
			lcd->draw(lcd->p, 10, (index + 2) * 8, ' ');
			lcd->draw(lcd->p, 10, (index + 3) * 8, '>');
			lcd->draw(lcd->p, 10, (index + 4) * 8, ' ');
			lcd->draw(lcd->p, 112, (index + 2) * 8, ' ');
			lcd->draw(lcd->p, 112, (index + 3) * 8, '<');
			lcd->draw(lcd->p, 112, (index + 4) * 8, ' ');
			
			if (waitKeyUp(LPAD_UP)) index = ((index > 0) ? index - 1 : 0);
			if (waitKeyUp(LPAD_DOWN)) index = ((index < 4) ? index + 1 : 4);
			if (waitKeyUp(RPAD_UP)) {
				index += 10;
				lcd->clear(lcd->p);
				
				switch (index) {
				 	case 10:
						break;
					case 11:
						STG(STG_BACK);
						break;
					case 12:
						break;
					case 13:
						HAL_TIM_Base_Start_IT(&htim10);
						setVolume(2);
						music = 0;
						break;
					case 14:
						lcd->colorb(lcd->p, 0xFFFFFF);
						lcd->colorf(lcd->p, 0x000000);
						lcd->clear(lcd->p);
						break;
					default:
						break;
				}
			}
		} else if (index < 20) {
			switch (index) {
				case 10:
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFF9800);
					lcd->printfc(lcd->p, 8, "W25Q64 Test");
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFFFFFF);
					lcd->printfc(lcd->p, 48, "Init W25Q64...");
					uint8_t result, manuf, buf;
					result = flash->begin(flash->p);
					manuf = flash->readManufacturer(flash->p);
					flash->read(flash->p, 0x00000000, &buf, 1);
					flash->end(flash->p);
					lcd->printfc(lcd->p, 64, "Result: %X", result);
					lcd->printfc(lcd->p, 72, "Manuf: %X", manuf);
					lcd->printfc(lcd->p, 80, "Buf: %X", buf);
					break;
				case 11:
					STG(STG_MAIN);
					break;
				case 12:
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFF9800);
					lcd->printfc(lcd->p, 8, "Flash Test");
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0x3F51B5);
					lcd->printfc(lcd->p, 24, "Flashing: 0x3F51B5");
					lcd->rect(lcd->p, 16, 48, 112, 112, 1);
					lcd->colorf(lcd->p, 0xB10000);
					lcd->printfc(lcd->p, 24, "Flashing: 0xB10000");
					lcd->rect(lcd->p, 16, 48, 112, 112, 1);
					lcd->colorf(lcd->p, 0xFFFFFF);
					break;
				case 13:
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFF9800);
					lcd->printfc(lcd->p, 8, "NSDN-Beeper");
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFFFFFF);
					lcd->printfc(lcd->p, 32, "NOW Playing...");
					lcd->printfc(lcd->p, 48, "%d of %d", cmusic, music + 1);
					switch (music) {
						case 0:
							lcd->printfc(lcd->p, 64, "Septet for");
							lcd->printfc(lcd->p, 72, "the Dead Princess");
							playMusicWithSpace(SYMBOL, MID_remilia, MID_remilia_LENGTH, 233, 16, 1);
							break;
						case 1:
							lcd->printfc(lcd->p, 64, "The Way of Doom God");
							lcd->printfc(lcd->p, 72, "- Dark Road -");
							playMusicWithSpace(SYMBOL, MID_hina, MID_hina_LENGTH, 233, 16, 1);
							break;
						case 2:
							lcd->printfc(lcd->p, 64, "Romantic Escape");
							lcd->printfc(lcd->p, 72, "Flight & Foul for");
							lcd->printfc(lcd->p, 80, "Impossible Bullets"); 
							playMusicWithSpace(SYMBOL, MID_seija, MID_seija_LENGTH, 233, 16, 1);
							break;
						case 3:
							lcd->printfc(lcd->p, 64, "Lively and Innocent");
							lcd->printfc(lcd->p, 72, "Girl (9)");
							playMusicWithSpace(SYMBOL, MID_9, MID_9_LENGTH, 233, 16, 1);
							break;
						case 4:
							lcd->printfc(lcd->p, 64, "Broken Moon");
							playMusicWithSpace(SYMBOL, MUSIC, MUSIC_LENGTH, 233, 16, 1);
							break;
						case 5:
							lcd->printfc(lcd->p, 64, "U.N. Owen was her?");
							playMusicWithSpace(SYMBOL, MID_U_N_Owen_was_her, MID_U_N_Owen_was_her_LENGTH, 128, 16, 1);
							break;
						case 6:
							lcd->printfc(lcd->p, 64, "Staking Your Life");
							lcd->printfc(lcd->p, 72, "on a Prank");
							playMusicWithSpace(SYMBOL, MID_Staking_Your_Life_on_a_Prank, MID_Staking_Your_Life_on_a_Prank_LENGTH, 192, 8, 1);
							break;
					}
					break;
				case 14:
					lcd->colorb(lcd->p, 0xFFFFFF);
					lcd->colorf(lcd->p, 0x000000);
					lcd->bitmapsc(lcd->p, 63, 48, 64, 64, __NYAGAME_LOGO_);
					lcd->printfc(lcd->p, 86, "NyaGame Portable");
					lcd->printfc(lcd->p, 98, "dev161229");
					break;
				default:
					break;
			}
			
			if (index == 11) {
				if (waitKeyUp(RPAD_DOWN)) {
					index -= 10;
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFF9800);
					lcd->clear(lcd->p);
				}
			}
			if (index != 13 && index != 11) {
				if (waitKeyUp(RPAD_RIGHT)) {
					index -= 10;
					lcd->colorb(lcd->p, 0x000000);
					lcd->colorf(lcd->p, 0xFF9800);
					lcd->clear(lcd->p);
					
					switch (index) {
						case 10:
							break;
						case 11:
							break;
						case 12:
							break;
						case 13:
							HAL_TIM_Base_Start_IT(&htim10);
							setVolume(2);
							break;
						case 14:
							break;
						default:
							break;
					}
				}
			}
		} else {
			index = 0;
			lcd->clear(lcd->p);
		}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LEDA_GPIO_Port, LEDA_Pin);
		HAL_GPIO_TogglePin(LEDB_GPIO_Port, LEDB_Pin);
		checkSD();
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SDIO init function */
static void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 4;

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM10 init function */
static void MX_TIM10_Init(void)
{

  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 4;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 239;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PADS0_Pin PADS1_Pin PADS2_Pin PADS3_Pin */
  GPIO_InitStruct.Pin = PADS0_Pin|PADS1_Pin|PADS2_Pin|PADS3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LPAD_Pin RPAD_Pin FLASH_CS_Pin IO_OUT_Pin */
  GPIO_InitStruct.Pin = LPAD_Pin|RPAD_Pin|FLASH_CS_Pin|IO_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDA_Pin LEDB_Pin LBEEP_Pin RBEEP_Pin */
  GPIO_InitStruct.Pin = LEDA_Pin|LEDB_Pin|LBEEP_Pin|RBEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_ENB_Pin USB_PULLUP_Pin LCD_BK_Pin LCD_CS_Pin 
                           LCD_DC_Pin LCD_RST_Pin SCL_Pin USB_OTG_Pin */
  GPIO_InitStruct.Pin = BT_ENB_Pin|USB_PULLUP_Pin|LCD_BK_Pin|LCD_CS_Pin 
                          |LCD_DC_Pin|LCD_RST_Pin|SCL_Pin|USB_OTG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BT_STATE_Pin */
  GPIO_InitStruct.Pin = BT_STATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BT_STATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDST_Pin */
  GPIO_InitStruct.Pin = SDST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SDST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LPAD_Pin|RPAD_Pin|IO_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LEDA_Pin|LEDB_Pin|LBEEP_Pin|RBEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BT_ENB_Pin|USB_PULLUP_Pin|LCD_BK_Pin|LCD_DC_Pin 
                          |SCL_Pin|SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_RST_Pin|USB_OTG_Pin, GPIO_PIN_SET);

}

/* USER CODE BEGIN 4 */

void TIM_IRQ_Callback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM10) {
		if (index == 13) {
			if (waitKeyUp(RPAD_RIGHT)) {
				jumpOut();
				index -= 10;
				lcd->colorb(lcd->p, 0x000000);
				lcd->clear(lcd->p);
				HAL_TIM_Base_Stop_IT(&htim10);
				HAL_GPIO_WritePin(LBEEP_GPIO_Port, LBEEP_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(RBEEP_GPIO_Port, RBEEP_Pin, GPIO_PIN_RESET);
			}
			if (waitKeyUp(LPAD_UP)) {
				if (music > 0) music -= 1;
				else music = cmusic - 1;
				jumpOut();
				lcd->colorb(lcd->p, 0x000000);
				lcd->clear(lcd->p);
			} else if (waitKeyUp(LPAD_DOWN)) {
				if (music < cmusic - 1) music += 1;
				else music = 0;
				jumpOut();
				lcd->colorb(lcd->p, 0x000000);
				lcd->clear(lcd->p);
			}
		}
		beeperInerrupt();
	}

	
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
