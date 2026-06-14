/**
 * @file    uart_cfg.h
 * @brief   UART Driver Configuration Header
 * @details This file contains compile-time configuration options for the
 *          UART driver. Modify these values to customize buffer sizes
 *          and other driver parameters.
 * 
 * @author  [Abdelfattah Moawed]
 * @date    [14/12/2025]
 * @version 1.0
 * 
 * @note    Changes to this file require recompilation of the UART driver
 * 
 * @par     Memory Considerations:
 *          Each UART_Handle_t uses approximately:
 *          TX_BUFFER_SIZE + RX_BUFFER_SIZE + ~40 bytes overhead
 *          
 *          With default 64-byte buffers:
 *          64 + 64 + 40 = ~168 bytes per UART instance
 */

#ifndef UART_CFG_H
#define UART_CFG_H

/*============================================================================
 *                    BUFFER SIZE CONFIGURATION
 *============================================================================*/

/**
 * @def     BUFFER_MAX
 * @brief   Maximum general buffer size (legacy/reserved)
 * @note    Consider using UART_TX_BUFFER_SIZE or UART_RX_BUFFER_SIZE instead
 */
#define BUFFER_MAX 64U

/**
 * @def     UART_TX_BUFFER_SIZE
 * @brief   Size of the transmit ring buffer in bytes
 * 
 * @details Determines how much data can be queued for transmission.
 *          Larger buffers allow queueing more data without blocking,
 *          but consume more RAM.
 * 
 *          Sizing guidelines:
 *          - Minimum: Largest single message size you'll send
 *          - Typical: 64-256 bytes for most applications
 *          - Large: 512+ bytes for high-throughput or burst data
 * 
 *          At 115200 baud, 64 bytes takes ~5.5ms to transmit
 *          At 9600 baud, 64 bytes takes ~67ms to transmit
 * 
 * @note    Must be > 0 and typically a power of 2 for efficiency
 * @warning Larger buffers increase RAM usage per UART instance
 */
#define UART_TX_BUFFER_SIZE 64

/**
 * @def     UART_RX_BUFFER_SIZE  
 * @brief   Size of the receive ring buffer in bytes
 * 
 * @details Determines how much received data can be buffered before
 *          the application reads it. Must be large enough to hold
 *          data between application reads to prevent overflow.
 * 
 *          Sizing guidelines:
 *          - Consider worst-case time between reads
 *          - Account for burst data reception
 *          - At 115200 baud: ~11,520 bytes/second maximum
 *          - At 9600 baud: ~960 bytes/second maximum
 * 
 *          Example: If your main loop runs every 10ms and baud is 115200:
 *          Max bytes in 10ms = 115200 / 1000 * 10 ≈ 115 bytes
 *          Recommended buffer: at least 128 bytes with margin
 * 
 * @note    Must be > 0 and typically a power of 2 for efficiency
 * @warning If buffer overflows, incoming data is lost!
 */
#define UART_RX_BUFFER_SIZE 64

/*============================================================================
 *                    OPTIONAL CONFIGURATION
 *============================================================================*/

/*
 * Additional configuration options can be added here:
 *
 * // Enable/disable specific UART instances to save code space
 * #define UART_ENABLE_USART1      1
 * #define UART_ENABLE_USART2      1  
 * #define UART_ENABLE_USART6      0
 *
 * // Default baud rate (if not specified in config struct)
 * #define UART_DEFAULT_BAUDRATE   115200
 *
 * // Enable RX overflow callback
 * #define UART_RX_OVERFLOW_CALLBACK_ENABLE  0
 *
 * // Enable DMA support (future feature)
 * #define UART_DMA_ENABLE         0
 */

#endif /* UART_CFG_H */
