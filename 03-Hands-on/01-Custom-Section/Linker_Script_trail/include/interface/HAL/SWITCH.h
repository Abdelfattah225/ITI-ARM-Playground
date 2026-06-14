#ifndef SWITCH_H
#define SWITCH_H
#include "../../configuration/HAL/SWITCH_cfg.h"
#include "../MCAL/GPIO_Interface.h"
typedef struct 
{
    /* data */
    void * Port;
    GPIO_Pin_t Pin;
    Switch_cfg_t resistor_cfg;
}SWITCH_cfg_t;

typedef enum
{
    SWITCH_Ok,
    SWITCH_NOT_NULL_PTR,
    SWITCH_INVALID
}swtich_status_t;


swtich_status_t Switch_Init();

swtich_status_t Switch_ReadState();

#endif
