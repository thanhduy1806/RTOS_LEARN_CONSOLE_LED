#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdint.h>
#include "gpio.h"
#include "usart.h"
#include "queue.h"
#include <string.h>
#include <stdio.h>

#include "app_led.h"

char buffer[20];


const char check_loop[]     = "\r\nCHECKING LOOP\r\n> ";
const char check_on[]       = "\r\nCHECKING ON\r\n> ";
const char check_off[]      = "\r\nCHECKING OFF\r\n> ";
const char check_blink[]    = "\r\nCHECKING BLINK\r\n> ";


const osThreadAttr_t ledTask_attribute = {
    .name = "ledTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

QueueHandle_t ledCommandQueue;
LedMessage_t cli_message = {.command = LED_COMMAND_ON, .blinkTimeMS = 0};

void app_led_init (void){
    ledCommandQueue = xQueueCreate(4, sizeof(LedMessage_t));
    if (ledCommandQueue == NULL)
    {
    Error_Handler();
    }
}

static void Led_Set(uint8_t on)
{
  /* LED PC13 trên Blue Pill thường active-low */
  HAL_GPIO_WritePin(
      GPIOC,
      GPIO_PIN_13,
      on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static void Led_Blink(uint32_t time_ms){
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  // vTaskDelay(pdMS_TO_TICKS(time_ms));
  // xQueueReceive(ledCommandQueue, &cli_message, pdMS_TO_TICKS(time_ms));
  xQueueReceive(ledCommandQueue, &cli_message, pdMS_TO_TICKS(time_ms));
}


void StartLedTask(void *argument)
{
  
//  TickType_t blinkDelay = pdMS_TO_TICKS(500);

  (void)argument;

  Led_Set(0);

  for (;;)
  {
    if (cli_message.command == LED_COMMAND_ON)
    {
      Led_Set(1);
      xQueueReceive(ledCommandQueue, &cli_message, portMAX_DELAY);
    }
    else if (cli_message.command == LED_COMMAND_OFF)
    {
      Led_Set(0);
      xQueueReceive(ledCommandQueue, &cli_message, portMAX_DELAY);
    }
    else
    {
      Led_Blink(cli_message.blinkTimeMS);
      HAL_UART_Transmit(&huart1, (uint8_t*)check_blink, sizeof(check_blink)-1, HAL_MAX_DELAY);

    }

  }
}







 



// static void Led_Blink(uint32_t time_ms){
//   HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//   // vTaskDelay(pdMS_TO_TICKS(time_ms));
  
// }


// void StartLedTask(void *argument)
// {
//   LedMessage_t cli_message = {.command = LED_COMMAND_ON, .blinkTimeMS = 0};
// //  TickType_t blinkDelay = pdMS_TO_TICKS(500);

//   (void)argument;

//   Led_Set(0);

//   for (;;)
//   {
//     if (cli_message.command == LED_COMMAND_ON)
//     {
//       Led_Set(1);
//       xQueueReceive(ledCommandQueue, &cli_message, portMAX_DELAY);
//     }
//     else if (cli_message.command == LED_COMMAND_OFF)
//     {
//       Led_Set(0);
//       xQueueReceive(ledCommandQueue, &cli_message, portMAX_DELAY);
//     }
//     else
//     {
//       HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//       xQueueReceive(ledCommandQueue, &cli_message, pdMS_TO_TICKS(cli_message.blinkTimeMS));
//     }

//     // xQueueReceive(ledCommandQueue, &cli_message, pdMS_TO_TICKS(cli_message.blinkTimeMS));

//   }
// }