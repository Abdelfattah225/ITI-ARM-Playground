#include "../../../include/interface/HAL/SWITCH.h"
#include "../../../include/configuration/HAL/SWITCH_cfg.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"

const SWITCH_cfg_t SWITCH_cfg[Switch_LEN]={
[Emergency_Switch]={
    .Port = GPIOA,
    .Pin = GPIO_PIN13,
    .resistor_cfg = Internal_PU,
},
[LED_Switch]={
    .Port = GPIOA,
    .Pin = GPIO_PIN14,
    .resistor_cfg = Internal_PU,
},

};

