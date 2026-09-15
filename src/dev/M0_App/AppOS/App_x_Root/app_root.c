
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#include "gpio.h"
#include "usart.h"
#include "queue.h"
#include <string.h>

#include "../App_cli/app_cli.h"
#include "../App_led/app_led.h"

void Approot_Growup (void){

    osThreadNew(StartConsoleTask, NULL, &consoleTask_attributes);
    osThreadNew(StartLedTask, NULL, &ledTask_attribute);

}