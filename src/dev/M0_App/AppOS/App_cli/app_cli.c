/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "gpio.h"
#include "usart.h"
#include "queue.h"
#include <string.h>

#include "app_cli.h"
#include "../App_led/app_led.h"


const osThreadAttr_t consoleTask_attributes = {
  .name = "consoleTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

QueueHandle_t uartRxQueue;

uint8_t uartRxByte;

void app_cli_init(void){
    uartRxQueue = xQueueCreate(64, sizeof(uint8_t));
    if ((uartRxQueue == NULL))
    {
    Error_Handler();
    }
}

uint8_t check_number (char *command){
  if ((*command<= '9') && (*command >= '0')){
    return 1;
  }
  return 0;
}

uint32_t convert_to_decimal (char *command){
  uint32_t decimal= 0;
  uint32_t digit =0;
  while (*(command+digit) != '\0'){
    decimal = decimal * 10 + (uint32_t)(*(command + digit)-'0');
    digit++;
  }
  return decimal;
}




void StartConsoleTask(void *argument)
{
  uint8_t receivedByte;
  char commandLine[16];
  uint8_t commandLength = 0;
  // LedCommand_t command;

  const char welcome[]      = "\r\nCommands: on, off, blink\r\n> ";
  const char errorMessage[] = "\r\nUnknown command\r\n> ";
  const char response[]     = "\r\nOK\r\n> ";
  const char blinktime[]    = "\r\nEnter Time(ms)\r\n>";

  (void)argument;

  HAL_UART_Transmit(
      &huart1,
      (uint8_t *)welcome,
      sizeof(welcome) - 1,
      HAL_MAX_DELAY);

  for (;;)
  {
    if (xQueueReceive(
            uartRxQueue,
            &receivedByte,
            portMAX_DELAY) != pdPASS)
    {
      continue;
    }

    if ((receivedByte == '\r') || (receivedByte == '\n'))
    {
      if (commandLength == 0)
      {
        continue;
      }

      commandLine[commandLength] = '\0';

      if (strcmp(commandLine, "o") == 0)
      {
        cli_message.command = LED_COMMAND_ON;
      }
      else if (strcmp(commandLine, "f") == 0)
      {
        cli_message.command = LED_COMMAND_OFF;
      }
      else if (strcmp(commandLine, "blink") == 0)
      {
        cli_message.command = LED_COMMAND_BLINK;
        HAL_UART_Transmit(&huart1, (uint8_t*)blinktime, sizeof(blinktime)-1, HAL_MAX_DELAY);
        goto WaitEnterTime;
      }
      else if (check_number(commandLine)){
        cli_message.blinkTimeMS = convert_to_decimal(commandLine);
      }
      else
      {
        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)errorMessage,
            sizeof(errorMessage) - 1,
            HAL_MAX_DELAY);

        commandLength = 0;
        continue;
      }

      {
        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)response,
            sizeof(response) - 1,
            HAL_MAX_DELAY);
      }

      
      xQueueSend(ledCommandQueue, &cli_message, 0);
      WaitEnterTime:

      commandLength = 0;
    }
    else if ((receivedByte == '\b') && (commandLength > 0))
    {
      commandLength--;
    }
    else if ((receivedByte >= ' ') &&
             (commandLength < sizeof(commandLine) - 1))
    {
      commandLine[commandLength++] = (char)receivedByte;
    }
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  BaseType_t higherPriorityTaskWoken = pdFALSE;

  if (huart->Instance == USART1)
  {
    xQueueSendFromISR(
        uartRxQueue,
        &uartRxByte,
        &higherPriorityTaskWoken);

    HAL_UART_Transmit(&huart1, &uartRxByte, 1, HAL_MAX_DELAY);
    

    HAL_UART_Receive_IT(
        &huart1,
        &uartRxByte,
        1);

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
  }
}
