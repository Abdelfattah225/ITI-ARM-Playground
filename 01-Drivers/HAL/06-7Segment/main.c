/*============================================================================
 *                              MCAL
 *============================================================================*/
#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"


/*============================================================================
 *                              HAL
 *============================================================================*/
#include "../include/interface/HAL/7seg.h"













  /**
 * @file    main.c
 * @brief   Test application for HSerial (DMA-based UART) driver
 * @details Demonstrates TX and RX operations using DMA callbacks
 * 
 * @author  ABDO
 * @date    2025
 */



extern Seven_SEG_cfg_t my7Seg_cfg;
 int main(void)
 {
    RCC_Init();
    SYSTICK_Init();
    Seven_SEG_Init(&my7Seg_cfg);


    while (1)
    {
        for(int i = 0 ; i< 10 ; i++ )
        {
            Seven_SEG_DisplayDigit(&my7Seg_cfg,i);
            SYSTICK_Delay_ms(1000);
       

    }}
    

 }