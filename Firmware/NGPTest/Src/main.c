/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. 
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
#include "stm32f1xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "Keypad.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
	/* Private variables ---------------------------------------------------------*/
	OLED* oled;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
	/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void checkSD() {
	oled->trans(oled->p, OLED_SCREEN_SMALL);
	if (HAL_GPIO_ReadPin(SDST_GPIO_Port, SDST_Pin) == GPIO_PIN_RESET) {
		HAL_SD_Init(&hsd, &SDCardInfo);
		oled->printfc(oled->p, 1, "SD card detected!");
	} else {
		HAL_SD_DeInit(&hsd);
		oled->printfc(oled->p, 1, "                 ");
	}
	oled->trans(oled->p, OLED_SCREEN_BIG);
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t index = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_SDIO_SD_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();

  /* USER CODE BEGIN 2 */
		oled = OLEDInit(SDA_GPIO_Port, SDA_Pin, SCL_GPIO_Port, SCL_Pin, OLED_SCREEN_BIG);
		
		oled->init(oled->p);
		oled->flash(oled->p, __NYAGAME_LOGO_);
		oled->trans(oled->p, OLED_SCREEN_SMALL);
		oled->init(oled->p);
		oled->font(oled->p, FontBig);
		oled->printfc(oled->p, 0, "NGP");
		oled->font(oled->p, FontSmall);
		oled->printfc(oled->p, 3, "Initializing!");
		oled->trans(oled->p, OLED_SCREEN_BIG);
		HAL_Delay(2000);
		
		oled->clear(oled->p);
		oled->trans(oled->p, OLED_SCREEN_SMALL);
		oled->clear(oled->p);
		oled->trans(oled->p, OLED_SCREEN_BIG);
		HAL_Delay(500);
	  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		HAL_GPIO_WritePin(LEDA_GPIO_Port, LEDA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_SET);
		
		if (index < 10) {
			/* MENU BUILD BEGIN */
			oled->color(oled->p, Black);
			oled->printfc(oled->p, 0, "MENU");
			oled->color(oled->p, White);
			oled->printfc(oled->p, 2, "Application 1"); //13 chars
			oled->printfc(oled->p, 3, "Application 2");
			oled->printfc(oled->p, 4, "Application 3");
			oled->printfc(oled->p, 5, "Application 4");
			oled->printfc(oled->p, 6, "About NyaGame");
			
			oled->draw(oled->p, 16, index + 1, ' ');
			oled->draw(oled->p, 16, index + 2, '>');
			oled->draw(oled->p, 16, index + 3, ' ');
			oled->draw(oled->p, 106, index + 1, ' ');
			oled->draw(oled->p, 106, index + 2, '<');
			oled->draw(oled->p, 106, index + 3, ' ');
			
			if (checkKeyUp(LPAD_UP)) index = ((index > 0) ? index - 1 : 0);
			if (checkKeyUp(LPAD_DOWN)) index = ((index < 4) ? index + 1 : 4);
			if (checkKeyUp(RPAD_UP)) {
				index += 10;
				oled->clear(oled->p);
			}
		} else if (index < 20) {
			switch (index) {
				case 10:
					oled->color(oled->p, Black);
					oled->printfc(oled->p, 0, "Application 1");
					oled->color(oled->p, White);
					oled->printfc(oled->p, 4, "This is a sample.");
					break;
				case 11:
					oled->color(oled->p, Black);
					oled->printfc(oled->p, 0, "Application 2");
					oled->color(oled->p, White);
					oled->printfc(oled->p, 4, "This is a sample.");
					break;
				case 12:
					oled->color(oled->p, Black);
					oled->printfc(oled->p, 0, "Application 3");
					oled->color(oled->p, White);
					oled->printfc(oled->p, 4, "This is a sample.");
					break;
				case 13:
					oled->color(oled->p, Black);
					oled->printfc(oled->p, 0, "Application 4");
					oled->color(oled->p, White);
					oled->printfc(oled->p, 4, "This is a sample.");
					break;
				case 14:
					oled->flash(oled->p, __NYAGAME_LOGO_);
					break;
				default:
					break;
			}
			
			if (checkKeyUp(RPAD_RIGHT)) {
				index -= 10;
				oled->clear(oled->p);
			}
		} else {
			index = 0;
			oled->clear(oled->p);
		}
		
		/* MENU BUILD END */
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_GPIO_WritePin(LEDA_GPIO_Port, LEDA_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_RESET);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hsd.Init.ClockDiv = 0;

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PADS0_Pin PADS1_Pin PADS2_Pin PADS3_Pin */
  GPIO_InitStruct.Pin = PADS0_Pin|PADS1_Pin|PADS2_Pin|PADS3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDA_Pin LEDB_Pin */
  GPIO_InitStruct.Pin = LEDA_Pin|LEDB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LPAD_Pin RPAD_Pin SCL_Pin SDA_Pin */
  GPIO_InitStruct.Pin = LPAD_Pin|RPAD_Pin|SCL_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SDST_Pin */
  GPIO_InitStruct.Pin = SDST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SDST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LEDA_Pin|LEDB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LPAD_Pin|RPAD_Pin|SCL_Pin|SDA_Pin, GPIO_PIN_RESET);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM8) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

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
