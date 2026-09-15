#ifndef APP_LED_H
#define APP_LED_H

typedef enum
{
    LED_COMMAND_ON,
    LED_COMMAND_OFF,
    LED_COMMAND_BLINK
} LedCommand_t;

const extern osThreadAttr_t ledTask_attribute; 

extern QueueHandle_t ledCommandQueue;

void StartLedTask(void *argument);
void app_led_init(void);

#endif