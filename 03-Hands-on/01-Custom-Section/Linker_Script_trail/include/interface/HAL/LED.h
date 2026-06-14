
#ifndef LED_H
#define LED_H

#include <stdlib.h>
#include <stdint.h>
#include "../../configuration/HAL/LED_cfg.h"
#include "../MCAL/GPIO_Interface.h"

typedef struct 
{
    LED_active_state_t active_state;
    void * port;
    GPIO_Pin_t pin;
    LED_IsPP_t IsPP; // push pull / open drain

}LED_cfg_t;


// loop on arr and initialize GPIO
void LED_init();

void LED_TrunON(LED_names_t LED_name);
void LED_TrunOFF(LED_names_t LED_name);
void LED_Toggle(LED_names_t LED_name);

#endif