/******************************************************************************
 * @file    RCC.c
 * @brief   RCC Driver Implementation for STM32F401CC
 * @author  Abdelfattah moawed & commented by AI
 * @date    2025
 *****************************************************************************/

#include "../../../include/interface/MCAL/rcc_init.h"
#include "../../../include/private/MCAL/rcc_prv.h"
#include "../../../include/configuration/MCAL/rcc_cfg.h"


#include <stdio.h>

/******************************************************************************
 * Private Helper Functions
 *****************************************************************************/

/**
 * @brief  Configure Flash latency based on system clock
 * @param  Latency: Number of wait states (0-2)
 * @retval None
 */
static void RCC_SetFlashLatency(uint8_t Latency)
{
    volatile uint32_t *FLASH_ACR = (volatile uint32_t *)0x40023C00;
    
    /* Clear latency bits and set new value */
    *FLASH_ACR = (*FLASH_ACR & ~0x0F) | (Latency & 0x0F);
}

/******************************************************************************
 * System Clock Configuration Functions
 *****************************************************************************/

RCC_Status_t RCC_Init(void)
{
    RCC_Status_t status = RCC_OK;
    
    /* Step 1: Enable HSI (already enabled at startup, but ensure it) */
    status = RCC_HSI_Enable();
    if (status != RCC_OK) return status;
    
#if (RCC_CLOCK_SOURCE == 1) || ((RCC_CLOCK_SOURCE == 2) && (RCC_PLL_SOURCE == 1))
    /* Step 2: Enable HSE if needed */
    #if (RCC_HSE_ENABLE == 1)
        status = RCC_HSE_Enable(RCC_HSE_BYPASS);
        if (status != RCC_OK) return status;
    #endif
#endif

#if (RCC_CLOCK_SOURCE == 2)
    /* Step 3: Configure and enable PLL */
    RCC_PLL_Config_t pllConfig = {
        .PLL_Source = RCC_PLL_SOURCE,
        .PLLM = RCC_PLLM_VALUE,
        .PLLN = RCC_PLLN_VALUE,
        .PLLP = RCC_PLLP_VALUE,
        .PLLQ = RCC_PLLQ_VALUE
    };
    
    status = RCC_PLL_Config(&pllConfig);
    if (status != RCC_OK) return status;
    
    status = RCC_PLL_Enable();
    if (status != RCC_OK) return status;
#endif

    /* Step 4: Configure Flash latency before increasing frequency */
    RCC_SetFlashLatency(RCC_FLASH_LATENCY);
    
    /* Step 5: Configure bus prescalers */
    RCC_SetAHBPrescaler(RCC_AHB_PRESCALER);
    RCC_SetAPB1Prescaler(RCC_APB1_PRESCALER);
    RCC_SetAPB2Prescaler(RCC_APB2_PRESCALER);
    
    /* Step 6: Switch to configured clock source */
    status = RCC_SetSystemClock(RCC_CLOCK_SOURCE);
    if (status != RCC_OK) return status;
    
#if (RCC_CSS_ENABLE == 1)
    /* Step 7: Enable Clock Security System if configured */
    RCC_CSS_Enable();
#endif

#if (RCC_LSE_ENABLE == 1)
    /* Step 8: Enable LSE if configured */
    status = RCC_LSE_Enable();
#endif

#if (RCC_LSI_ENABLE == 1)
    /* Step 9: Enable LSI if configured */
    status = RCC_LSI_Enable();
#endif

#if (RCC_RTC_ENABLE == 1)
    /* Step 10: Configure RTC clock if enabled */
    RCC_RTC_SetClockSource(RCC_RTC_CLOCK_SOURCE);
    RCC_RTC_EnableClock();
#endif

#if (RCC_MCO1_ENABLE == 1)
    /* Step 11: Configure MCO1 if enabled */
    RCC_MCO1_Config(RCC_MCO1_SOURCE, RCC_MCO1_PRESCALER);
#endif

#if (RCC_MCO2_ENABLE == 1)
    /* Step 12: Configure MCO2 if enabled */
    RCC_MCO2_Config(RCC_MCO2_SOURCE, RCC_MCO2_PRESCALER);
#endif
    
    return RCC_OK;
}

RCC_Status_t RCC_HSI_Enable(void)
{
    uint32_t timeout = RCC_HSI_TIMEOUT;
    
    /* Enable HSI */
    RCC->CR.BIT.HSION = 1;
    
    /* Wait until HSI is ready */
    while ((RCC->CR.BIT.HSIRDY == 0) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_Status_t RCC_HSI_Disable(void)
{
    /* Check if HSI is currently the system clock */
    if (RCC->CFGR.BIT.SWS == 0)
    {
        return RCC_ERROR; /* Cannot disable HSI when it's the system clock */
    }
    
    /* Disable HSI */
    RCC->CR.BIT.HSION = 0;
    
    return RCC_OK;
}

RCC_Status_t RCC_HSE_Enable(RCC_HSE_Mode_t Mode)
{
    uint32_t timeout = RCC_HSE_TIMEOUT;
    
    /* Configure bypass mode if needed */
    if (Mode == RCC_HSE_BYPASS)
    {
        RCC->CR.BIT.HSEBYP = 1;
    }
    else
    {
        RCC->CR.BIT.HSEBYP = 0;
    }
    
    /* Enable HSE */
    RCC->CR.BIT.HSEON = 1;
    
    /* Wait until HSE is ready */
    while ((RCC->CR.BIT.HSERDY == 0) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_Status_t RCC_HSE_Disable(void)
{
    /* Check if HSE is currently the system clock or PLL source */
    if (RCC->CFGR.BIT.SWS == 1)
    {
        return RCC_ERROR; /* Cannot disable HSE when it's the system clock */
    }
    
    if ((RCC->CFGR.BIT.SWS == 2) && (RCC->PLLCFGR.BIT.PLLSRC == 1))
    {
        return RCC_ERROR; /* Cannot disable HSE when PLL is system clock and uses HSE */
    }
    
    /* Disable HSE */
    RCC->CR.BIT.HSEON = 0;
    
    return RCC_OK;
}

RCC_Status_t RCC_PLL_Config(const RCC_PLL_Config_t* Config)
{
    /* Validate input parameters */
    if (Config == NULL)
    {
        return RCC_INVALID_PARAM;
    }
    
    if ((Config->PLLM < 2) || (Config->PLLM > 63))
    {
        return RCC_INVALID_PARAM;
    }
    
    if ((Config->PLLN < 50) || (Config->PLLN > 432))
    {
        return RCC_INVALID_PARAM;
    }
    
    if ((Config->PLLQ < 2) || (Config->PLLQ > 15))
    {
        return RCC_INVALID_PARAM;
    }
    
    /* PLL must be disabled before configuration */
    if (RCC->CR.BIT.PLLON == 1)
    {
        return RCC_ERROR;
    }
    
    /* Configure PLL */
    RCC->PLLCFGR.BIT.PLLSRC = Config->PLL_Source;
    RCC->PLLCFGR.BIT.PLLM = Config->PLLM;
    RCC->PLLCFGR.BIT.PLLN = Config->PLLN;
    RCC->PLLCFGR.BIT.PLLP = Config->PLLP;
    RCC->PLLCFGR.BIT.PLLQ = Config->PLLQ;
    
    return RCC_OK;
}

RCC_Status_t RCC_PLL_Enable(void)
{
    uint32_t timeout = RCC_PLL_TIMEOUT;
    
    /* Enable PLL */
    RCC->CR.BIT.PLLON = 1;
    
    /* Wait until PLL is ready */
    while ((RCC->CR.BIT.PLLRDY == 0) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_Status_t RCC_PLL_Disable(void)
{
    /* Check if PLL is currently the system clock */
    if (RCC->CFGR.BIT.SWS == 2)
    {
        return RCC_ERROR; /* Cannot disable PLL when it's the system clock */
    }
    
    /* Disable PLL */
    RCC->CR.BIT.PLLON = 0;
    
    return RCC_OK;
}

RCC_Status_t RCC_SetSystemClock(RCC_SysClk_t ClkSource)
{
    uint32_t timeout = 5000;
    
    /* Validate clock source is ready */
    switch (ClkSource)
    {
        case RCC_SYSCLK_HSI:
            if (RCC->CR.BIT.HSIRDY == 0) return RCC_ERROR;
            break;
            
        case RCC_SYSCLK_HSE:
            if (RCC->CR.BIT.HSERDY == 0) return RCC_ERROR;
            break;
            
        case RCC_SYSCLK_PLL:
            if (RCC->CR.BIT.PLLRDY == 0) return RCC_ERROR;
            break;
            
        default:
            return RCC_INVALID_PARAM;
    }
    
    /* Set system clock switch */
    RCC->CFGR.BIT.SW = ClkSource;
    
    /* Wait until system clock switch is complete */
    while ((RCC->CFGR.BIT.SWS != ClkSource) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_SysClk_t RCC_GetSystemClock(void)
{
    return (RCC_SysClk_t)RCC->CFGR.BIT.SWS;
}

RCC_ClkStatus_t RCC_GetClockStatus(RCC_SysClk_t ClkSource)
{
    switch (ClkSource)
    {
        case RCC_SYSCLK_HSI:
            return (RCC_ClkStatus_t)RCC->CR.BIT.HSIRDY;
            
        case RCC_SYSCLK_HSE:
            return (RCC_ClkStatus_t)RCC->CR.BIT.HSERDY;
            
        case RCC_SYSCLK_PLL:
            return (RCC_ClkStatus_t)RCC->CR.BIT.PLLRDY;
            
        default:
            return RCC_NOT_READY;
    }
}

/******************************************************************************
 * Bus Prescalers Functions
 *****************************************************************************/

RCC_Status_t RCC_SetAHBPrescaler(RCC_AHB_Prescaler_t Prescaler)
{
    /* Clear HPRE bits and set new value */
    RCC->CFGR.BIT.HPRE = Prescaler;
    
    return RCC_OK;
}

RCC_Status_t RCC_SetAPB1Prescaler(RCC_APB_Prescaler_t Prescaler)
{
    /* Clear PPRE1 bits and set new value */
    RCC->CFGR.BIT.PPRE1 = Prescaler;
    
    return RCC_OK;
}

RCC_Status_t RCC_SetAPB2Prescaler(RCC_APB_Prescaler_t Prescaler)
{
    /* Clear PPRE2 bits and set new value */
    RCC->CFGR.BIT.PPRE2 = Prescaler;
    
    return RCC_OK;
}

/******************************************************************************
 * Peripheral Clock Control Functions - AHB1
 *****************************************************************************/

void RCC_AHB1_EnableClock(RCC_AHB1_Peripheral_t Peripheral)
{
    RCC->AHB1ENR.REG |= (1U << Peripheral);
}

void RCC_AHB1_DisableClock(RCC_AHB1_Peripheral_t Peripheral)
{
    RCC->AHB1ENR.REG &= ~(1U << Peripheral);
}

void RCC_AHB1_ResetPeripheral(RCC_AHB1_Peripheral_t Peripheral)
{
    /* Assert reset */
    RCC->AHB1RSTR.REG |= (1U << Peripheral);
    
    /* Deassert reset */
    RCC->AHB1RSTR.REG &= ~(1U << Peripheral);
}

/******************************************************************************
 * Peripheral Clock Control Functions - AHB2
 *****************************************************************************/

void RCC_AHB2_EnableClock(RCC_AHB2_Peripheral_t Peripheral)
{
    RCC->AHB2ENR.REG |= (1U << Peripheral);
}

void RCC_AHB2_DisableClock(RCC_AHB2_Peripheral_t Peripheral)
{
    RCC->AHB2ENR.REG &= ~(1U << Peripheral);
}

void RCC_AHB2_ResetPeripheral(RCC_AHB2_Peripheral_t Peripheral)
{
    /* Assert reset */
    RCC->AHB2RSTR.REG |= (1U << Peripheral);
    
    /* Deassert reset */
    RCC->AHB2RSTR.REG &= ~(1U << Peripheral);
}

/******************************************************************************
 * Peripheral Clock Control Functions - APB1
 *****************************************************************************/

void RCC_APB1_EnableClock(RCC_APB1_Peripheral_t Peripheral)
{
    RCC->APB1ENR.REG |= (1U << Peripheral);
}

void RCC_APB1_DisableClock(RCC_APB1_Peripheral_t Peripheral)
{
    RCC->APB1ENR.REG &= ~(1U << Peripheral);
}

void RCC_APB1_ResetPeripheral(RCC_APB1_Peripheral_t Peripheral)
{
    /* Assert reset */
    RCC->APB1RSTR.REG |= (1U << Peripheral);
    
    /* Deassert reset */
    RCC->APB1RSTR.REG &= ~(1U << Peripheral);
}

/******************************************************************************
 * Peripheral Clock Control Functions - APB2
 *****************************************************************************/

void RCC_APB2_EnableClock(RCC_APB2_Peripheral_t Peripheral)
{
    RCC->APB2ENR.REG |= (1U << Peripheral);
}

void RCC_APB2_DisableClock(RCC_APB2_Peripheral_t Peripheral)
{
    RCC->APB2ENR.REG &= ~(1U << Peripheral);
}

void RCC_APB2_ResetPeripheral(RCC_APB2_Peripheral_t Peripheral)
{
    /* Assert reset */
    RCC->APB2RSTR.REG |= (1U << Peripheral);
    
    /* Deassert reset */
    RCC->APB2RSTR.REG &= ~(1U << Peripheral);
}

/******************************************************************************
 * Clock Frequency Information Functions
 *****************************************************************************/

uint32_t RCC_GetSYSCLKFrequency(void)
{
    uint32_t sysclk = 0;
    uint32_t pllm, plln, pllp, pllsrc;
    
    /* Determine system clock source */
    switch (RCC->CFGR.BIT.SWS)
    {
        case 0: /* HSI */
            sysclk = RCC_HSI_FREQ;
            break;
            
        case 1: /* HSE */
            sysclk = RCC_HSE_FREQ;
            break;
            
        case 2: /* PLL */
            pllsrc = RCC->PLLCFGR.BIT.PLLSRC;
            pllm = RCC->PLLCFGR.BIT.PLLM;
            plln = RCC->PLLCFGR.BIT.PLLN;
            pllp = RCC->PLLCFGR.BIT.PLLP;
            
            if (pllsrc == 0)
            {
                /* PLL source is HSI */
                sysclk = (RCC_HSI_FREQ / pllm) * plln;
            }
            else
            {
                /* PLL source is HSE */
                sysclk = (RCC_HSE_FREQ / pllm) * plln;
            }
            
            /* Apply PLLP divider (PLLP = 2, 4, 6, or 8) */
            sysclk = sysclk / (2 * (pllp + 1));
            break;
            
        default:
            sysclk = RCC_HSI_FREQ;
            break;
    }
    
    return sysclk;
}

uint32_t RCC_GetHCLKFrequency(void)
{
    uint32_t sysclk = RCC_GetSYSCLKFrequency();
    uint32_t hpre = RCC->CFGR.BIT.HPRE;
    uint32_t ahb_prescaler;
    
    /* Determine AHB prescaler */
    if (hpre < 8)
    {
        ahb_prescaler = 1;
    }
    else
    {
        ahb_prescaler = 1 << (hpre - 7);
    }
    
    return (sysclk / ahb_prescaler);
}

uint32_t RCC_GetPCLK1Frequency(void)
{
    uint32_t hclk = RCC_GetHCLKFrequency();
    uint32_t ppre1 = RCC->CFGR.BIT.PPRE1;
    uint32_t apb1_prescaler;
    
    /* Determine APB1 prescaler */
    if (ppre1 < 4)
    {
        apb1_prescaler = 1;
    }
    else
    {
        apb1_prescaler = 1 << (ppre1 - 3);
    }
    
    return (hclk / apb1_prescaler);
}

uint32_t RCC_GetPCLK2Frequency(void)
{
    uint32_t hclk = RCC_GetHCLKFrequency();
    uint32_t ppre2 = RCC->CFGR.BIT.PPRE2;
    uint32_t apb2_prescaler;
    
    /* Determine APB2 prescaler */
    if (ppre2 < 4)
    {
        apb2_prescaler = 1;
    }
    else
    {
        apb2_prescaler = 1 << (ppre2 - 3);
    }
    
    return (hclk / apb2_prescaler);
}

uint32_t RCC_GetTIM_APB1_Frequency(void)
{
    uint32_t ppre1 = RCC->CFGR.BIT.PPRE1;
    uint32_t pclk1 = RCC_GetPCLK1Frequency();
    
    /* If APB1 prescaler is 1, timer clock = APB1 clock
     * Otherwise, timer clock = 2 × APB1 clock */
    if (ppre1 < 4)
    {
        return pclk1;
    }
    else
    {
        return (pclk1 * 2);
    }
}

uint32_t RCC_GetTIM_APB2_Frequency(void)
{
    uint32_t ppre2 = RCC->CFGR.BIT.PPRE2;
    uint32_t pclk2 = RCC_GetPCLK2Frequency();
    
    /* If APB2 prescaler is 1, timer clock = APB2 clock
     * Otherwise, timer clock = 2 × APB2 clock */
    if (ppre2 < 4)
    {
        return pclk2;
    }
    else
    {
        return (pclk2 * 2);
    }
}

/******************************************************************************
 * CSS and Security Functions
 *****************************************************************************/

void RCC_CSS_Enable(void)
{
    RCC->CR.BIT.CSSON = 1;
}

void RCC_CSS_Disable(void)
{
    RCC->CR.BIT.CSSON = 0;
}

/******************************************************************************
 * MCO Functions
 *****************************************************************************/

RCC_Status_t RCC_MCO1_Config(uint8_t ClkSource, uint8_t Prescaler)
{
    if (ClkSource > 3)
    {
        return RCC_INVALID_PARAM;
    }
    
    /* Configure MCO1 source and prescaler */
    RCC->CFGR.BIT.MCO1 = ClkSource;
    RCC->CFGR.BIT.MCO1PRE = Prescaler;
    
    return RCC_OK;
}

RCC_Status_t RCC_MCO2_Config(uint8_t ClkSource, uint8_t Prescaler)
{
    if (ClkSource > 3)
    {
        return RCC_INVALID_PARAM;
    }
    
    /* Configure MCO2 source and prescaler */
    RCC->CFGR.BIT.MCO2 = ClkSource;
    RCC->CFGR.BIT.MCO2PRE = Prescaler;
    
    return RCC_OK;
}

/******************************************************************************
 * LSE and RTC Functions
 *****************************************************************************/

RCC_Status_t RCC_LSE_Enable(void)
{
    uint32_t timeout = RCC_LSE_TIMEOUT;
    
    /* Enable LSE */
    RCC->BDCR.BIT.LSEON = 1;
    
    /* Wait until LSE is ready */
    while ((RCC->BDCR.BIT.LSERDY == 0) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_Status_t RCC_LSE_Disable(void)
{
    /* Disable LSE */
    RCC->BDCR.BIT.LSEON = 0;
    
    return RCC_OK;
}

RCC_Status_t RCC_LSI_Enable(void)
{
    uint32_t timeout = RCC_LSI_TIMEOUT;
    
    /* Enable LSI */
    RCC->CSR.BIT.LSION = 1;
    
    /* Wait until LSI is ready */
    while ((RCC->CSR.BIT.LSIRDY == 0) && (timeout > 0))
    {
        timeout--;
    }
    
    return (timeout > 0) ? RCC_OK : RCC_TIMEOUT;
}

RCC_Status_t RCC_LSI_Disable(void)
{
    /* Disable LSI */
    RCC->CSR.BIT.LSION = 0;
    
    return RCC_OK;
}

RCC_Status_t RCC_RTC_SetClockSource(uint8_t ClkSource)
{
    if (ClkSource > 3)
    {
        return RCC_INVALID_PARAM;
    }
    
    /* Set RTC clock source */
    RCC->BDCR.BIT.RTCSEL = ClkSource;
    
    return RCC_OK;
}

void RCC_RTC_EnableClock(void)
{
    RCC->BDCR.BIT.RTCEN = 1;
}

void RCC_RTC_DisableClock(void)
{
    RCC->BDCR.BIT.RTCEN = 0;
}

/******************************************************************************
 * Backup Domain and Reset Flags Functions
 *****************************************************************************/

void RCC_BackupDomain_Reset(void)
{
    /* Assert backup domain reset */
    RCC->BDCR.BIT.BDRST = 1;
    
    /* Deassert backup domain reset */
    RCC->BDCR.BIT.BDRST = 0;
}

void RCC_ClearResetFlags(void)
{
    RCC->CSR.BIT.RMVF = 1;
}

uint32_t RCC_GetResetFlags(void)
{
    /* Return reset flags (bits 25-31 of CSR) */
    return ((RCC->CSR.REG >> 24) & 0xFF);
}

/******************************************************************************
 * End of File
 *****************************************************************************/