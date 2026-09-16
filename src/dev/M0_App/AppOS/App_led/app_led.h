#ifndef APP_LED_H
#define APP_LED_H

typedef enum
{
    LED_COMMAND_ON,
    LED_COMMAND_OFF,
    LED_COMMAND_BLINK
} LedCommand_t;

typedef struct
{
    LedCommand_t command;
    uint32_t blinkTimeMS;
} LedMessage_t;

extern LedMessage_t cli_message;

const extern osThreadAttr_t ledTask_attribute; 

extern QueueHandle_t ledCommandQueue;

void StartLedTask(void *argument);
void app_led_init(void);

#endif