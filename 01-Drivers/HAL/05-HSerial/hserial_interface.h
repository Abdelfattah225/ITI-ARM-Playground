/**
 * @file    hserial_interface.h
 * @brief   High-level Serial (UART) HAL Driver Interface
 * @details This driver provides an abstraction layer over UART peripherals
 *          using DMA for efficient non-blocking data transfers.
 *          Supports USART1, USART2, and USART6 on STM32F4xx.
 * 
 * @author  [Your Name]
 * @date    [Date]
 * @version 1.0
 * 
 * @note    Target: STM32F401xC/xE
 * 
 * @example
 *          // Initialize USART1 at 115200 baud
 *          HSerial_Init(HSERIAL_USART1, 115200);
 *          
 *          // Send data with callback
 *          uint8_t tx_buffer[] = "Hello World!";
 *          HSerial_Send(HSERIAL_USART1, tx_buffer, sizeof(tx_buffer), MyTxCallback);
 *          
 *          // Receive data with callback
 *          uint8_t rx_buffer[64];
 *          HSerial_Receive(HSERIAL_USART1, rx_buffer, 64, MyRxCallback);
 */

#ifndef HSERIAL_INTERFACE_H
#define HSERIAL_INTERFACE_H

#include <stdint.h>

/*============================================================================
 *                              ENUMS
 *============================================================================*/

/**
 * @brief Available UART instances
 * @note  Pin mapping (default configuration):
 *        - USART1: TX=PA9,  RX=PA10 (APB2, AF7)
 *        - USART2: TX=PA2,  RX=PA3  (APB1, AF7)
 *        - USART6: TX=PA11, RX=PA12 (APB2, AF8)
 */
typedef enum {
    HSERIAL_USART1,     /**< USART1 instance */
    HSERIAL_USART2,     /**< USART2 instance */
    HSERIAL_USART6,     /**< USART6 instance */
    HSERIAL_COUNT       /**< Total number of instances (for array sizing) */
} HSerial_Instance_t;

/**
 * @brief Error codes returned by HSerial functions
 */
typedef enum {
    HSERIAL_OK,             /**< Operation successful */
    HSERIAL_ERROR,          /**< General error (DMA init failed, etc.) */
    HSERIAL_NULLPTR,        /**< NULL pointer passed as argument */
    HSERIAL_BUSY,           /**< UART is busy with ongoing transfer */
    HSERIAL_INVALID_PARAM   /**< Invalid parameter (wrong UART instance, etc.) */
} HSerial_Error_t;

/**
 * @brief Status codes passed to user callback
 * @note  These indicate the result of an asynchronous operation
 */
typedef enum {
    HSERIAL_TX_COMPLETE,    /**< Transmission completed successfully */
    HSERIAL_RX_COMPLETE,    /**< Reception completed successfully */
    HSERIAL_TX_ERROR,       /**< Transmission failed (DMA error) */
    HSERIAL_RX_ERROR        /**< Reception failed (DMA error) */
} HSerial_Status_t;

/*============================================================================
 *                              CALLBACK TYPE
 *============================================================================*/

/**
 * @brief User callback function type
 * @param status The result of the completed operation
 * 
 * @note  This callback is called from ISR context (DMA interrupt).
 *        Keep the callback short and avoid blocking operations.
 * 
 * @example
 *        void MyCallback(HSerial_Status_t status) {
 *            if (status == HSERIAL_TX_COMPLETE) {
 *                // Handle successful transmission
 *            }
 *        }
 */
typedef void (*HSerial_Callback_t)(HSerial_Status_t status);

/*============================================================================
 *                          FUNCTION PROTOTYPES
 *============================================================================*/

/**
 * @brief  Initialize a UART instance with specified baud rate
 * 
 * @param  uart      Which UART to initialize (HSERIAL_USART1/2/6)
 * @param  baudrate  Desired baud rate (e.g., 9600, 115200, 921600)
 * 
 * @return HSERIAL_OK              Success
 * @return HSERIAL_INVALID_PARAM   Invalid UART instance
 * 
 * @note   Configures: 8 data bits, no parity, 1 stop bit (8N1)
 * @note   Enables DMA for both TX and RX
 * @note   GPIO pins are configured automatically
 * 
 * @pre    System clocks must be configured before calling
 */
HSerial_Error_t HSerial_Init(HSerial_Instance_t uart, uint32_t baudrate);

/**
 * @brief  Send data asynchronously via DMA
 * 
 * @param  uart      Which UART to use
 * @param  buffer    Pointer to data buffer to transmit
 * @param  size      Number of bytes to send
 * @param  callback  Function to call when transfer completes
 * 
 * @return HSERIAL_OK        Transfer started successfully
 * @return HSERIAL_NULLPTR   buffer or callback is NULL
 * @return HSERIAL_BUSY      Previous transfer still in progress
 * @return HSERIAL_ERROR     DMA initialization failed
 * 
 * @warning Buffer must remain valid until callback is called!
 *          Do not modify or free the buffer during transfer.
 * 
 * @note   This function returns immediately (non-blocking).
 *         The callback is called when transfer completes.
 */
HSerial_Error_t HSerial_Send(HSerial_Instance_t uart, uint8_t *buffer, uint16_t size, HSerial_Callback_t callback);

/**
 * @brief  Receive data asynchronously via DMA
 * 
 * @param  uart      Which UART to use
 * @param  buffer    Pointer to buffer where received data will be stored
 * @param  size      Number of bytes to receive
 * @param  callback  Function to call when transfer completes
 * 
 * @return HSERIAL_OK        Reception started successfully
 * @return HSERIAL_NULLPTR   buffer or callback is NULL
 * @return HSERIAL_BUSY      Previous reception still in progress
 * @return HSERIAL_ERROR     DMA initialization failed
 * 
 * @warning Buffer must remain valid until callback is called!
 * 
 * @note   Callback is called only when ALL requested bytes are received.
 *         For variable-length reception, consider using IDLE line detection.
 */
HSerial_Error_t HSerial_Receive(HSerial_Instance_t uart, uint8_t *buffer, uint16_t size, HSerial_Callback_t callback);

#endif /* HSERIAL_INTERFACE_H */