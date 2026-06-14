// #include "../include/interface/MCAL/rcc_init.h"
// #include "../include/interface/MCAL/GPIO_Interface.h"
// #include "../include/interface/HAL/LED.h"
// #include "../include/interface/HAL/LCD.h"
// #include "../include/interface/MCAL/SYSTICK_Interface.h"
// #include "./os/sched.h"




#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/HAL/LED.h"
#include "../include/interface/HAL/SWITCH.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"

/* 
 * IMPORTANT: SWITCH_cfg cannot be const, 
 * because the main loop updates the debounce counters inside it.
 */
extern SWITCH_cfg_t SWITCH_cfg[Switch_LEN]; 
extern const LED_cfg_t LED_cfg[LED_LEN];

int main(void)
{
    /* Initialize System Clock first (Crucial for GPIO to work) */
    RCC_Init(); // Uncomment if you have this function
    SYSTICK_Init();
        /* Initialize Peripherals */
    LED_init();
    Switch_Init();

    while (1)
    {
        /* 
         * 1. Update the state 
         * This calculates the debounce logic and updates the struct internally.
         */
        Switch_ReadState_Blocking(&SWITCH_cfg[LED_Switch]);

        /* 
         * 2. Read the result from the Struct
         * We check .DebounceState.current_logic_state, NOT the return value of the function.
         */
        if(SWITCH_cfg[LED_Switch].DebounceState.current_logic_state == PRESSED)
        {
             /* If button is pressed, turn LED OFF */
             LED_TrunOFF(LED_WARNING);
        }
        else
        {
             /* If button is NOT pressed, turn LED ON */
             LED_TrunON(LED_WARNING);
        }
        
        /* 
         * Optional: Add a small delay here if your CPU runs very fast,
         * otherwise the debounce counter increments too quickly.
         * For example: delay_ms(1); 
         */
    }
    
    return 0;
}
