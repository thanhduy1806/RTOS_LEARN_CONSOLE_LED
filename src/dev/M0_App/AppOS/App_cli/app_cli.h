#ifndef APP_CLI_H
#define APP_CLI_H

const extern osThreadAttr_t consoleTask_attributes;

extern QueueHandle_t uartRxQueue;

extern uint8_t uartRxByte;

void StartConsoleTask(void *argument);
void app_cli_init(void);

#endif
