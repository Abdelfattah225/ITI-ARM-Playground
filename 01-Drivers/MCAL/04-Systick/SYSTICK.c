

#include "../../../include/configuration/MCAL/SYSTICK_Cfg.h"
#include "../../../include/interface/MCAL/SYSTICK_Interface.h"
#include "../../../include/private/MCAL/SYSTICK_Private.h"

#define NULL (void*)0
static void (*SYSTICK_CallbackFunc)(void) = NULL;

/******************************************************************************
 * Function Impelementations
 ******************************************************************************/

/**
 * @brief  Initialize SysTick timer with configuration
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */

SYSTICK_ErrorStatus_t SYSTICK_Init(void) {
    // 1. Disable SysTick during configuration
    SYSTICK_Disable();
    
    // 2. Set Clock Source based on Config Macro
SYSTICK_SetClockSource(SYSTICK_CLOCK_SOURCE);
    // 3. Set Interrupt usage based on Config Macro
#if SYSTICK_INTERRUPT_MODE == SYSTICK_INT_ENABLE
    SYSTICK_EnableInterrupt();
#else
    SYSTICK_DisableInterrupt();
#endif

    return SYSTICK_OK;
}
/**
 * @brief  Set reload value for SysTick timer
 * @param  reloadValue: 24-bit reload value (0x000001 to 0xFFFFFF)
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_SetReloadValue(uint32_t reloadValue){
    if(reloadValue > 0xFFFFFF || reloadValue < 0x000001)
    {
        return SYSTICK_INVALID_RELOAD;
    }
    SYSTICK_LOAD_REG = reloadValue;
    return SYSTICK_OK;
}

/**
 * @brief  Get current reload value
 * @param  None
 * @retval uint32_t: Current reload value
 */
uint32_t SYSTICK_GetReloadValue(void){
    return (SYSTICK_LOAD_REG & SYSTICK_LOAD_RELOAD_MASK) ;
}

/**
 * @brief  Get current counter value
 * @param  None
 * @retval uint32_t: Current counter value
 */
uint32_t SYSTICK_GetCurrentValue(void){
        return (SYSTICK_VAL_REG & SYSTICK_VAL_CURRENT_MASK);
}

/**
 * @brief  Set clock source for SysTick
 * @param  clkSource: Clock source selection (SYSTICK_ClkSource_t)
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_SetClockSource(SYSTICK_ClkSource_t clkSource){
    if (clkSource == SYSTICK_CLK_AHB_DIV8 )
    {
        // clear bit
        SYSTICK_CTRL_REG &= ~(1UL<<SYSTICK_CTRL_CLKSOURCE_POS);
        return SYSTICK_OK;
    }
    else if(clkSource == SYSTICK_CLK_AHB)
    {
        SYSTICK_CTRL_REG  |= (1UL<<SYSTICK_CTRL_CLKSOURCE_POS);
        return SYSTICK_OK;

    }
    else
    {
        return SYSTICK_ERROR;
    }   
    
}

/**
 * @brief  Enable SysTick timer
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Enable(void){
    SYSTICK_CTRL_REG |= (1U<< SYSTICK_CTRL_ENABLE_POS);
    return SYSTICK_OK;
}

/**
 * @brief  Disable SysTick timer
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Disable(void){
        SYSTICK_CTRL_REG &= ~(1U<< SYSTICK_CTRL_ENABLE_POS);
        return SYSTICK_OK;
}

/**
 * @brief  Enable SysTick interrupt
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_EnableInterrupt(void){
          SYSTICK_CTRL_REG |= (1U<< SYSTICK_CTRL_TICKINT_POS);
          return SYSTICK_OK;
}

/**
 * @brief  Disable SysTick interrupt
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_DisableInterrupt(void){
        SYSTICK_CTRL_REG &= ~(1U<< SYSTICK_CTRL_TICKINT_POS);
        return SYSTICK_OK;
}

/**
 * @brief  Check if SysTick counter has counted to zero
 * @param  None
 * @retval uint8_t: 1 if counted to zero, 0 otherwise
 * @note   Reading this flag clears it automatically
 */
uint8_t SYSTICK_GetCountFlag(void){
    if (SYSTICK_CTRL_REG & (1U << SYSTICK_CTRL_COUNTFLAG_POS)) {
        return 1;
    } 
    else {
        return 0;
    }
}

/**
 * @brief  Reset current value register (restarts counting)
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Reset(void){
        SYSTICK_VAL_REG = 0;
        return SYSTICK_OK;
}

/**
 * @brief  Configure SysTick for delay in microseconds
 * @param  microSeconds: Delay time in microseconds
 * @param  ahbFreqMHz: AHB clock frequency in MHz
 * @param  clkSource: Clock source selection
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_ConfigDelay_us(uint32_t microSeconds, uint32_t ahbFreqMHz, SYSTICK_ClkSource_t clkSource){
    uint32_t reloadvalue;
   if (clkSource == SYSTICK_CLK_AHB_DIV8 )
    {
        reloadvalue = ((ahbFreqMHz*microSeconds)/8)-1;
    }
    else if(clkSource == SYSTICK_CLK_AHB)
    {
        reloadvalue = (ahbFreqMHz*microSeconds)-1;
    }
    SYSTICK_SetReloadValue(reloadvalue);
    SYSTICK_SetClockSource(clkSource);
    return SYSTICK_OK;

}
/**
 * @brief  Configure SysTick for delay in milliseconds
 * @param  milliSeconds: Delay time in milliseconds
 * @param  ahbFreqMHz: AHB clock frequency in MHz
 * @param  clkSource: Clock source selection
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_ConfigDelay_ms(uint32_t milliSeconds, uint32_t ahbFreqMHz, SYSTICK_ClkSource_t clkSource){
 uint32_t reloadvalue;
   if (clkSource == SYSTICK_CLK_AHB_DIV8 )
    {
        reloadvalue = ((ahbFreqMHz*1000*milliSeconds)/8)-1;
    }
    else if(clkSource == SYSTICK_CLK_AHB)
    {
        reloadvalue = (ahbFreqMHz*1000*milliSeconds)-1;
    }
   
    SYSTICK_SetReloadValue(reloadvalue);
    SYSTICK_SetClockSource(clkSource);

    return SYSTICK_OK;
}
/**
 * @brief  Blocking delay in microseconds
 * @param  microSeconds: Delay time in microseconds
 * @retval None
 * @note   SysTick must be configured before calling this function
 */

void SYSTICK_Delay_us(uint32_t microSeconds)
{
    uint32_t i;
    
    /* Configure for 1 microsecond tick */
    SYSTICK_ConfigDelay_us(1, SYSTICK_AHB_FREQ_MHZ, SYSTICK_CLOCK_SOURCE);
    /* Reset counter */
    SYSTICK_Reset();
    
    /* Enable SysTick */
    SYSTICK_Enable();
           
    for (i = 0; i < microSeconds; i++)
    {
 
        /* Wait until count flag is set (counted to zero) */
        while (SYSTICK_GetCountFlag() == 0);
    }
    /* Disable SysTick */
    SYSTICK_Disable();
}
/**
 * @brief  Blocking delay in milliseconds
 * @param  milliSeconds: Delay time in milliseconds
 * @retval None
 * @note   SysTick must be configured before calling this function
 */
void SYSTICK_Delay_ms(uint32_t milliSeconds){
    uint32_t i;
    /* Reset counter */
    SYSTICK_Reset();
    
    /* Enable SysTick */
    SYSTICK_Enable();

    SYSTICK_ConfigDelay_ms(1, SYSTICK_AHB_FREQ_MHZ, SYSTICK_CLOCK_SOURCE);
    for (i = 0; i < milliSeconds; i++)
    {        
        /* Wait until count flag is set (counted to zero) */
        while (SYSTICK_GetCountFlag() == 0);
        
        
    }
    /* Disable SysTick */
    SYSTICK_Disable();

}

/**
 * @brief  Set callback function for SysTick interrupt
 * @param  callbackFunc: Pointer to callback function
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */

SYSTICK_ErrorStatus_t SYSTICK_SetCallback(void (*callbackFunc)(void))
{
    if (callbackFunc != NULL)
    {
        SYSTICK_CallbackFunc = callbackFunc;
        return SYSTICK_OK;
    }
    
    return SYSTICK_ERROR;
}

/******************************************************************************
 * SysTick Interrupt Handler
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (SYSTICK_CallbackFunc != NULL)
    {
        SYSTICK_CallbackFunc();
    }
}