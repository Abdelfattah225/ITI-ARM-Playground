/**
 * @file    uart.c
 * @brief   UART/USART Driver Implementation for STM32F4xx
 * @details This driver provides interrupt-driven UART communication with
 *          ring buffer support for both TX and RX operations.
 *          Supports USART1, USART2, and USART6 peripherals.
 * 
 * @author  [Abdelfattah]
 * @date    [14/12/2025]
 * @version 1.0
 * 
 * @note    Hardware: STM32F401xx / STM32F4xx series
 * @note    This driver uses interrupt-based (non-blocking) TX/RX with circular buffers
 */

/*============================================================================
 *                              INCLUDES
 *============================================================================*/
#include "../../../include/configuration/MCAL/uart_cfg.h"
#include "../../../include/interface/MCAL/uart_Interface.h"
#include "../../../include/private/MCAL/uart_private.h"

#include "../../../include/interface/MCAL/rcc_init.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"
#include "../../../include/interface/MCAL/NVIC_Interface.h"

/*============================================================================
 *                         PRIVATE VARIABLES
 *============================================================================*/

/**
 * @brief Static handle pointers for each UART instance
 * @details These pointers are used by ISRs to access the correct handle
 *          since ISRs cannot receive parameters. Each UART instance
 *          stores its handle pointer here during initialization.
 */
static UART_Handle_t *uart1_handle = NULL;  /* Handle pointer for USART1 ISR */
static UART_Handle_t *uart2_handle = NULL;  /* Handle pointer for USART2 ISR */
static UART_Handle_t *uart6_handle = NULL;  /* Handle pointer for USART6 ISR */

/*============================================================================
 *                         PRIVATE DEFINES
 *============================================================================*/

/**
 * @brief Local Status Register (SR) bit masks
 * @details These masks are used for efficient flag checking in ISR
 *          by reading SR register once and masking bits
 */
#define USART_SR_ORE   (1U << 3)   /* Overrun Error flag - bit 3 of SR */
#define USART_SR_RXNE  (1U << 5)   /* RX Not Empty flag - bit 5 of SR  */

/*============================================================================
 *                    PUBLIC FUNCTION IMPLEMENTATIONS
 *============================================================================*/

/**
 * @brief   Initialize UART peripheral with specified configuration
 * 
 * @details This function performs complete UART initialization:
 *          1. Validates input parameters
 *          2. Enables required clocks (GPIO and UART peripheral)
 *          3. Configures TX/RX GPIO pins in alternate function mode
 *          4. Initializes TX/RX ring buffers
 *          5. Configures baud rate, word length, parity, and stop bits
 *          6. Enables UART and RXNE interrupt
 *          7. Enables NVIC interrupt for the UART
 * 
 * @param[in]  uart    Pointer to UART configuration structure containing:
 *                     - uart_instance: Which UART to use (1, 2, or 6)
 *                     - Buad_rate: Desired baud rate (e.g., 9600, 115200)
 *                     - word_length: 8 or 9 bit data
 *                     - Parity: None, Even, or Odd
 *                     - Stop_bit: Number of stop bits
 * 
 * @param[out] handle  Pointer to UART handle structure that will be
 *                     populated with runtime data (instance pointer,
 *                     ring buffers, busy flag)
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Initialization successful
 * @retval UART_NULL_POINTER Either uart or handle is NULL
 * @retval UART_NOK          Invalid UART instance specified
 * 
 * @note    Pin mapping (fixed in this implementation):
 *          - USART1: TX=PA9,  RX=PA10 (AF7)
 *          - USART2: TX=PA2,  RX=PA3  (AF7)
 *          - USART6: TX=PA11, RX=PA12 (AF8)
 * 
 * @warning The handle must remain valid (not go out of scope) for the
 *          entire duration of UART usage, as ISRs reference it.
 * 
 * @code
 *          // Example usage:
 *          UART_Config_t config = {
 *              .uart_instance = UART_INSTANCE_1,
 *              .Buad_rate = 115200,
 *              .word_length = UART_WORD_8BIT,
 *              .Parity = UART_PARITY_NONE,
 *              .Stop_bit = UART_STOP_1BIT
 *          };
 *          UART_Handle_t handle;
 *          UART_Init(&config, &handle);
 * @endcode
 */
UART_enu_err_t UART_Init(UART_Config_t *uart, UART_Handle_t *handle)
{
    /*------------------------------------------------------------------
     * Step 1: Parameter Validation
     * - Ensure no NULL pointers are passed
     * - This is critical as dereferencing NULL causes hard fault
     *------------------------------------------------------------------*/
    if (uart == NULL || handle == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* GPIO pin configuration structures for TX and RX pins */
    GPIO_PinConfig_t tx_pinconfig, rx_pinconfig;
    
    /* Enable GPIOA clock - all UART pins in this driver use Port A */
    RCC_AHB1_EnableClock(RCC_AHB1_GPIOA);

    /* Peripheral clock frequency - needed for baud rate calculation
     * USART1/6 are on APB2 bus, USART2 is on APB1 bus */
    uint32_t periph_clock = 0;

    /* Pointer to UART register block - will be set based on instance */
    volatile USART_TypeDef *uart_reg = NULL;

    /*------------------------------------------------------------------
     * Step 2-4: Instance-specific Configuration
     * - Enable peripheral clock
     * - Configure GPIO pins for alternate function
     * - Store handle pointer for ISR access
     *------------------------------------------------------------------*/
    switch (uart->uart_instance)
    {
    case UART_INSTANCE_1:
        /* ============== USART1 Configuration ============== */
        
        /* Set instance pointer in handle for later register access */
        uart_reg = USART1;
        handle->instance = USART1;

        /* USART1 is on APB2 bus - get APB2 clock frequency */
        periph_clock = RCC_GetPCLK2Frequency();

        /* Enable USART1 peripheral clock on APB2 bus */
        RCC_APB2_EnableClock(RCC_APB2_USART1);
        
        /* Enable GPIOA clock (redundant but safe) */
        RCC_AHB1_EnableClock(RCC_AHB1_GPIOA);

        /*--- TX Pin Configuration (PA9) ---*/
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN9;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;      /* Alternate function mode */
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL; /* Push-pull for TX */
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;           /* High speed for UART */
        tx_pinconfig.PuPd = GPIO_PUPD_NONE;            /* No pull-up/down for TX */
        tx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2; /* AF7 = USART1/2 */
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);

        /*--- RX Pin Configuration (PA10) ---*/
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN10;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_PULLUP;          /* Pull-up on RX to prevent floating */
        rx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);

        /* Store handle pointer for ISR - ISRs can't receive parameters,
         * so we store the handle in a static variable */
        uart1_handle = handle;
        break;

    case UART_INSTANCE_2:
        /* ============== USART2 Configuration ============== */
        
        handle->instance = USART2;
        
        /* Enable USART2 peripheral clock on APB1 bus */
        RCC_APB1_EnableClock(RCC_APB1_USART2);
        
        uart_reg = USART2;
        
        /* USART2 is on APB1 bus - typically slower than APB2 */
        periph_clock = RCC_GetPCLK1Frequency();

        /*--- TX Pin Configuration (PA2) ---*/
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN2;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;
        tx_pinconfig.PuPd = GPIO_PUPD_NONE;
        tx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);

        /*--- RX Pin Configuration (PA3) ---*/
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN3;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_PULLUP;
        rx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);

        uart2_handle = handle;
        break;

    case UART_INSTANCE_6:
        /* ============== USART6 Configuration ============== */
        
        handle->instance = USART6;
        
        /* Enable USART6 peripheral clock on APB2 bus */
        RCC_APB2_EnableClock(RCC_APB2_USART6);
        
        uart_reg = USART6;
        
        /* USART6 is on APB2 bus */
        periph_clock = RCC_GetPCLK2Frequency();

        /*--- TX Pin Configuration (PA11) ---*/
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN11;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;
        tx_pinconfig.PuPd = GPIO_PUPD_PULLUP;
        tx_pinconfig.AlternateFunc = GPIO_AF8_USART6;  /* AF8 = USART6 */
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);

        /*--- RX Pin Configuration (PA12) ---*/
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN12;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_NONE;
        rx_pinconfig.AlternateFunc = GPIO_AF8_USART6;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);

        uart6_handle = handle;
        break;

    default:
        /* Invalid UART instance specified */
        return UART_NOK;
    }

    /*------------------------------------------------------------------
     * Step 5: Initialize Ring Buffers
     * - TX buffer: Holds data waiting to be transmitted
     * - RX buffer: Holds received data waiting to be read by application
     * - Ring buffers enable non-blocking interrupt-driven operation
     *------------------------------------------------------------------*/
    ring_buffer_init(&handle->tx_buffer, handle->tx_buffer_mem, UART_TX_BUFFER_SIZE);
    ring_buffer_init(&handle->rx_buffer, handle->rx_buffer_mem, UART_RX_BUFFER_SIZE);

    /*------------------------------------------------------------------
     * Step 6: Configure Baud Rate Register (BRR)
     * - BRR = fPCLK / (16 * BaudRate) for OVER8=0
     * - Contains mantissa (bits 15:4) and fraction (bits 3:0)
     *------------------------------------------------------------------*/
    uart_reg->BRR.REG = uart_calculate_BRR(periph_clock, uart->Buad_rate);

    /*------------------------------------------------------------------
     * Step 7: Configure Control Register 1 (CR1)
     * - Word length (M bit): 0=8-bit, 1=9-bit
     * - Parity control enable (PCE)
     * - Parity selection (PS): 0=Even, 1=Odd
     * - Transmitter enable (TE)
     * - Receiver enable (RE)
     * - RXNE Interrupt enable (RXNEIE)
     *------------------------------------------------------------------*/
    
    /* Start with clean CR1 configuration */
    uart_reg->CR1.REG = 0;
    
    /* Set word length: M bit = 1 for 9-bit, 0 for 8-bit */
    uart_reg->CR1.BIT.M = (uart->word_length == UART_WORD_9BIT) ? 1 : 0;

    /* Configure parity settings */
    if (uart->Parity == UART_PARITY_NONE)
    {
        uart_reg->CR1.BIT.PCE = 0;  /* Disable parity control */
    }
    else
    {
        uart_reg->CR1.BIT.PCE = 1;  /* Enable parity control */
        /* PS bit: 0 = Even parity, 1 = Odd parity */
        uart_reg->CR1.BIT.PS = (uart->Parity == UART_PARITY_ODD) ? 1 : 0;
    }

    /* Enable Transmitter and Receiver */
    uart_reg->CR1.BIT.TE = 1;  /* Transmitter Enable */
    uart_reg->CR1.BIT.RE = 1;  /* Receiver Enable */

    /* Enable RXNE (RX Not Empty) interrupt for receive operations
     * This triggers an interrupt when data is received */
    uart_reg->CR1.BIT.RXNEIE = 1;

    /* DO NOT enable TXE interrupt here!
     * TXE interrupt is enabled only when there's data to transmit.
     * Enabling it here would cause continuous interrupts since
     * TXE is always set when transmit buffer is empty */
    uart_reg->CR1.BIT.TXEIE = 0;

    /*------------------------------------------------------------------
     * Step 8: Configure Control Register 2 (CR2)
     * - Stop bits configuration
     * - 00: 1 stop bit, 01: 0.5 stop bit, 10: 2 stop bits, 11: 1.5 stop bits
     *------------------------------------------------------------------*/
    uart_reg->CR2.BIT.STOP = uart->Stop_bit;

    /*------------------------------------------------------------------
     * Step 9: Enable UART Peripheral
     * - Setting UE bit enables the UART
     * - Must be done after all configuration is complete
     *------------------------------------------------------------------*/
    uart_reg->CR1.BIT.UE = 1;

    /*------------------------------------------------------------------
     * Step 10: Enable NVIC Interrupt
     * - Enable the interrupt in the Nested Vectored Interrupt Controller
     * - This allows the CPU to respond to UART interrupt requests
     *------------------------------------------------------------------*/
    switch (uart->uart_instance)
    {
    case UART_INSTANCE_1:
        NVIC_EnableIRQ(USART1_IRQn);  /* IRQ number 37 */
        break;
    case UART_INSTANCE_2:
        NVIC_EnableIRQ(USART2_IRQn);  /* IRQ number 38 */
        break;
    case UART_INSTANCE_6:
        NVIC_EnableIRQ(USART6_IRQn);  /* IRQ number 71 */
        break;
    default:
        break;
    }

    /* Initialize TX busy flag - no transmission in progress */
    handle->tx_busy = 0;

    return UART_OK;
}

/**
 * @brief   Queue data for transmission via UART (non-blocking)
 * 
 * @details This function copies data to the TX ring buffer and initiates
 *          transmission if not already in progress. The actual transmission
 *          is handled by the ISR in the background.
 * 
 *          Operation flow:
 *          1. Validate parameters
 *          2. Check if TX buffer has enough space
 *          3. Copy all bytes to TX ring buffer
 *          4. If UART is idle, kick-start transmission by:
 *             - Writing first byte to DR (Data Register)
 *             - Enabling TXE interrupt for subsequent bytes
 *          5. If UART is already transmitting, just ensure TXE interrupt
 *             is enabled - ISR will handle the new data
 * 
 * @param[in] handle  Pointer to initialized UART handle
 * @param[in] data    Pointer to data buffer to transmit
 * @param[in] length  Number of bytes to transmit
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Data queued successfully
 * @retval UART_NULL_POINTER handle or data is NULL
 * @retval UART_BUFFER_FULL  Not enough space in TX buffer for all data
 * 
 * @note    This is a non-blocking function - it returns immediately after
 *          queueing data. Use UART_IsTxBusy() to check if transmission
 *          is complete.
 * 
 * @warning All bytes must fit in buffer - partial writes are not supported.
 *          Check UART_GetTxFreeSpace() before calling if unsure.
 */
UART_enu_err_t UART_SendBuffer(UART_Handle_t *handle, uint8_t *data, uint16_t length)
{
    /* Validate input parameters */
    if (handle == NULL || data == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* Zero-length transmission is a no-op success */
    if (length == 0)
    {
        return UART_OK;
    }

    /* Check if TX buffer has enough space for all data
     * We require ALL bytes to fit - no partial writes */
    if (ring_buffer_get_free_space(&handle->tx_buffer) < length)
    {
        return UART_BUFFER_FULL;
    }

    /* Copy all bytes to TX ring buffer
     * This is safe since we verified space above */
    for (uint16_t i = 0; i < length; i++)
    {
        ring_buffer_write(&handle->tx_buffer, data[i]);
    }

    /* Get pointer to UART registers for direct hardware access */
    volatile USART_TypeDef *uart_reg = (volatile USART_TypeDef *)handle->instance;

    /*--------------------------------------------------------------
     * TRANSMISSION START LOGIC (CRITICAL SECTION)
     * 
     * Two cases:
     * 1. UART is idle (tx_busy=0): We must "kick-start" transmission
     *    by writing the first byte to DR and enabling TXE interrupt
     * 
     * 2. UART is already transmitting (tx_busy=1): Just ensure
     *    TXE interrupt is enabled - ISR will pick up new data
     * 
     * BUGFIX NOTE: If already transmitting, we must NOT write to DR
     * here as it could corrupt an ongoing transmission!
     *--------------------------------------------------------------*/
    if (handle->tx_busy == 0)
    {
        /* UART is idle - we need to start transmission */
        handle->tx_busy = 1;

        /* Check if Transmit Data Register is empty (TXE=1) */
        if (uart_reg->SR.BIT.TXE == 1)
        {
            uint8_t byte;
            
            /* Read first byte from buffer and write to Data Register */
            if (ring_buffer_read(&handle->tx_buffer, &byte) == UART_OK)
            {
                /* Write byte to DR - this starts the transmission
                 * Writing to DR clears TXE flag */
                uart_reg->DR.REG = byte;
                
                /* Enable TXE interrupt - ISR will send remaining bytes */
                uart_reg->CR1.BIT.TXEIE = 1;
            }
            else
            {
                /* Unexpected: buffer should have data since we just wrote to it
                 * Reset to idle state */
                handle->tx_busy = 0;
                uart_reg->CR1.BIT.TXEIE = 0;
            }
        }
        else
        {
            /* TXE not set (transmitter not ready yet)
             * Enable interrupt - ISR will handle when TXE becomes 1 */
            uart_reg->CR1.BIT.TXEIE = 1;
        }
    }
    else
    {
        /* UART already busy transmitting
         * Just ensure TXE interrupt is enabled - ISR will send new data
         * when it finishes current transmission */
        uart_reg->CR1.BIT.TXEIE = 1;
    }

    return UART_OK;
}

/**
 * @brief   Read received data from RX ring buffer (non-blocking)
 * 
 * @details This function reads data that was previously received by the
 *          UART ISR and stored in the RX ring buffer. It reads up to
 *          'length' bytes, but may return fewer if buffer has less data.
 * 
 * @param[in]  handle          Pointer to initialized UART handle
 * @param[out] data            Buffer to store received data
 * @param[in]  length          Maximum number of bytes to read
 * @param[out] received_count  Actual number of bytes read
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           At least one byte was read
 * @retval UART_NULL_POINTER handle, data, or received_count is NULL
 * @retval UART_BUFFER_EMPTY No data available in RX buffer
 * 
 * @note    This function returns whatever data is available, up to 'length'
 *          bytes. Always check received_count for actual amount read.
 * 
 * @note    Use UART_GetRxCount() to check available data before calling.
 */
UART_enu_err_t UART_ReceiveBuffer(UART_Handle_t *handle,
                                  uint8_t *data,
                                  uint16_t length,
                                  uint16_t *received_count)
{
    /* Step 1: Validate all pointer parameters */
    if (handle == NULL || data == NULL || received_count == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* Initialize received count to 0 */
    *received_count = 0;

    /* Zero-length request is a no-op success */
    if (length == 0)
    {
        return UART_OK;
    }

    /* Read up to 'length' bytes from RX ring buffer */
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t byte;
        
        /* Attempt to read one byte from RX buffer */
        UART_enu_err_t st = ring_buffer_read(&handle->rx_buffer, &byte);
        
        if (st == UART_OK)
        {
            /* Successfully read a byte - store it */
            data[i] = byte;
            (*received_count)++;
        }
        else
        {
            /* RX buffer is empty - stop reading */
            break;
        }
    }

    /* Return OK if we got at least one byte, BUFFER_EMPTY otherwise */
    return (*received_count > 0) ? UART_OK : UART_BUFFER_EMPTY;
}

/**
 * @brief   Get number of bytes available in RX buffer
 * 
 * @param[in] handle  Pointer to UART handle
 * 
 * @return  Number of bytes waiting to be read, 0 if handle is NULL
 */
uint16_t UART_GetRxCount(UART_Handle_t *handle)
{
    if (handle == NULL)
    {
        return 0;
    }
    return ring_buffer_get_count(&handle->rx_buffer);
}

/**
 * @brief   Get available free space in TX buffer
 * 
 * @param[in] handle  Pointer to UART handle
 * 
 * @return  Number of bytes that can be written to TX buffer, 0 if handle is NULL
 */
uint16_t UART_GetTxFreeSpace(UART_Handle_t *handle)
{
    if (handle == NULL)
    {
        return 0;
    }
    return ring_buffer_get_free_space(&handle->tx_buffer);
}

/**
 * @brief   Check if UART transmission is in progress
 * 
 * @param[in] handle  Pointer to UART handle
 * 
 * @return  1 if transmission in progress, 0 if idle or handle is NULL
 * 
 * @note    Useful for waiting until all data is sent before entering
 *          low-power mode or disabling UART
 */
uint8_t UART_IsTxBusy(UART_Handle_t *handle)
{
    if (handle == NULL)
    {
        return 0;
    }
    return handle->tx_busy;
}

/*============================================================================
 *                    BAUD RATE CALCULATION
 *============================================================================*/

/**
 * @brief   Calculate BRR register value for desired baud rate
 * 
 * @details The USART baud rate is calculated from:
 *          BaudRate = fPCLK / (16 * USARTDIV)  [when OVER8=0]
 * 
 *          Therefore: USARTDIV = fPCLK / (16 * BaudRate)
 * 
 *          BRR format:
 *          - Bits 15:4 = Mantissa (integer part of USARTDIV)
 *          - Bits 3:0  = Fraction (fractional part * 16)
 * 
 *          To avoid floating point math, we calculate:
 *          USARTDIV * 16 = fPCLK / BaudRate
 *          Then extract mantissa and fraction from this value.
 * 
 * @param[in] periph_clock  Peripheral clock frequency in Hz (PCLK1 or PCLK2)
 * @param[in] baudrate      Desired baud rate (e.g., 9600, 115200)
 * 
 * @return  16-bit BRR register value
 * 
 * @note    The formula includes rounding: (clock + baud/2) / baud
 *          This gives better accuracy than truncation.
 */
uint16_t uart_calculate_BRR(uint32_t periph_clock, uint32_t baudrate)
{
    /* Prevent division by zero */
    if (baudrate == 0)
    {
        return 0;
    }
    
    /* Step 1: Calculate USARTDIV * 16 with rounding
     * Adding (baudrate/2) before division implements rounding */
    uint32_t USARTDIVx16 = (periph_clock + (baudrate / 2U)) / baudrate;
    
    /* Step 2: Extract mantissa (integer part)
     * Mantissa = USARTDIV * 16 / 16 = USARTDIV integer part */
    uint16_t Mantissa = USARTDIVx16 / 16U;
    
    /* Step 3: Extract fraction (0-15)
     * Fraction = USARTDIV * 16 % 16 = fractional part * 16 */
    uint16_t Fraction = USARTDIVx16 % 16U;
    
    /* Step 4: Combine into BRR format
     * BRR[15:4] = Mantissa, BRR[3:0] = Fraction */
    return (Mantissa << 4) | (Fraction & 0x0f);
}

/*============================================================================
 *                       RING BUFFER FUNCTIONS
 *============================================================================*/

/**
 * @brief   Initialize a ring buffer structure
 * 
 * @details Ring buffers (circular buffers) allow efficient FIFO data storage
 *          without needing to shift data. They use head/tail pointers that
 *          wrap around when reaching the end of the buffer.
 * 
 *          Memory Layout:
 *          [0][1][2][3][4][5][6][7]  <- buffer array
 *                ^           ^
 *               tail        head
 *          Data is read from tail, written at head
 * 
 * @param[out] RB      Pointer to ring buffer structure to initialize
 * @param[in]  buffer  Pointer to memory array for data storage
 * @param[in]  size    Size of the buffer array in bytes
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Initialization successful
 * @retval UART_NULL_POINTER RB or buffer is NULL, or size is 0
 */
UART_enu_err_t ring_buffer_init(UART_ring_buffer_t *RB, uint8_t *buffer, uint16_t size)
{
    /* Validate all parameters */
    if (RB == NULL || buffer == NULL || size == 0)
    {
        return UART_NULL_POINTER;
    }
    
    /* Set buffer pointer and size */
    RB->buffer = buffer;
    RB->size = size;
    
    /* Initialize pointers to beginning (empty buffer) */
    RB->head = 0;   /* Next write position */
    RB->tail = 0;   /* Next read position */
    RB->count = 0;  /* Number of bytes in buffer */

    return UART_OK;
}

/**
 * @brief   Write one byte to ring buffer
 * 
 * @details Writes data at current HEAD position, then advances HEAD.
 *          HEAD pointer wraps around when it reaches end of buffer.
 * 
 * @param[in] RB    Pointer to ring buffer
 * @param[in] data  Byte to write
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Write successful
 * @retval UART_NULL_POINTER RB is NULL
 * @retval UART_BUFFER_FULL  Buffer is full, data not written
 */
UART_enu_err_t ring_buffer_write(UART_ring_buffer_t *RB, uint8_t data)
{
    /* Step 1: Check if buffer pointer is NULL */
    if (RB == NULL)
    {
        return UART_NULL_POINTER;
    }
    
    /* Step 2: Check if buffer is FULL
     * Buffer is full when count equals size */
    if (RB->count == RB->size)
    {
        return UART_BUFFER_FULL;
    }
    
    /* Step 3: Write data at HEAD position */
    RB->buffer[RB->head] = data;
    
    /* Step 4: Move HEAD forward with wrap around
     * Using modulo operation for circular behavior */
    RB->head = (RB->head + 1) % RB->size;
    
    /* Step 5: Increment byte count */
    RB->count++;
    
    /* Step 6: Return success */
    return UART_OK;
}

/**
 * @brief   Read one byte from ring buffer
 * 
 * @details Reads data from current TAIL position, then advances TAIL.
 *          TAIL pointer wraps around when it reaches end of buffer.
 * 
 * @param[in]  RB    Pointer to ring buffer
 * @param[out] data  Pointer to store read byte
 * 
 * @return UART_enu_err_t
 * @retval UART_OK           Read successful
 * @retval UART_NULL_POINTER RB or data is NULL
 * @retval UART_BUFFER_EMPTY Buffer is empty, no data to read
 */
UART_enu_err_t ring_buffer_read(UART_ring_buffer_t *RB, uint8_t *data)
{
    /* Check for NULL pointers */
    if (RB == NULL || data == NULL)
    {
        return UART_NULL_POINTER;
    }
    
    /* Step 3: Check if buffer is EMPTY
     * Buffer is empty when count is 0 */
    if (RB->count == 0)
    {
        return UART_BUFFER_EMPTY;
    }

    /* Step 4: Read data from TAIL position */
    *data = RB->buffer[RB->tail];

    /* Step 5: Move TAIL forward with wrap around */
    RB->tail = (RB->tail + 1) % RB->size;
    
    /* Step 6: Decrement byte count */
    RB->count--;
    
    /* Step 7: Return success */
    return UART_OK;
}

/**
 * @brief   Get number of bytes stored in ring buffer
 * 
 * @param[in] RB  Pointer to ring buffer
 * 
 * @return  Number of bytes in buffer, 0 if RB is NULL
 */
uint16_t ring_buffer_get_count(UART_ring_buffer_t *RB)
{
    if (RB == NULL)
    {
        return 0;
    }
    return RB->count;
}

/**
 * @brief   Get free space available in ring buffer
 * 
 * @param[in] RB  Pointer to ring buffer
 * 
 * @return  Number of bytes that can be written, 0 if RB is NULL
 */
uint16_t ring_buffer_get_free_space(UART_ring_buffer_t *RB)
{
    if (RB == NULL)
    {
        return 0;
    }
    return (RB->size - RB->count);
}

/**
 * @brief   Check if ring buffer is empty
 * 
 * @param[in] RB  Pointer to ring buffer
 * 
 * @return  1 if empty, 0 if not empty or RB is NULL
 */
uint8_t ring_buffer_is_empty(UART_ring_buffer_t *RB)
{
    if (RB == NULL)
    {
        return 0;  /* NULL is treated as not empty to fail-safe */
    }
    return (RB->count == 0) ? 1 : 0;
}

/**
 * @brief   Check if ring buffer is full
 * 
 * @param[in] RB  Pointer to ring buffer
 * 
 * @return  1 if full, 0 if not full or RB is NULL
 */
uint8_t ring_buffer_is_full(UART_ring_buffer_t *RB)
{
    if (RB == NULL)
    {
        return 0;  /* NULL is treated as not full to fail-safe */
    }
    return (RB->count == RB->size) ? 1 : 0;
}

/*============================================================================
 *                    INTERRUPT SERVICE ROUTINES
 *============================================================================*/

/**
 * @brief   Common UART ISR handler for all UART instances
 * 
 * @details This function handles both TX and RX interrupts:
 * 
 *          TX Interrupt (TXE - Transmit Data Register Empty):
 *          - Triggered when DR is empty and ready for next byte
 *          - Reads next byte from TX ring buffer and writes to DR
 *          - Disables TXE interrupt when buffer is empty
 * 
 *          RX Interrupt (RXNE - Receive Data Register Not Empty):
 *          - Triggered when a byte is received
 *          - Reads byte from DR and stores in RX ring buffer
 *          - Handles Overrun Error (ORE) to prevent lockup
 * 
 *          ORE (Overrun Error) Handling:
 *          - ORE occurs when new data arrives before previous was read
 *          - Must be cleared by reading SR then DR (in that order)
 *          - If not cleared, RXNE interrupt will stop working!
 * 
 * @param[in] handle  Pointer to UART handle for the interrupting instance
 * 
 * @note    This is called from instance-specific ISR wrappers
 * @note    Must be as fast as possible - avoid complex operations
 */
static void UART_IRQHandler_Common(UART_Handle_t *handle)
{
    /* Safety check - handle could be NULL if ISR fires before init */
    if (handle == NULL)
    {
        return;
    }

    /* Get pointer to UART hardware registers */
    volatile USART_TypeDef *uart_reg = (volatile USART_TypeDef *)handle->instance;

    /*==================================================================
     * TX INTERRUPT HANDLING
     * 
     * Conditions for TX processing:
     * 1. TXE flag is set (TX data register empty)
     * 2. TXEIE is enabled (we want TX interrupts)
     * 3. tx_busy flag is set (we're in a transmission)
     *==================================================================*/
    if (uart_reg->SR.BIT.TXE == 1 && uart_reg->CR1.BIT.TXEIE == 1 && handle->tx_busy == 1)
    {
        uint8_t byte_to_send;

        /* Try to get next byte from TX buffer */
        if (ring_buffer_read(&handle->tx_buffer, &byte_to_send) == UART_OK)
        {
            /* Write byte to Data Register
             * This automatically clears TXE flag and starts transmission */
            uart_reg->DR.REG = byte_to_send;
        }
        else
        {
            /* TX buffer is empty - transmission complete */
            uart_reg->CR1.BIT.TXEIE = 0;  /* Disable TXE interrupt */
            handle->tx_busy = 0;           /* Mark as idle */
        }
    }

    /*==================================================================
     * RX INTERRUPT HANDLING
     * 
     * Important: Read SR once and use the snapshot to avoid race
     * conditions. Reading SR followed by DR clears both RXNE and ORE.
     * 
     * ORE (Overrun Error) can happen if:
     * - Data arrives faster than we can process
     * - ISR is delayed by higher priority interrupt
     * 
     * ORE MUST be cleared by reading SR then DR, or RXNE will never
     * be set again and UART will appear to stop receiving!
     *==================================================================*/
    
    /* Take a snapshot of Status Register */
    uint32_t sr = uart_reg->SR.REG;

    if (sr & USART_SR_RXNE)
    {
        /* RXNE is set - data available in Data Register */
        
        /* Read received byte from DR
         * This clears RXNE flag. Since we already read SR,
         * this sequence also clears ORE if it was set */
        uint8_t received_byte = (uint8_t)uart_reg->DR.REG;

        /* Store in RX ring buffer if space available */
        if (!ring_buffer_is_full(&handle->rx_buffer))
        {
            ring_buffer_write(&handle->rx_buffer, received_byte);
        }
        /* Note: If buffer is full, byte is silently discarded.
         * Consider adding an overflow counter for debugging */
    }
    else
    {
        /* RXNE not set, but we might have ORE without pending data */
        if (sr & USART_SR_ORE)
        {
            /* Clear ORE by reading DR (we already read SR above)
             * The data read is invalid/duplicate, so discard it */
            volatile uint32_t tmp = uart_reg->DR.REG;
            (void)tmp;  /* Suppress "unused variable" warning */
        }
    }
}

/**
 * @brief   USART1 Interrupt Service Routine
 * @details Calls common handler with USART1 handle
 * @note    Registered in vector table as USART1_IRQHandler
 */
void USART1_IRQHandler(void)
{
    UART_IRQHandler_Common(uart1_handle);
}

/**
 * @brief   USART2 Interrupt Service Routine
 * @details Calls common handler with USART2 handle
 * @note    Registered in vector table as USART2_IRQHandler
 */
void USART2_IRQHandler(void)
{
    UART_IRQHandler_Common(uart2_handle);
}

/**
 * @brief   USART6 Interrupt Service Routine
 * @details Calls common handler with USART6 handle
 * @note    Registered in vector table as USART6_IRQHandler
 */
void USART6_IRQHandler(void)
{
    UART_IRQHandler_Common(uart6_handle);
}

/*============================================================================
 *                           END OF FILE
 *============================================================================*/
