    #include "../../../include/interface/HAL/SWITCH.h"
    #include "../../../include/configuration/HAL/SWITCH_cfg.h"
    #include "../../../include/interface/MCAL/GPIO_Interface.h"

    /* 
    * NOTE: 'const' must be removed! 
    * The driver writes to 'DebounceState' inside this array during runtime.
    */
    SWITCH_cfg_t SWITCH_cfg[Switch_LEN] = {
        [Emergency_Switch] = {
            .Port = GPIOB,
            .Pin = GPIO_PIN1,
            .resistor_cfg = Internal_PU,
            .debounce_duration_ms = 10,
            .DebounceState = {0}
        },

        [LED_Switch] = {
            .Port = GPIOB,
            .Pin = GPIO_PIN0,
            .resistor_cfg = Internal_PU,
            .debounce_duration_ms = 10,
            .DebounceState = {0}
        }
    };