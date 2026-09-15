/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "gpio.h"
#include "usart.h"
#include "queue.h"
#include <string.h>

#include "../../src/dev/M0_App/AppOS/App_cli/app_cli.h"
#include "../../src/dev/M0_App/AppOS/App_led/app_led.h"
#include "../../src/dev/M0_App/AppOS/App_x_root/app_root.h"

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
/* USER CODE BEGIN Variables */


/* USER CODE END Variables */

/* Definitions for defaultTask */
// osThreadId_t defaultTaskHandle;
// const osThreadAttr_t defaultTask_attributes = {
//   .name = "defaultTask",
//   .stack_size = 128 * 4,
//   .priority = (osPriority_t) osPriorityNormal,
// };

// const osThreadAttr_t ledTask_attributes = {
//   .name = "ledTask",
//   .stack_size = 128 * 4,
//   .priority = (osPriority_t) osPriorityAboveNormal,
// };

// const osThreadAttr_t consoleTask_attributes = {
//   .name = "consoleTask",
//   .stack_size = 256 * 4,
//   .priority = (osPriority_t) osPriorityAboveNormal,
// };

// const osThreadAttr_t blinkTask_attributes = {
// 	.name = "binkTask",
// 	.stack_size = 128 * 4,
// 	.priority = osPriorityAboveNormal1,
// };


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

// void StartLedTask(void *argument);
// void StartConsoleTask(void *argument);
// void StartBlinkTask(void *argument);

/* USER CODE END FunctionPrototypes */

// void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void){
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  app_cli_init();
  app_led_init();

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  // defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  // osThreadNew(StartConsoleTask, NULL, &consoleTask_attributes);
  // osThreadNew(StartLedTask, NULL, &ledTask_attributes);
  // osThreadNew(StartBlinkTask, NULL, &blinkTask_attributes);

  Approot_Growup();

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  HAL_UART_Receive_IT(&huart1, &uartRxByte, 1);

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
// void StartDefaultTask(void *argument)
// {
//   /* USER CODE BEGIN StartDefaultTask */
//   /* Infinite loop */
//   for(;;)
//   {
//     osDelay(1);
//   }
//   /* USER CODE END StartDefaultTask */
// }

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// static void Led_Set(uint8_t on)
// {
//   /* LED PC13 trên Blue Pill thường active-low */
//   HAL_GPIO_WritePin(
//       GPIOC,
//       GPIO_PIN_13,
//       on ? GPIO_PIN_RESET : GPIO_PIN_SET);
// }

// void StartLedTask(void *argument)
// {
//   LedCommand_t command = LED_COMMAND_ON;
//   TickType_t blinkDelay = pdMS_TO_TICKS(500);

//   (void)argument;

//   Led_Set(0);

//   for (;;)
//   {
//     if (command == LED_COMMAND_ON)
//     {
//       Led_Set(1);
//       xQueueReceive(ledCommandQueue, &command, portMAX_DELAY);
//     }
//     else if (command == LED_COMMAND_OFF)
//     {
//       Led_Set(0);
//       xQueueReceive(ledCommandQueue, &command, portMAX_DELAY);
//     }
//     else
//     {
//       Led_Set(1);

//       if (xQueueReceive(ledCommandQueue, &command, blinkDelay) != pdPASS)
//       {
//         Led_Set(0);

//         if (xQueueReceive(ledCommandQueue, &command, blinkDelay) != pdPASS)
//         {
//           command = LED_COMMAND_BLINK;
//         }
//       }
//     }
//   }
// }

// void StartConsoleTask(void *argument)
// {
//   uint8_t receivedByte;
//   char commandLine[16];
//   uint8_t commandLength = 0;
//   LedCommand_t command;

//   (void)argument;

//   const char welcome[] =
//       "\r\nCommands: on, off, blink\r\n> ";

//   HAL_UART_Transmit(
//       &huart1,
//       (uint8_t *)welcome,
//       sizeof(welcome) - 1,
//       HAL_MAX_DELAY);

//   for (;;)
//   {
//     if (xQueueReceive(
//             uartRxQueue,
//             &receivedByte,
//             portMAX_DELAY) != pdPASS)
//     {
//       continue;
//     }

//     if ((receivedByte == '\r') || (receivedByte == '\n'))
//     {
//       if (commandLength == 0)
//       {
//         continue;
//       }

//       commandLine[commandLength] = '\0';

//       if (strcmp(commandLine, "on") == 0)
//       {
//         command = LED_COMMAND_ON;
//       }
//       else if (strcmp(commandLine, "f") == 0)
//       {
//         command = LED_COMMAND_OFF;
//       }
//       else if (strcmp(commandLine, "blink") == 0)
//       {
//         command = LED_COMMAND_BLINK;
//       }
//       else
//       {
//         const char errorMessage[] =
//             "\r\nUnknown command\r\n> ";

//         HAL_UART_Transmit(
//             &huart1,
//             (uint8_t *)errorMessage,
//             sizeof(errorMessage) - 1,
//             HAL_MAX_DELAY);

//         commandLength = 0;
//         continue;
//       }

//       xQueueSend(ledCommandQueue, &command, 0);

//       {
//         const char response[] = "\r\nOK\r\n> ";

//         HAL_UART_Transmit(
//             &huart1,
//             (uint8_t *)response,
//             sizeof(response) - 1,
//             HAL_MAX_DELAY);
//       }

//       commandLength = 0;
//     }
//     else if ((receivedByte == '\b') && (commandLength > 0))
//     {
//       commandLength--;
//     }
//     else if ((receivedByte >= ' ') &&
//              (commandLength < sizeof(commandLine) - 1))
//     {
//       commandLine[commandLength++] = (char)receivedByte;
//     }
//   }
// }

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//   BaseType_t higherPriorityTaskWoken = pdFALSE;

//   if (huart->Instance == USART1)
//   {
//     xQueueSendFromISR(
//         uartRxQueue,
//         &uartRxByte,
//         &higherPriorityTaskWoken);

//     HAL_UART_Receive_IT(
//         &huart1,
//         &uartRxByte,
//         1);

//     portYIELD_FROM_ISR(higherPriorityTaskWoken);
//   }
// }

/* USER CODE END Application */

