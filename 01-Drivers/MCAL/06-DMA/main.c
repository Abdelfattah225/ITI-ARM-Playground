/*============================================================================
 *                              MCAL
 *============================================================================*/
#include "../include/interface/MCAL/SYSTICK_Interface.h"
#include "../include/interface/MCAL/uart_Interface.h"
#include "../include/interface/MCAL/NVIC_Interface.h"
#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"


/*============================================================================
 *                              HAL
 *============================================================================*/
#include "../include/interface/HAL/hserial_interface.h"











  /**
 * @file    main.c
 * @brief   Test application for HSerial (DMA-based UART) driver
 * @details Demonstrates TX and RX operations using DMA callbacks
 * 
 * @author  Test Application
 * @date    2025
 */

/*============================================================================
 *                              DEFINES
 *============================================================================*/
#define TEST_UART       HSERIAL_USART1
#define BAUD_RATE       115200
#define RX_BUFFER_SIZE  64

/*============================================================================
 *                          GLOBAL VARIABLES
 *============================================================================*/
static volatile uint8_t g_tx_complete = 0;
static volatile uint8_t g_rx_complete = 0;
static volatile uint8_t g_error_flag = 0;

static uint8_t g_rx_buffer[RX_BUFFER_SIZE];
static uint8_t g_tx_message[] = "Hello from STM32 via DMA!\r\n";

/*============================================================================
 *                          CALLBACK FUNCTIONS
 *============================================================================*/

/**
 * @brief TX/RX completion callback
 * @param status Result of the DMA transfer
 */
void HSerial_Callback(HSerial_Status_t status)
{
    switch (status)
    {
        case HSERIAL_TX_COMPLETE:
            g_tx_complete = 1;
            break;
            
        case HSERIAL_RX_COMPLETE:
            g_rx_complete = 1;
            break;
            
        case HSERIAL_TX_ERROR:
        case HSERIAL_RX_ERROR:
            g_error_flag = 1;
            break;
    }
}


/*============================================================================
 *                              MAIN
 *============================================================================*/
int main(void)
{
    HSerial_Error_t err;
    
    /*------------------------------------------------------------------------
     * 1. Initialize UART with DMA
     *----------------------------------------------------------------------*/
    err = HSerial_Init(TEST_UART, BAUD_RATE);
    if (err != HSERIAL_OK)
    {
        /* Init failed - halt */
        while (1);
    }
    
    /*------------------------------------------------------------------------
     * 2. Send initial message
     *----------------------------------------------------------------------*/
    g_tx_complete = 0;
    err = HSerial_Send(TEST_UART, g_tx_message, sizeof(g_tx_message) - 1, HSerial_Callback);
    
    
    /*------------------------------------------------------------------------
     * 3. Main loop: Echo received data
     *----------------------------------------------------------------------*/
    while (1)
    {
      

    }
    
    return 0;
}