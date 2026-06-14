#include "../../../include/interface/HAL/LED.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"

GPIO_PinConfig_t Pin_cfg[LED_LEN];
extern  LED_cfg_t LED_cfg[LED_LEN];

void LED_init(){
    for(uint8_t i = 0 ; i< LED_LEN ; i++)
    {
       Pin_cfg[i].Port = LED_cfg[i].port;
       Pin_cfg[i].Pin  = LED_cfg[i].pin ;
        Pin_cfg[i].OutputType = LED_cfg[i].IsPP ;
       Pin_cfg[i].PuPd = GPIO_PUPD_NONE;
       Pin_cfg[i].AlternateFunc = GPIO_AF0_SYSTEM ;
       Pin_cfg[i].Mode = GPIO_MODE_OUTPUT;
       Pin_cfg[i].Speed = GPIO_SPEED_HIGH;
        GPIO_PinInit(Pin_cfg[i].Port, &Pin_cfg[i]);
    };
    
    GPIO_WritePin(LED_cfg[LED_LEN-1].port, LED_cfg[LED_LEN-1].pin, GPIO_PIN_RESET);

}

void LED_TrunON(LED_names_t LED_name){
    if(LED_cfg[LED_name].active_state == ACTIVE_HIGH)
    {
        GPIO_WritePin(LED_cfg[LED_name].port, LED_cfg[LED_name].pin, GPIO_PIN_SET);
    }
    else
    {
        GPIO_WritePin(LED_cfg[LED_name].port, LED_cfg[LED_name].pin, GPIO_PIN_RESET);
    }
}

void LED_TrunOFF(LED_names_t LED_name){
  if(LED_cfg[LED_name].active_state == ACTIVE_LOW)
    {
        GPIO_WritePin(LED_cfg[LED_name].port, LED_cfg[LED_name].pin, GPIO_PIN_SET);
    }
    else
    {
        GPIO_WritePin(LED_cfg[LED_name].port, LED_cfg[LED_name].pin, GPIO_PIN_RESET);
    }
}

void LED_Toggle(LED_names_t LED_name){
    GPIO_TogglePin(LED_cfg[LED_name].port, LED_cfg[LED_name].pin);
}
