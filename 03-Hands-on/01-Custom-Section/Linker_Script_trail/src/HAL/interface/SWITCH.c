#include "../../../include/interface/HAL/SWITCH.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"
GPIO_PinConfig_t swtich_pin_GPIO[Switch_LEN];
extern const SWITCH_cfg_t SWITCH_cfg[Switch_LEN];

swtich_status_t Switch_Init()
{

    for (uint8_t i = 0; i < Switch_LEN; i++)
    {
        swtich_pin_GPIO[i].Port = SWITCH_cfg[i].Port;
        swtich_pin_GPIO[i].Pin = SWITCH_cfg[i].Pin;
        swtich_pin_GPIO[i].OutputType = GPIO_OUTPUT_PUSHPULL;
        swtich_pin_GPIO[i].AlternateFunc = GPIO_AF0_SYSTEM;
        swtich_pin_GPIO[i].Mode = GPIO_MODE_INTPUT;
        swtich_pin_GPIO[i].Speed = GPIO_SPEED_HIGH;
        if ((SWITCH_cfg[i].resistor_cfg == Internal_PD))
        {
            swtich_pin_GPIO[i].PuPd = GPIO_PUPD_PULLUP;
        }
        else if ((SWITCH_cfg[i].resistor_cfg == Internal_PU))
        {
            swtich_pin_GPIO[i].PuPd = GPIO_PUPD_PULLDOWN;
        }
        else
        {
            swtich_pin_GPIO[i].PuPd = GPIO_PUPD_NONE;
        }
        GPIO_PinInit(swtich_pin_GPIO[i].Port, &SWITCH_cfg[i]);
    };
}

swtich_status_t Switch_ReadState(SWITCH_cfg_t *SWITHCx)
{

    uint32_t read_value = GPIO_ReadPin(SWITHCx->Port, SWITHCx->Pin);

    if ((SWITHCx->resistor_cfg == Internal_PU) || (SWITHCx->resistor_cfg == External_PU))
    {
        // if pressed
        if (read_value == LOW)
        {
            return PRESSED;
        }
        else
        {
            return NOTPRESSED;
        }
    }
    else if ((SWITHCx->resistor_cfg == Internal_PD) || (SWITHCx->resistor_cfg == External_PD))
    {
        // if pressed
        if (read_value == HIGH)
        {
            return PRESSED;
        }
        else
        {
            return NOTPRESSED;
        }
    }
}