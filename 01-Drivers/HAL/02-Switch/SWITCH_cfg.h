#ifndef SWITCH_CFG_H
#define SWITCH_CFG_H

/// @brief you can add multiple switch names before {Switch_LEN}
// @note : then you must add switch configuraiton in SWITCH_cfg.c file
typedef enum 
{
    Emergency_Switch,
    LED_Switch,
    Switch_LEN
}Switch_names_t;

#endif
