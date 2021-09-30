/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lptim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
stepper_motor extruder = { 0 };
stepper_dir DIR = CW;

dc_motor DC_motor = { 0 };
encoder_button enc_btn;
cursor_position cursor_pos;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t message[32];
uint8_t length;
uint16_t enc_value = 0;
uint16_t DC_enc_value = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM16_Init();
  MX_LPTIM1_Init();
  /* USER CODE BEGIN 2 */
	encoder_init(&enc_btn, ENC_BTN_GPIO_Port, ENC_BTN_Pin, 20);
	delay_init();
	ST7920_Init();
	stepper_init(&extruder, &htim1, TIM_CHANNEL_1, &htim2, EXT_DIR_GPIO_Port,
	EXT_DIR_Pin);
	DC_motor_Init(&DC_motor, &htim16, TIM_CHANNEL_1, CUT_DIR_IN1_GPIO_Port,
	CUT_DIR_IN1_Pin, CUT_DIR_IN2_GPIO_Port, CUT_DIR_IN2_Pin, &hlptim1);
	Init_menu(&cursor_pos);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	stepper_set_angle(&extruder, 360, 10, DIR);
	DC_set_angle(&DC_motor, 360, 50, DIR);
	while (1) {

		ENC_Button_Action(&enc_btn, &cursor_pos);
		menu_update(&cursor_pos);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim) {
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hlptim);
	DC_stop(&DC_motor);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (FIRST_MISS_FLAG == 1) {
		if (htim->Instance == extruder.slave_timer.htim->Instance) {
			stepper_stop(&extruder);
		}
	}
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//	if (GPIO_Pin == enc_btn.PIN) {
//
//		enc_btn.last_tick = HAL_GetTick();

//		if(!(HAL_GPIO_ReadPin(enc_btn.PORT, enc_btn.PIN)))
//		{
//
//			if(cursor_pos.current_layer == FIRST_LAYER)
//			{
//				if(cursor_pos.FL_position == SETTINGS)
//				{
//					cursor_pos.current_layer = SECOND_LAYER;
//					__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
//				}
//
//				// START pressed
//
//				else if(cursor_pos.FL_position == START)
//				{
//					stepper_set_angle(&extruder, 360, 10, DIR);
//					DC_set_angle(&DC_motor, 360, 50, LEFT);
//
//					__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
//					cursor_pos.FL_position = DEFAULT;
//				}
//
//				else if(cursor_pos.FL_position == STOP)
//				{
//					stepper_stop(&extruder);
//					DC_stop(&DC_motor);
//
//					__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
//					cursor_pos.FL_position = DEFAULT;
//				}
//
//
//
//			}
//
//			else if(cursor_pos.current_layer == SECOND_LAYER)
//			{
//				if(cursor_pos.SL_position == BACK)
//				{
//					cursor_pos.current_layer = FIRST_LAYER;
//					cursor_pos.FL_position = DEFAULT;
//				}
//
//				else if(cursor_pos.SL_position == FIL_DIA)
//				{
//					cursor_pos.current_layer = THIRD_LAYER;
//					cursor_pos.TL_position = DIAMETER_175;
//				}
//
//				else if(cursor_pos.SL_position == FIL_DEN)
//				{
//					cursor_pos.current_layer = THIRD_LAYER;
//					cursor_pos.TL_position = DENSITY_PLA;
//				}
//
//
//			}
//
//		}
//
//	}
//
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
