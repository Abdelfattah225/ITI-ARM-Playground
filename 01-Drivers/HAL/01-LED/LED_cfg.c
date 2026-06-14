
#include "../../../include/interface/HAL/LED.h"
#include "../../../include/configuration/HAL/LED_cfg.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"




const LED_cfg_t LED_cfg[LED_LEN]={

[LED_WARNING]={
    .active_state = ACTIVE_HIGH,
    .port= GPIOA,
    .pin= GPIO_PIN0,
    .IsPP = PUSH_PULL,
},

[LED_DOOROPEN]={
    .active_state = ACTIVE_HIGH,
    .port= GPIOA,
    .pin= GPIO_PIN1,
    .IsPP = PUSH_PULL,
},


[LED_ERROR]={
    .active_state = ACTIVE_HIGH,
    .port= GPIOA,
    .pin= GPIO_PIN2,
    .IsPP = PUSH_PULL,
},
[GND_PIN]={
    .active_state = ACTIVE_LOW,
    .port= GPIOA,
    .pin= GPIO_PIN8,
    .IsPP = PUSH_PULL,
}

};