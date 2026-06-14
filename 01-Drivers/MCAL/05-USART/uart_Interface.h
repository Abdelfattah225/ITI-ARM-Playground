/**
 * @file    uart_Interface.h
 * @brief   UART Driver Public Interface Header
 * @details This header provides the public API for the UART driver including:
 *          - Configuration structures for UART initialization
 *          - Handle structure for runtime UART management
 *          - Ring buffer types and functions for data buffering
 *          - Error codes for status reporting
 *          - Function prototypes for UART operations
 * 
 * @author  [Abdelfatah Moawed]
 * @date    [14/12/2025]
 * @version 1.0
 * 
 * @note    Supported UART instances: USART1, USART2, USART6
 * @note    Target: STM32F4xx series microcontrollers
 * 
 * @par Usage Example:
 * @code
 *      // 1. Create configuration and handle
 *      UART_Config_t uart_config = {
 *          .uart_instance = UART_INSTANCE_1,
 *          .Buad_rate = 115200,
 *          .word_length = UART_WORD_8BIT,
 *          .Parity = UART_PARITY_NONE,
 *          .Stop_bit = UART_STOP_1
 *      };
 *      UART_Handle_t uart_handle;
 *      
 *      // 2. Initialize UART
 *      UART_Init(&uart_config, &uart_handle);
 *      
 *      // 3. Send data
 *      uint8_t tx_data[] = "Hello";
 *      UART_SendBuffer(&uart_handle, tx_data, 5);
 *      
 *      // 4. Receive data
 *      uint8_t rx_data[32];
 *      uint16_t count;
 *      if (UART_GetRxCount(&uart_handle) > 0) {
 *          UART_ReceiveBuffer(&uart_handle, rx_data, 32, &count);
 *      }
 * @endcode
 */

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

/*============================================================================
 *                              INCLUDES
 *============================================================================*/

#include "../../../lib/lib_utii.h"                    /* Standard types (uint8_t, etc.) */
#include "../../configuration/MCAL/uart_cfg.h"        /* Buffer size configurations */

/*============================================================================
 *                          TYPE DEFINITIONS
 *============================================================================*/

/**
 * @brief   Ring Buffer (Circular Buffer) Structure
 * 
 * @details Ring buffers provide efficient FIFO (First-In-First-Out) data storage
 *          for interrupt-driven communication. They allow the ISR to store/retrieve
 *          data without blocking, and the main application to access data at its
 *          own pace.
 * 
 *          Visual representation of ring buffer operation:
 *          @verbatim
 *          Initial state (empty):
 *          [_][_][_][_][_][_][_][_]
 *           ^
 *          head=tail=0, count=0
 *          
 *          After writing 3 bytes (A, B, C):
 *          [A][B][C][_][_][_][_][_]
 *           ^     ^
 *          tail  head=3, count=3
 *          
 *          After reading 2 bytes:
 *          [_][_][C][_][_][_][_][_]
 *                 ^  ^
 *              tail=2 head=3, count=1
 *          
 *          Wrap-around (writing D, E, F, G, H, I):
 *          [I][_][C][D][E][F][G][H]
 *              ^  ^
 *          head=1 tail=2, count=7
 *          @endverbatim
 * 
 * @note    This structure should be initialized using ring_buffer_init()
 *          before use.
 */
typedef struct
{
    uint8_t  *buffer;   /**< Pointer to the data storage array */
    uint16_t head;      /**< Write index - where next byte will be written */
    uint16_t tail;      /**< Read index - where next byte will be read from */
    uint16_t size;      /**< Total capacity of the buffer in bytes */
    uint16_t count;     /**< Current number of bytes stored in buffer */

} UART_ring_buffer_t;

/**
 * @brief   UART Driver Error/Status Codes
 * 
 * @details Enumeration of all possible return values from UART driver functions.
 *          These codes indicate success or the specific type of error encountered.
 */
typedef enum
{
    UART_OK = 0,        /**< Operation completed successfully */
    UART_NOK,           /**< General error / operation failed */
    UART_NULL_POINTER,  /**< NULL pointer passed as parameter */
    UART_BUFFER_FULL,   /**< Ring buffer is full, cannot write more data */
    UART_BUFFER_EMPTY   /**< Ring buffer is empty, no data to read */
} UART_enu_err_t;

/**
 * @brief   UART Word Length Configuration
 * 
 * @details Defines the number of data bits per UART frame.
 *          
 *          Frame structure (UART_WORD_8BIT, no parity):
 *          [START][D0][D1][D2][D3][D4][D5][D6][D7][STOP]
 *          
 *          Frame structure (UART_WORD_9BIT or 8-bit with parity):
 *          [START][D0][D1][D2][D3][D4][D5][D6][D7][D8/P][STOP]
 * 
 * @note    When parity is enabled with 8-bit word length, actual data is 7 bits
 *          (8th bit is parity). Use 9-bit mode for 8 data bits + parity.
 */
typedef enum
{
    UART_WORD_8BIT,     /**< 8 data bits per frame (most common) */
    UART_WORD_9BIT      /**< 9 data bits per frame (used with parity for 8-bit data) */
} UART_WordLength_t;

/**
 * @brief   UART Stop Bits Configuration
 * 
 * @details Defines the number of stop bits at the end of each frame.
 *          Stop bits provide time for the receiver to process the received byte.
 *          
 *          - 1 stop bit: Standard for most applications
 *          - 2 stop bits: Used for slower receivers or noisy environments
 *          - 0.5/1.5 stop bits: Used in smartcard mode
 * 
 * @note    Values match the USART_CR2 STOP bit field directly
 */
typedef enum
{
    UART_STOP_1   = 0,  /**< 1 stop bit (default, most common) */
    UART_STOP_0_5 = 1,  /**< 0.5 stop bit (smartcard mode) */
    UART_STOP_2   = 2,  /**< 2 stop bits */
    UART_STOP_1_5 = 3   /**< 1.5 stop bits (smartcard mode) */
} UART_StopBits_t;

/**
 * @brief   UART Parity Configuration
 * 
 * @details Parity is an error detection mechanism. The parity bit makes the
 *          total number of 1-bits either even or odd.
 *          
 *          - NONE: No parity bit, no error detection
 *          - EVEN: Parity bit set so total 1-bits is even
 *          - ODD:  Parity bit set so total 1-bits is odd
 * 
 *          Example (data = 0x35 = 0011 0101, five 1-bits):
 *          - Even parity: P=1 (makes total 6, which is even)
 *          - Odd parity:  P=0 (keeps total 5, which is odd)
 * 
 * @note    Parity can detect single-bit errors but cannot correct them
 */
typedef enum
{
    UART_PARITY_NONE,   /**< No parity bit - no error detection */
    UART_PARITY_EVEN,   /**< Even parity - bit set for even number of 1s */
    UART_PARITY_ODD     /**< Odd parity - bit set for odd number of 1s */
} UART_Parity_t;

/**
 * @brief   UART Instance Selection
 * 
 * @details Identifies which physical UART peripheral to use.
 *          
 *          STM32F401 UART availability and pin options:
 *          | Instance | Bus  | Default TX | Default RX | Alt TX | Alt RX |
 *          |----------|------|------------|------------|--------|--------|
 *          | USART1   | APB2 | PA9        | PA10       | PB6    | PB7    |
 *          | USART2   | APB1 | PA2        | PA3        | PD5    | PD6    |
 *          | USART6   | APB2 | PA11       | PA12       | PC6    | PC7    |
 * 
 * @note    This driver uses default pin mapping (Port A pins)
 */
typedef enum
{
    UART_INSTANCE_1,    /**< USART1 - PA9/PA10, APB2 bus (high speed) */
    UART_INSTANCE_2,    /**< USART2 - PA2/PA3, APB1 bus */
    UART_INSTANCE_6     /**< USART6 - PA11/PA12, APB2 bus (high speed) */
} UART_Instance_t;

/**
 * @brief   UART Configuration Structure
 * 
 * @details Contains all parameters needed to initialize a UART peripheral.
 *          Pass this structure to UART_Init() to configure the hardware.
 * 
 * @note    All fields must be set before calling UART_Init()
 * 
 * @par     Common Configurations:
 *          - 115200 8N1: 115200 baud, 8 data bits, No parity, 1 stop bit
 *          - 9600 8N1:   9600 baud, 8 data bits, No parity, 1 stop bit
 *          - 9600 8E1:   9600 baud, 8 data bits, Even parity, 1 stop bit
 */
typedef struct
{
    UART_Instance_t   uart_instance;  /**< Which UART peripheral (1, 2, or 6) */
    uint32_t          Buad_rate;      /**< Baud rate in bits/second (e.g., 9600, 115200) */
    UART_WordLength_t word_length;    /**< Data bits per frame (8 or 9) */
    UART_StopBits_t   Stop_bit;       /**< Number of stop bits */
    UART_Parity_t     Parity;         /**< Parity mode (None, Even, Odd) */
    
} UART_Config_t;

/**
 * @brief   UART Runtime Handle Structure
 * 
 * @details This structure maintains the runtime state of a UART instance.
 *          It is populated by UART_Init() and used by all other UART functions.
 *          
 *          Contains:
 *          - Pointer to hardware registers
 *          - TX and RX ring buffers with their memory
 *          - Transmission state flag
 * 
 * @warning This structure must remain in scope (not be destroyed) for the
 *          entire time the UART is in use, as the ISR accesses it.
 * 
 * @note    Declare as static or global, not as local variable in a function
 *          that returns.
 * 
 * @par     Memory Usage:
 *          sizeof(UART_Handle_t) ≈ 2 * (UART_TX_BUFFER_SIZE + sizeof(ring_buffer_t))
 *                                + 2 * (UART_RX_BUFFER_SIZE + sizeof(ring_buffer_t))
 *                                + ~10 bytes overhead
 */
typedef struct
{
    /**
     * @brief Pointer to UART hardware registers
     * @details Cast to USART_TypeDef* when accessing registers.
     *          Set by UART_Init() based on selected instance.
     */
    void *instance;
    
    /**
     * @brief TX ring buffer control structure
     * @details Manages the transmit buffer. Data is added by UART_SendBuffer()
     *          and removed by the TXE interrupt handler.
     */
    UART_ring_buffer_t tx_buffer;
    
    /**
     * @brief RX ring buffer control structure
     * @details Manages the receive buffer. Data is added by the RXNE interrupt
     *          handler and removed by UART_ReceiveBuffer().
     */
    UART_ring_buffer_t rx_buffer;
    
    /**
     * @brief TX buffer data storage
     * @details Fixed-size array for transmit data. Size defined by
     *          UART_TX_BUFFER_SIZE in uart_cfg.h
     */
    uint8_t tx_buffer_mem[UART_TX_BUFFER_SIZE];
    
    /**
     * @brief RX buffer data storage  
     * @details Fixed-size array for receive data. Size defined by
     *          UART_RX_BUFFER_SIZE in uart_cfg.h
     */
    uint8_t rx_buffer_mem[UART_RX_BUFFER_SIZE];
    
    /**
     * @brief Buffer size (reserved for future use)
     * @note Currently unused - buffer sizes come from config macros
     */
    uint16_t size;
    
    /**
     * @brief TX busy flag
     * @details Indicates if a transmission is currently in progress.
     *          - 0: UART TX is idle
     *          - 1: Transmission in progress (data in buffer or being sent)
     *          Set by UART_SendBuffer(), cleared by ISR when buffer empties.
     */
    uint8_t tx_busy;
    
} UART_Handle_t;

/*============================================================================
 *                    RING BUFFER FUNCTION PROTOTYPES
 *============================================================================*/

/**
 * @brief   Initialize a ring buffer with provided memory
 * 
 * @param[out] RB      Pointer to ring buffer structure to initialize
 * @param[in]  buffer  Pointer to memory array for data storage
 * @param[in]  size    Size of the buffer array in bytes
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Success
 * @retval UART_NULL_POINTER Invalid parameters
 * 
 * @note    Called internally by UART_Init(), typically not needed by user
 */
UART_enu_err_t ring_buffer_init(UART_ring_buffer_t *RB, uint8_t *buffer, uint16_t size);

/**
 * @brief   Write one byte to ring buffer (at HEAD)
 * 
 * @param[in] buffer  Pointer to ring buffer
 * @param[in] data    Byte to write
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Success
 * @retval UART_NULL_POINTER buffer is NULL
 * @retval UART_BUFFER_FULL  No space available
 */
UART_enu_err_t ring_buffer_write(UART_ring_buffer_t *buffer, uint8_t data);

/**
 * @brief   Read one byte from ring buffer (from TAIL)
 * 
 * @param[in]  buffer  Pointer to ring buffer
 * @param[out] data    Pointer to store read byte
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Success
 * @retval UART_NULL_POINTER Invalid parameters
 * @retval UART_BUFFER_EMPTY No data available
 */
UART_enu_err_t ring_buffer_read(UART_ring_buffer_t *buffer, uint8_t *data);

/**
 * @brief   Get number of bytes currently in buffer
 * 
 * @param[in] RB  Pointer to ring buffer
 * @return    Byte count, 0 if RB is NULL
 */
uint16_t ring_buffer_get_count(UART_ring_buffer_t *RB);

/**
 * @brief   Get available free space in buffer
 * 
 * @param[in] RB  Pointer to ring buffer
 * @return    Free bytes, 0 if RB is NULL
 */
uint16_t ring_buffer_get_free_space(UART_ring_buffer_t *RB);

/**
 * @brief   Check if buffer is empty
 * 
 * @param[in] RB  Pointer to ring buffer
 * @return    1 if empty, 0 otherwise
 */
uint8_t ring_buffer_is_empty(UART_ring_buffer_t *RB);

/**
 * @brief   Check if buffer is full
 * 
 * @param[in] RB  Pointer to ring buffer
 * @return    1 if full, 0 otherwise
 */
uint8_t ring_buffer_is_full(UART_ring_buffer_t *RB);

/*============================================================================
 *                    UART FUNCTION PROTOTYPES
 *============================================================================*/

/**
 * @brief   Calculate BRR register value for baud rate
 * 
 * @param[in] periph_clock  Peripheral clock frequency (Hz)
 * @param[in] baudrate      Desired baud rate (bps)
 * 
 * @return  16-bit BRR register value
 * 
 * @note    Internal function, typically not called by user
 */
uint16_t uart_calculate_BRR(uint32_t periph_clock, uint32_t baudrate);

/**
 * @brief   Initialize UART peripheral
 * 
 * @details Configures GPIO pins, enables clocks, sets up baud rate,
 *          frame format, and interrupts. Initializes ring buffers.
 * 
 * @param[in]  uart    Pointer to configuration structure
 * @param[out] handle  Pointer to handle structure (will be populated)
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Success
 * @retval UART_NULL_POINTER Invalid parameters
 * @retval UART_NOK          Invalid UART instance
 * 
 * @pre     System clocks must be configured
 * @post    UART is ready for TX/RX operations
 * 
 * @warning Handle must remain valid for duration of UART usage
 */
UART_enu_err_t UART_Init(UART_Config_t *uart, UART_Handle_t *handle);

/**
 * @brief   Queue data for transmission (non-blocking)
 * 
 * @details Copies data to TX buffer and starts transmission.
 *          Returns immediately - actual transmission happens via ISR.
 * 
 * @param[in] handle  Pointer to initialized UART handle
 * @param[in] data    Pointer to data buffer to send
 * @param[in] length  Number of bytes to send
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Data queued successfully
 * @retval UART_NULL_POINTER Invalid parameters
 * @retval UART_BUFFER_FULL  Insufficient buffer space
 * 
 * @note    Check UART_GetTxFreeSpace() before calling if needed
 * @note    Use UART_IsTxBusy() to check transmission completion
 */
UART_enu_err_t UART_SendBuffer(UART_Handle_t *handle, uint8_t *data, uint16_t length);

/**
 * @brief   Read received data from RX buffer (non-blocking)
 * 
 * @details Reads up to 'length' bytes from the RX ring buffer.
 *          May return fewer bytes if buffer has less data.
 * 
 * @param[in]  handle          Pointer to initialized UART handle
 * @param[out] data            Buffer to store received data
 * @param[in]  length          Maximum bytes to read
 * @param[out] received_count  Actual bytes read
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           At least one byte read
 * @retval UART_NULL_POINTER Invalid parameters
 * @retval UART_BUFFER_EMPTY No data available
 * 
 * @note    Always check received_count for actual amount read
 */
UART_enu_err_t UART_ReceiveBuffer(UART_Handle_t *handle, uint8_t *data, 
                                   uint16_t length, uint16_t *received_count);

/*============================================================================
 *                    UART HELPER FUNCTION PROTOTYPES
 *============================================================================*/

/**
 * @brief   Get number of bytes available in RX buffer
 * 
 * @param[in] handle  Pointer to UART handle
 * @return    Number of bytes ready to read
 * 
 * @par Example:
 * @code
 *      if (UART_GetRxCount(&handle) >= expected_bytes) {
 *          UART_ReceiveBuffer(&handle, buffer, expected_bytes, &count);
 *      }
 * @endcode
 */
uint16_t UART_GetRxCount(UART_Handle_t *handle);

/**
 * @brief   Get free space in TX buffer
 * 
 * @param[in] handle  Pointer to UART handle
 * @return    Number of bytes that can be queued
 * 
 * @par Example:
 * @code
 *      if (UART_GetTxFreeSpace(&handle) >= data_length) {
 *          UART_SendBuffer(&handle, data, data_length);
 *      }
 * @endcode
 */
uint16_t UART_GetTxFreeSpace(UART_Handle_t *handle);

/**
 * @brief   Check if transmission is in progress
 * 
 * @param[in] handle  Pointer to UART handle
 * @return    1 if busy, 0 if idle
 * 
 * @par Example:
 * @code
 *      // Wait for transmission to complete before sleep
 *      while (UART_IsTxBusy(&handle)) {
 *          // Optionally yield or do other work
 *      }
 *      enter_sleep_mode();
 * @endcode
 */
uint8_t UART_IsTxBusy(UART_Handle_t *handle);

#endif /* UART_INTERFACE_H */