#ifndef SWITCH_CFG_H
#define SWITCH_CFG_H


typedef enum
{
    Internal_PU,
    External_PU,
    Internal_PD,
    External_PD,
    floating
}Switch_cfg_t;


typedef enum 
{
    Emergency_Switch,
    LED_Switch,
    Switch_LEN
}Switch_names_t;

typedef enum{
    LOW,
    HIGH
}Button_State_t;


typedef enum{
    NOTPRESSED,
    PRESSED
}Resturn_Button_State_t;

#endif