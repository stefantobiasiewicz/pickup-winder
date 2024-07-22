/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define MOTOR_PWM_Pin GPIO_PIN_0
#define MOTOR_PWM_GPIO_Port GPIOA
#define MOTOR_RELAY_Pin GPIO_PIN_1
#define MOTOR_RELAY_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOC
#define LCD_D1_Pin GPIO_PIN_5
#define LCD_D1_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_1
#define LCD_RS_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_2
#define LCD_E_GPIO_Port GPIOB
#define LCD_D2_Pin GPIO_PIN_6
#define LCD_D2_GPIO_Port GPIOC
#define HALL_Pin GPIO_PIN_7
#define HALL_GPIO_Port GPIOC
#define HALL_EXTI_IRQn EXTI9_5_IRQn
#define LCD_D3_Pin GPIO_PIN_8
#define LCD_D3_GPIO_Port GPIOC
#define Z_DIR_Pin GPIO_PIN_8
#define Z_DIR_GPIO_Port GPIOA
#define MOTOR_EN_Pin GPIO_PIN_9
#define MOTOR_EN_GPIO_Port GPIOA
#define X_STEP_Pin GPIO_PIN_10
#define X_STEP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define X_DIR_Pin GPIO_PIN_4
#define X_DIR_GPIO_Port GPIOB
#define Z_STEP_Pin GPIO_PIN_5
#define Z_STEP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void update_us_timer();
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
