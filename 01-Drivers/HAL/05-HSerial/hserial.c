/**
 * @file    hserial_program.c
 * @brief   High-level Serial (UART) HAL Driver Implementation
 * @details Implements UART communication using DMA for efficient transfers.
 * 
 * @architecture
 *          +-------------+     +-------------+     +-------------+
 *          |   User App  | --> |  HSerial    | --> |    DMA      |
 *          | (callback)  | <-- |   (HAL)     | <-- |   (MCAL)    |
 *          +-------------+     +-------------+     +-------------+
 *                                    |
 *                                    v
 *                              +-------------+
 *                              |    UART     |
 *                              |   (MCAL)    |
 *                              +-------------+
 * 
 * @flow    TX Flow:
 *          1. User calls HSerial_Send()
 *          2. HAL configures DMA (Memory -> UART DR)
 *          3. DMA transfers each byte automatically
 *          4. DMA complete ISR fires
 *          5. HAL wrapper calls user callback
 * 
 *          RX Flow:
 *          1. User calls HSerial_Receive()
 *          2. HAL configures DMA (UART DR -> Memory)
 *          3. Each received byte triggers DMA transfer
 *          4. When count reached, DMA complete ISR fires
 *          5. HAL wrapper calls user callback
 */

#include "../../../include/interface/HAL/hserial_interface.h"
#include "../../../include/interface/MCAL/dma_interface.h"
#include "../../../include/interface/MCAL/uart_Interface.h" 
#include "../../../include/private/MCAL/uart_private.h"
#include "../../../include/interface/MCAL/rcc_init.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"

/*============================================================================
 *                          FORWARD DECLARATIONS
 *============================================================================*/

/*
 * Forward declarations for DMA callback wrapper functions.
 * These are needed because the functions are used in HSerial_Send/Receive
 * but defined at the bottom of this file.
 * 
 * Each UART instance needs separate TX and RX callbacks because:
 * 1. DMA callback signature doesn't include UART info
 * 2. We need to map DMA completion back to correct user callback
 */
static void USART1_TX_DMA_Callback(DMA_Status_t status);
static void USART1_RX_DMA_Callback(DMA_Status_t status);
static void USART2_TX_DMA_Callback(DMA_Status_t status);
static void USART2_RX_DMA_Callback(DMA_Status_t status);
static void USART6_TX_DMA_Callback(DMA_Status_t status);
static void USART6_RX_DMA_Callback(DMA_Status_t status);

/*============================================================================
 *                          PRIVATE DATA
 *============================================================================*/

/*
 * User callback storage
 * --------------------
 * When user calls HSerial_Send/Receive, they provide a callback.
 * We store it here so our DMA callback wrapper can invoke it later.
 * 
 * Why separate variables instead of array?
 * - Clearer code for small number of instances
 * - Avoids indexing errors
 * - Compiler can optimize better
 */
static HSerial_Callback_t USART1_TX_Callback = NULL;
static HSerial_Callback_t USART1_RX_Callback = NULL;
static HSerial_Callback_t USART2_TX_Callback = NULL;
static HSerial_Callback_t USART2_RX_Callback = NULL;
static HSerial_Callback_t USART6_TX_Callback = NULL;
static HSerial_Callback_t USART6_RX_Callback = NULL;

/*
 * Busy flags
 * ----------
 * Prevent multiple simultaneous transfers on same UART/direction.
 * 
 * volatile: Required because flags are modified in ISR context
 *           and read in normal context. Prevents compiler optimization
 *           that might cache the value.
 * 
 * Separate TX/RX: UART can do full-duplex (TX and RX simultaneously),
 *                 but not multiple TX or multiple RX at same time.
 */
static volatile uint8_t USART1_TX_Busy = 0;
static volatile uint8_t USART1_RX_Busy = 0;
static volatile uint8_t USART2_TX_Busy = 0;
static volatile uint8_t USART2_RX_Busy = 0;
static volatile uint8_t USART6_TX_Busy = 0;
static volatile uint8_t USART6_RX_Busy = 0;

/*============================================================================
 *                          PUBLIC FUNCTIONS
 *============================================================================*/

/**
 * @brief Initialize UART peripheral with GPIO and DMA support
 * 
 * @details Initialization sequence:
 *          1. Enable GPIO clock (GPIOA used for all UARTs in this config)
 *          2. Enable UART peripheral clock
 *          3. Configure TX pin (Alternate Function, Push-Pull)
 *          4. Configure RX pin (Alternate Function, Pull-Up)
 *          5. Disable UART (required before configuration)
 *          6. Set baud rate
 *          7. Configure frame format (8N1)
 *          8. Enable TX, RX, and DMA
 *          9. Enable UART
 */
HSerial_Error_t HSerial_Init(HSerial_Instance_t uart, uint32_t baudrate)
{
    /* Pointer to UART registers - volatile because hardware can change values */
    volatile USART_TypeDef *UART;
    
    /* Peripheral clock frequency - needed for baud rate calculation */
    uint32_t periph_clock;
    
    /* GPIO configuration structures */
    GPIO_PinConfig_t tx_pinconfig, rx_pinconfig;
    
    /*
     * Enable GPIOA clock
     * All UART pins in this driver use GPIOA (PA2, PA3, PA9, PA10, PA11, PA12)
     * Must be enabled before configuring any pins
     */
    RCC_AHB1_EnableClock(RCC_AHB1_GPIOA);

    /*
     * Per-instance configuration
     * Each UART has different:
     * - Clock bus (APB1 or APB2) affecting baud rate calculation
     * - GPIO pins
     * - Alternate function number
     */
    switch (uart)
    {
    case HSERIAL_USART1:
        /*
         * USART1 Configuration
         * - Bus: APB2 (higher speed bus)
         * - TX: PA9  (AF7)
         * - RX: PA10 (AF7)
         * - DMA: DMA2 (Stream7/Ch4 for TX, Stream2/Ch4 for RX)
         */
        RCC_APB2_EnableClock(RCC_APB2_USART1);
        UART = USART1;
        periph_clock = RCC_GetPCLK2Frequency();  /* APB2 clock */
        
        /* TX Pin Configuration (PA9) */
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN9;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;      /* UART controls pin */
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL; /* Standard output */
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;          /* Fast transitions */
        tx_pinconfig.PuPd = GPIO_PUPD_NONE;            /* TX doesn't need pull */
        tx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2; /* AF7 = USART1/2 */
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);
        
        /* RX Pin Configuration (PA10) */
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN10;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_PULLUP;          /* Prevent floating when idle */
        rx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);
        break;
        
    case HSERIAL_USART2:
        /*
         * USART2 Configuration
         * - Bus: APB1 (lower speed bus, max 42MHz on F401)
         * - TX: PA2 (AF7)
         * - RX: PA3 (AF7)
         * - DMA: DMA1 (Stream6/Ch4 for TX, Stream5/Ch4 for RX)
         * 
         * Note: USART2 is commonly used for ST-Link Virtual COM Port
         */
        RCC_APB1_EnableClock(RCC_APB1_USART2);
        UART = USART2;
        periph_clock = RCC_GetPCLK1Frequency();  /* APB1 clock */
        
        /* TX Pin Configuration (PA2) */
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN2;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;
        tx_pinconfig.PuPd = GPIO_PUPD_NONE;
        tx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);
        
        /* RX Pin Configuration (PA3) */
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN3;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_PULLUP;
        rx_pinconfig.AlternateFunc = GPIO_AF7_USART1_2;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);
        break;
        
    case HSERIAL_USART6:
        /*
         * USART6 Configuration
         * - Bus: APB2 (higher speed bus)
         * - TX: PA11 (AF8)
         * - RX: PA12 (AF8)
         * - DMA: DMA2 (Stream6/Ch5 for TX, Stream1/Ch5 for RX)
         * 
         * Note: Different AF number (AF8) compared to USART1/2
         */
        RCC_APB2_EnableClock(RCC_APB2_USART6);
        UART = USART6;
        periph_clock = RCC_GetPCLK2Frequency();  /* APB2 clock */

        /* TX Pin Configuration (PA11) */
        tx_pinconfig.Port = GPIOA;
        tx_pinconfig.Pin = GPIO_PIN11;
        tx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        tx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        tx_pinconfig.Speed = GPIO_SPEED_HIGH;
        tx_pinconfig.PuPd = GPIO_PUPD_NONE;
        tx_pinconfig.AlternateFunc = GPIO_AF8_USART6;  /* AF8 for USART6 */
        GPIO_PinInit(tx_pinconfig.Port, &tx_pinconfig);

        /* RX Pin Configuration (PA12) */
        rx_pinconfig.Port = GPIOA;
        rx_pinconfig.Pin = GPIO_PIN12;
        rx_pinconfig.Mode = GPIO_MODE_ALTERNATVE;
        rx_pinconfig.OutputType = GPIO_OUTPUT_PUSHPULL;
        rx_pinconfig.Speed = GPIO_SPEED_HIGH;
        rx_pinconfig.PuPd = GPIO_PUPD_PULLUP;
        rx_pinconfig.AlternateFunc = GPIO_AF8_USART6;
        GPIO_PinInit(rx_pinconfig.Port, &rx_pinconfig);
        break;
        
    default:
        /* Invalid UART instance */
        return HSERIAL_ERROR;
    }
    
    /*
     * UART Configuration
     * Must disable UART before changing settings
     */
    UART->CR1.BIT.UE = 0;  /* Disable UART */
    
    /*
     * Baud Rate Configuration
     * BRR = PCLK / (16 * BAUD) for oversampling by 16
     * uart_calculate_BRR() handles the math including fractional part
     */
    UART->BRR.REG = uart_calculate_BRR(periph_clock, baudrate);
    
    /*
     * Frame Format: 8N1
     * - M=0:   8 data bits
     * - PCE=0: Parity disabled
     * - STOP=0: 1 stop bit
     * 
     * This is the most common UART configuration
     */
    UART->CR1.BIT.M = 0;     /* 8 data bits (0=8bits, 1=9bits) */
    UART->CR1.BIT.PCE = 0;   /* No parity */
    UART->CR2.BIT.STOP = 0;  /* 1 stop bit (00=1, 01=0.5, 10=2, 11=1.5) */
    
    /*
     * Enable Transmitter and Receiver
     * Can be enabled independently for half-duplex operation
     */
    UART->CR1.BIT.TE = 1;  /* Enable transmitter */
    UART->CR1.BIT.RE = 1;  /* Enable receiver */
    
    /*
     * Enable DMA for TX and RX
     * DMAT: DMA request generated when TXE (TX buffer empty) is set
     * DMAR: DMA request generated when RXNE (RX buffer not empty) is set
     */
    UART->CR3.BIT.DMAT = 1;  /* Enable DMA transmitter */
    UART->CR3.BIT.DMAR = 1;  /* Enable DMA receiver */
    
    /* Enable UART - ready for operation */
    UART->CR1.BIT.UE = 1;
    
    return HSERIAL_OK;
}


/**
 * @brief Start asynchronous transmission via DMA
 * 
 * @details DMA Configuration for TX:
 *          - Direction: Memory to Peripheral (M2P)
 *          - Source: User buffer (increments each byte)
 *          - Destination: UART Data Register (fixed address)
 *          - Mode: Normal (single transfer, not circular)
 * 
 * @flow    1. Validate parameters
 *          2. Check if busy
 *          3. Set busy flag + store callback
 *          4. Configure DMA
 *          5. Start DMA
 *          6. Return (transfer happens in background)
 */
HSerial_Error_t HSerial_Send(HSerial_Instance_t uart, uint8_t *buffer, uint16_t size, HSerial_Callback_t callback)
{
    /* Step 1: Validate parameters */
    if (buffer == NULL || callback == NULL)
    {
        return HSERIAL_NULLPTR;
    }
    
    /* Zero-size transfer is technically successful (nothing to do) */
    if (size == 0)
    {
        return HSERIAL_OK;
    }
    
    /* DMA configuration structure */
    DMA_Config_t dma_config;
    
    /* Pointer to our DMA wrapper callback */
    DMA_Cbf_t dma_callback;
    
    /* Pointer to busy flag for this UART */
    volatile uint8_t *busy_flag;
    
    /*
     * Step 2-3: Per-instance setup
     * - Check busy state
     * - Set busy flag
     * - Store user callback
     * - Configure DMA stream/channel/address
     * 
     * DMA Stream/Channel mapping from STM32F4 Reference Manual:
     * USART1_TX: DMA2_Stream7_Channel4
     * USART2_TX: DMA1_Stream6_Channel4
     * USART6_TX: DMA2_Stream6_Channel5
     */
    switch (uart)
    {
    case HSERIAL_USART1:
        busy_flag = &USART1_TX_Busy;
        
        /* Check if previous transfer still in progress */
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        /* Mark as busy and store callback */
        *busy_flag = 1;
        USART1_TX_Callback = callback;
        
        /* USART1 TX DMA Configuration */
        dma_config.DMA = DMA_2;
        dma_config.Stream = DMA_STREAM7;
        dma_config.Channel = DMA_CHANNEL4;
        dma_config.Peripheral_Address = (uint32_t)&USART1->DR;
        dma_callback = USART1_TX_DMA_Callback;
        break;
        
    case HSERIAL_USART2:
        busy_flag = &USART2_TX_Busy;
        
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        *busy_flag = 1;
        USART2_TX_Callback = callback;
        
        /* USART2 TX DMA Configuration */
        dma_config.DMA = DMA_1;
        dma_config.Stream = DMA_STREAM6;
        dma_config.Channel = DMA_CHANNEL4;
        dma_config.Peripheral_Address = (uint32_t)&USART2->DR;
        dma_callback = USART2_TX_DMA_Callback;
        break;
        
    case HSERIAL_USART6:
        busy_flag = &USART6_TX_Busy;
        
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        *busy_flag = 1;
        USART6_TX_Callback = callback;
        
        /* USART6 TX DMA Configuration */
        dma_config.DMA = DMA_2;
        dma_config.Stream = DMA_STREAM6;
        dma_config.Channel = DMA_CHANNEL5;
        dma_config.Peripheral_Address = (uint32_t)&USART6->DR;
        dma_callback = USART6_TX_DMA_Callback;
        break;
        
    default:
        return HSERIAL_ERROR;
    }
    
    /*
     * Step 4: Configure common DMA parameters
     */
    
    /* Direction: Memory -> Peripheral (we're sending data OUT) */
    dma_config.Dir = DMA_DIR_M2P;
    
    /* Source address: user's buffer */
    dma_config.Memory_Address = (uint32_t)buffer;
    
    /* Number of bytes to transfer */
    dma_config.Transfer_Count = size;
    
    /* 
     * Data size: Byte (8-bit)
     * UART data register is 8-bit for 8N1 configuration
     */
    dma_config.Periphera_DataSize = DMA_SIZE_BYTE;
    dma_config.Memory_DataSize = DMA_SIZE_BYTE;
    
    /*
     * Address increment:
     * - Peripheral: NO (always write to same DR register)
     * - Memory: YES (move through buffer byte by byte)
     */
    dma_config.Peripheral_INC = DMA_INC_NO;
    dma_config.Memory_INC = DMA_INC_YES;
    
    /*
     * Mode: Normal (not circular)
     * Transfer once and stop. Circular would repeat forever.
     */
    dma_config.Mode = DMA_MODE_NORMAL;
    
    /* Priority: High (UART is time-sensitive) */
    dma_config.Priority = DMA_PRIORITY_HIGH;
    
    /* Step 5: Initialize DMA with our configuration */
    if (DMA_Init(&dma_config) != DMA_OK)
    {
        *busy_flag = 0;  /* Clear busy on failure */
        return HSERIAL_ERROR;
    }
    
    /* Step 6: Start the DMA transfer */
    if (DMA_Start(&dma_config, dma_callback) != DMA_OK)
    {
        *busy_flag = 0;  /* Clear busy on failure */
        return HSERIAL_ERROR;
    }
    
    /*
     * Success - transfer is now running in background
     * User's callback will be called when complete
     */
    return HSERIAL_OK;
}


/**
 * @brief Start asynchronous reception via DMA
 * 
 * @details DMA Configuration for RX:
 *          - Direction: Peripheral to Memory (P2M)
 *          - Source: UART Data Register (fixed address)
 *          - Destination: User buffer (increments each byte)
 *          - Mode: Normal (stops after receiving 'size' bytes)
 * 
 * @note    This waits for EXACTLY 'size' bytes before calling callback.
 *          For protocols with variable-length messages, consider:
 *          - Using IDLE line detection interrupt
 *          - Using circular DMA with periodic polling
 *          - Using a larger buffer and parsing in callback
 */
HSerial_Error_t HSerial_Receive(HSerial_Instance_t uart, uint8_t *buffer, uint16_t size, HSerial_Callback_t callback)
{
    /* Step 1: Validate parameters */
    if (buffer == NULL || callback == NULL)
    {
        return HSERIAL_NULLPTR;
    }
    
    if (size == 0)
    {
        return HSERIAL_OK;
    }
    
    DMA_Config_t dma_config;
    DMA_Cbf_t dma_callback;
    volatile uint8_t *busy_flag;
    
    /*
     * Step 2-3: Per-instance setup
     * 
     * DMA Stream/Channel mapping for RX:
     * USART1_RX: DMA2_Stream2_Channel4 (or Stream5)
     * USART2_RX: DMA1_Stream5_Channel4
     * USART6_RX: DMA2_Stream1_Channel5 (or Stream2)
     */
    switch (uart)
    {
    case HSERIAL_USART1:
        busy_flag = &USART1_RX_Busy;
        
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        *busy_flag = 1;
        USART1_RX_Callback = callback;
        
        /* USART1 RX DMA Configuration */
        dma_config.DMA = DMA_2;
        dma_config.Stream = DMA_STREAM2;
        dma_config.Channel = DMA_CHANNEL4;
        dma_config.Peripheral_Address = (uint32_t)&USART1->DR;
        dma_callback = USART1_RX_DMA_Callback;
        break;
        
    case HSERIAL_USART2:
        busy_flag = &USART2_RX_Busy;
        
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        *busy_flag = 1;
        USART2_RX_Callback = callback;
        
        /* USART2 RX DMA Configuration */
        dma_config.DMA = DMA_1;
        dma_config.Stream = DMA_STREAM5;
        dma_config.Channel = DMA_CHANNEL4;
        dma_config.Peripheral_Address = (uint32_t)&USART2->DR;
        dma_callback = USART2_RX_DMA_Callback;
        break;
        
    case HSERIAL_USART6:
        busy_flag = &USART6_RX_Busy;
        
        if (*busy_flag)
        {
            return HSERIAL_BUSY;
        }
        
        *busy_flag = 1;
        USART6_RX_Callback = callback;
        
        /* USART6 RX DMA Configuration */
        dma_config.DMA = DMA_2;
        dma_config.Stream = DMA_STREAM1;
        dma_config.Channel = DMA_CHANNEL5;
        dma_config.Peripheral_Address = (uint32_t)&USART6->DR;
        dma_callback = USART6_RX_DMA_Callback;
        break;
        
    default:
        return HSERIAL_ERROR;
    }
    
    /*
     * Step 4: Configure common DMA parameters
     */
    
    /* Direction: Peripheral -> Memory (receiving data IN) */
    dma_config.Dir = DMA_DIR_P2M;
    
    /* Destination address: user's buffer */
    dma_config.Memory_Address = (uint32_t)buffer;
    
    /* Number of bytes to receive */
    dma_config.Transfer_Count = size;
    
    /* Data sizes: Byte */
    dma_config.Periphera_DataSize = DMA_SIZE_BYTE;
    dma_config.Memory_DataSize = DMA_SIZE_BYTE;
    
    /*
     * Address increment:
     * - Peripheral: NO (always read from same DR register)
     * - Memory: YES (fill buffer sequentially)
     */
    dma_config.Peripheral_INC = DMA_INC_NO;
    dma_config.Memory_INC = DMA_INC_YES;
    
    /* Mode: Normal (receive fixed number of bytes then stop) */
    dma_config.Mode = DMA_MODE_NORMAL;
    
    dma_config.Priority = DMA_PRIORITY_HIGH;
    
    /* Step 5: Initialize DMA */
    if (DMA_Init(&dma_config) != DMA_OK)
    {
        *busy_flag = 0;
        return HSERIAL_ERROR;
    }
    
    /* Step 6: Start DMA */
    if (DMA_Start(&dma_config, dma_callback) != DMA_OK)
    {
        *busy_flag = 0;
        return HSERIAL_ERROR;
    }
    
    return HSERIAL_OK;
}

/*============================================================================
 *                      DMA CALLBACK WRAPPERS
 *============================================================================*/

/*
 * These functions bridge DMA callbacks to user callbacks.
 * 
 * Why are these needed?
 * 1. DMA driver uses its own status enum (DMA_Status_t)
 * 2. User expects HAL status enum (HSerial_Status_t)
 * 3. DMA callback doesn't know which UART triggered it
 * 4. We need to clear busy flags
 * 
 * Flow:
 *    DMA Transfer Complete
 *           |
 *           v
 *    DMA ISR (in dma_program.c)
 *           |
 *           v
 *    USARTx_TX/RX_DMA_Callback() <-- This function
 *           |
 *           +-- Clear busy flag
 *           +-- Convert DMA status to HSerial status
 *           v
 *    User's callback function
 * 
 * WARNING: These run in interrupt context!
 *          Keep them short, don't call blocking functions.
 */

/**
 * @brief DMA callback wrapper for USART1 TX
 * @param status DMA transfer result (DMA_COMPLETE or error)
 */
static void USART1_TX_DMA_Callback(DMA_Status_t status)
{
    /* Clear busy flag - transfer is done (success or fail) */
    USART1_TX_Busy = 0;
    
    /* Call user's callback if registered */
    if (USART1_TX_Callback != NULL)
    {
        /* Convert DMA status to HSerial status */
        USART1_TX_Callback((status == DMA_COMPLETE) ? HSERIAL_TX_COMPLETE : HSERIAL_TX_ERROR);
    }
}

/**
 * @brief DMA callback wrapper for USART1 RX
 * @param status DMA transfer result
 */
static void USART1_RX_DMA_Callback(DMA_Status_t status)
{
    USART1_RX_Busy = 0;
    
    if (USART1_RX_Callback != NULL)
    {
        USART1_RX_Callback((status == DMA_COMPLETE) ? HSERIAL_RX_COMPLETE : HSERIAL_RX_ERROR);
    }
}

/**
 * @brief DMA callback wrapper for USART2 TX
 * @param status DMA transfer result
 */
static void USART2_TX_DMA_Callback(DMA_Status_t status)
{
    USART2_TX_Busy = 0;
    
    if (USART2_TX_Callback != NULL)
    {
        USART2_TX_Callback((status == DMA_COMPLETE) ? HSERIAL_TX_COMPLETE : HSERIAL_TX_ERROR);
    }
}

/**
 * @brief DMA callback wrapper for USART2 RX
 * @param status DMA transfer result
 */
static void USART2_RX_DMA_Callback(DMA_Status_t status)
{
    USART2_RX_Busy = 0;
    
    if (USART2_RX_Callback != NULL)
    {
        USART2_RX_Callback((status == DMA_COMPLETE) ? HSERIAL_RX_COMPLETE : HSERIAL_RX_ERROR);
    }
}

/**
 * @brief DMA callback wrapper for USART6 TX
 * @param status DMA transfer result
 */
static void USART6_TX_DMA_Callback(DMA_Status_t status)
{
    USART6_TX_Busy = 0;
    
    if (USART6_TX_Callback != NULL)
    {
        USART6_TX_Callback((status == DMA_COMPLETE) ? HSERIAL_TX_COMPLETE : HSERIAL_TX_ERROR);
    }
}

/**
 * @brief DMA callback wrapper for USART6 RX
 * @param status DMA transfer result
 */
static void USART6_RX_DMA_Callback(DMA_Status_t status)
{
    USART6_RX_Busy = 0;
    
    if (USART6_RX_Callback != NULL)
    {
        USART6_RX_Callback((status == DMA_COMPLETE) ? HSERIAL_RX_COMPLETE : HSERIAL_RX_ERROR);
    }
}

/*============================================================================
 *                          END OF FILE
 *============================================================================*/