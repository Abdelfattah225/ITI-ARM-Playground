#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "../include/interface/HAL/LED.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/MCAL/rcc_init.h"


extern const LED_cfg_t LED_cfg[LED_LEN];

void Toggle_LED_task1(LED_names_t led_name)
{
    while (1)
    {
        LED_Toggle(led_name);
        vTaskDelay(1000);
    }
}

void Toggle_LED_task2(LED_names_t led_name)
{
    while (1)
    {
        LED_Toggle(led_name);
        vTaskDelay(500);
    }
}

int main(void)
{
    RCC_Init();
    LED_init();

    xTaskHandle ToggleTask;
    xTaskCreate(Toggle_LED_task1, "task1", 100, LED_DOOROPEN, 1, &ToggleTask);
    xTaskCreate(Toggle_LED_task2, "task2", 100, LED_WARNING, 2, &ToggleTask);

    vTaskStartScheduler();

    return 0;
}

// what you want to do in idel
void vApplicationIdleHook(void)
{
}

// is you need every tick something happen ?
void vApplicationTickHook(void)
{
}

// if stack overflow happen what should I do ?
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName)
{
    while (1)
    {
    }
}
// if i cant create stack dyanmically what should I Do ?
void vApplicationMallocFailedHook()
{
    while (1)
    {
        /* code */
    }
}