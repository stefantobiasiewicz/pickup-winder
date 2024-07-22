/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbh_hid.h"


#include "../libs/lcd16x2.h"
#include "../libs/eeprom.h"

#include "../../appV2/machine_motion.h"
#include "../../appV2/program.h"
#include "../../appV2/machine.h"
#include "../../appV2/statemachine/winder_machine.h"

#include "../../appV2/statemachine/states/state_run.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
	if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)  // if the HID is Mouse
			{
		uint8_t key;
		HID_KEYBD_Info_TypeDef *Keyboard_Info;
		Keyboard_Info = USBH_HID_GetKeybdInfo(phost);  // get the info
		key = USBH_HID_GetASCIICode(Keyboard_Info);  // get the key pressed

		signal_t signal;
		signal.key_pressed = key;
		app_put_signal(signal);
	}
}

void ll_print(char *_1st_line, char *_2nd_line) {
	lcd16x2_clear();
	lcd16x2_1stLine();
	lcd16x2_printf(_1st_line);
	lcd16x2_2ndLine();
	lcd16x2_printf(_2nd_line);
}

void motor_enable_fun(bool en) {
	HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin,
			en == true ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void x_step_fun(bool dir) {
	HAL_GPIO_WritePin(X_DIR_GPIO_Port, X_DIR_Pin,
			dir == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(X_STEP_GPIO_Port, X_STEP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(X_STEP_GPIO_Port, X_STEP_Pin, GPIO_PIN_RESET);
}

void y_step_fun(bool dir) {
	HAL_GPIO_WritePin(Z_DIR_GPIO_Port, Z_DIR_Pin,
			dir == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Z_STEP_GPIO_Port, Z_STEP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Z_STEP_GPIO_Port, Z_STEP_Pin, GPIO_PIN_RESET);
}

/**
 * Set the PWM duty cycle for a motor connected to TIM5 channel 1.
 *
 * @param pwm The desired duty cycle in per mille (‰) for the motor's PWM signal.
 *            The input should be in the range of 0 to 1000, where 0‰ means the output is always low and
 *            1000‰ means the output is always high.
 *
 * Note: This function assumes that TIM5 and its channel 1 have been previously configured for PWM output.
 *       Make sure that the PWM initialization and starting functions are correctly set up before calling this function.
 */
void set_motor_pwm(int pwm) {
    if (pwm < 0) pwm = 0;        // Ensure pwm is not below 0‰
    if (pwm > 1000) pwm = 1000;  // Ensure pwm does not exceed 1000‰

    uint32_t max = TIM5->ARR;    // Get the auto-reload value of the timer
    uint32_t res = (max * pwm) / 1000;  // Calculate the compare value for the duty cycle in per mille

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, res);  // Set the compare value to adjust the duty cycle
}

/**
 * Control the direction of a motor relay.
 *
 * This function sets the state of a GPIO pin connected to a motor relay,
 * effectively controlling the relay's switching and, consequently, the direction
 * of the motor's operation.
 *
 * @param dir A boolean value specifying the desired state of the relay.
 *            If true, the relay is activated (pin set high).
 *            If false, the relay is deactivated (pin set low).
 */
void set_motor_relay(bool dir) {
	HAL_GPIO_WritePin(MOTOR_RELAY_GPIO_Port, MOTOR_RELAY_Pin,
			dir == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static long long us_tick = 0;
void update_us_timer() {
	us_tick++;
}

long get_us_fun() {
	return us_tick;
}


//void ll_read_flash(machine_static_params_t *machine_static_params) {
//
//}
//
//void ll_write_flash(machine_static_params_t *machine_static_params) {
//
//}


void appV2_error(const char * message) {
	ll_print("APP error handler", (char *)message);

	while(1);
}
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
  MX_USB_HOST_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	lcd16x2_init_4bits(GPIOB, LCD_RS_Pin, LCD_E_Pin,
			GPIOC, LCD_D1_Pin, LCD_D2_Pin, LCD_D3_Pin, LCD_D4_Pin);

	lcd16x2_printf("Winding Machine");
	lcd16x2_cursorShow(false);


	machine_control_t machine_controll_callbacks = {
			.print_fun = ll_print,
	};

	app_init(machine_controll_callbacks);

	motor_enable_fun(false);


	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);



	void gcode_done() {
		g_code_t * next = program_get_next_gcode();
		if(next != NULL) {
			state_run_current_gcode(next);
		} else {
			program_machine_wait_for_gcode();
		}
	}

	void current_g_code_in_proggress(g_code_t * code) {
		state_run_current_gcode(code);
	}


	// app V2
	machine_motion_callbacks_t cllbacks = {
			.get_us_fun = get_us_fun,
			.step_a = x_step_fun, // correct mapping of CNC sheeld and program
			.step_x = y_step_fun,
			.g_code_done = gcode_done,
			.current_g_code_info = current_g_code_in_proggress,
	};

	machine_motion_init(cllbacks);

	void program_start() {
		motor_enable_fun(true);
	}

	void program_finished() {
		motor_enable_fun(false);
		state_run_program_finished();
	}

	void program_send_g_code_to_machine(g_code_t * code) {
		machine_motion_move_g_code(code);
	}

	program_callbacks_t program_callbacks = {
		.program_start_callback = program_start,
		.program_finished_callback = program_finished,
		.program_send_next_gcode_to_machine = program_send_g_code_to_machine,
	};

	program_init(program_callbacks);


	machine_motion_set_speed_time(90);

//    basic_alg_args args = {
//        .distance = 12.5,
//        .distance_step = 2,
//        .turns =  20,
//    };
//
//
//    basic_alg_provider.init(&args);
//    program_start(&basic_alg_provider);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
		app_state_machine_loop();
		machine_motion_loop();
		program_loop();
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 200;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 10000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
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
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|Z_DIR_Pin|MOTOR_EN_Pin|X_STEP_Pin
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_D4_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RS_Pin|LCD_E_Pin|X_DIR_Pin|Z_STEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MOTOR_RELAY_Pin */
  GPIO_InitStruct.Pin = MOTOR_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MOTOR_RELAY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin Z_DIR_Pin MOTOR_EN_Pin X_STEP_Pin
                           PA15 */
  GPIO_InitStruct.Pin = LD2_Pin|Z_DIR_Pin|MOTOR_EN_Pin|X_STEP_Pin
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D1_Pin LCD_D2_Pin LCD_D3_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RS_Pin LCD_E_Pin X_DIR_Pin Z_STEP_Pin */
  GPIO_InitStruct.Pin = LCD_RS_Pin|LCD_E_Pin|X_DIR_Pin|Z_STEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : HALL_Pin */
  GPIO_InitStruct.Pin = HALL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(HALL_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
