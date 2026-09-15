#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "gpio.h"
#include "usart.h"
#include "queue.h"
#include <string.h>

#include "app_led.h"

const osThreadAttr_t ledTask_attribute = {
    .name = "ledTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

QueueHandle_t ledCommandQueue;

void app_led_init (void){
    ledCommandQueue = xQueueCreate(4, sizeof(LedCommand_t));
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


void StartLedTask(void *argument)
{
  LedCommand_t command = LED_COMMAND_ON;
  TickType_t blinkDelay = pdMS_TO_TICKS(500);

  (void)argument;

  Led_Set(0);

  for (;;)
  {
    if (command == LED_COMMAND_ON)
    {
      Led_Set(1);
      xQueueReceive(ledCommandQueue, &command, portMAX_DELAY);
    }
    else if (command == LED_COMMAND_OFF)
    {
      Led_Set(0);
      xQueueReceive(ledCommandQueue, &command, portMAX_DELAY);
    }
    else
    {
      Led_Set(1);

      if (xQueueReceive(ledCommandQueue, &command, blinkDelay) != pdPASS)
      {
        Led_Set(0);

        if (xQueueReceive(ledCommandQueue, &command, blinkDelay) != pdPASS)
        {
          command = LED_COMMAND_BLINK;
        }
      }
    }
  }
}