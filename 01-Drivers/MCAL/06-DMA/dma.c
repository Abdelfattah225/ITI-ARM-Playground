
#include "../../../include/private/MCAL/dma_private.h"
#include "../../../include/interface/MCAL/dma_interface.h"

#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/MCAL/NVIC_Interface.h"

/*============================================================================
 *                      DMA DRIVER FOR STM32F4xx
 *============================================================================
 * This driver provides an abstraction layer for the DMA (Direct Memory Access)
 * peripheral on STM32F4xx microcontrollers.
 * 
 * Key Features:
 * - Supports DMA1 and DMA2 controllers
 * - 8 streams per controller (Stream 0-7)
 * - 8 channels per stream (Channel 0-7)
 * - Memory-to-Memory, Memory-to-Peripheral, Peripheral-to-Memory transfers
 * - Interrupt-driven with callback support
 * - Circular and Normal transfer modes
 * 
 * Hardware Notes:
 * - DMA1: Cannot perform Memory-to-Memory transfers (hardware limitation)
 * - DMA2: Supports all transfer directions including M2M
 * - Circular mode: Cannot be used with M2M transfers
 *============================================================================*/

/*============================================================================
 *                          PRIVATE CONSTANTS
 *============================================================================*/

/**
 * @brief Bit masks to clear all interrupt flags for each stream
 * 
 * These masks are used to clear the interrupt flags in LIFCR/HIFCR registers.
 * Each stream has 5 flags that need to be cleared:
 * - TCIF (Transfer Complete)
 * - HTIF (Half Transfer)
 * - TEIF (Transfer Error)
 * - DMEIF (Direct Mode Error)
 * - FEIF (FIFO Error)
 * 
 * LIFCR handles Streams 0-3, HIFCR handles Streams 4-7
 * The pattern repeats: Stream 0 & 4 use same bit positions, etc.
 */
const uint32_t STREAM_CLEAR_MASK[] = {
    0x0000003D,  // Stream 0/4: bits 0,2,3,4,5 (FEIF0, DMEIF0, TEIF0, HTIF0, TCIF0)
    0x00000F40,  // Stream 1/5: bits 6,8,9,10,11
    0x003D0000,  // Stream 2/6: bits 16,18,19,20,21
    0x0F400000   // Stream 3/7: bits 22,24,25,26,27
};

/*============================================================================
 *                          PRIVATE VARIABLES
 *============================================================================*/

/**
 * @brief Callback function pointers for DMA1 streams
 * 
 * Array of 8 function pointers (one per stream) that store user-defined
 * callback functions. These are called from ISR when transfer completes.
 * Initialized to NULL for safety.
 */
static DMA_Cbf_t DMA1_Callbacks[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/**
 * @brief Callback function pointers for DMA2 streams
 */
static DMA_Cbf_t DMA2_Callbacks[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/**
 * @brief IRQ numbers for DMA1 streams
 * 
 * Maps stream index to corresponding NVIC IRQ number.
 * Note: Stream 7 has a much higher IRQ number (47) due to STM32 interrupt mapping.
 */
static const IRQn_Type DMA1_IRQn[8] = {    
    DMA1_Stream0_IRQn,  // IRQ 11
    DMA1_Stream1_IRQn,  // IRQ 12
    DMA1_Stream2_IRQn,  // IRQ 13
    DMA1_Stream3_IRQn,  // IRQ 14
    DMA1_Stream4_IRQn,  // IRQ 15
    DMA1_Stream5_IRQn,  // IRQ 16
    DMA1_Stream6_IRQn,  // IRQ 17
    DMA1_Stream7_IRQn   // IRQ 47 (non-contiguous!)
};

/**
 * @brief IRQ numbers for DMA2 streams
 * 
 * Note: Streams 5-7 have non-contiguous IRQ numbers (68-70).
 */
static const IRQn_Type DMA2_IRQn[8] = {
    DMA2_Stream0_IRQn,  // IRQ 56
    DMA2_Stream1_IRQn,  // IRQ 57
    DMA2_Stream2_IRQn,  // IRQ 58
    DMA2_Stream3_IRQn,  // IRQ 59
    DMA2_Stream4_IRQn,  // IRQ 60
    DMA2_Stream5_IRQn,  // IRQ 68 (non-contiguous!)
    DMA2_Stream6_IRQn,  // IRQ 69
    DMA2_Stream7_IRQn   // IRQ 70
};

/**
 * @brief TCIF (Transfer Complete Interrupt Flag) bit masks
 * 
 * Used to check if transfer complete flag is set in LISR/HISR.
 * Index 0 = Streams 0 & 4, Index 1 = Streams 1 & 5, etc.
 */
static const uint32_t TCIF_MASK[] = {
    (1UL << 5),   // Stream 0/4: bit 5
    (1UL << 11),  // Stream 1/5: bit 11
    (1UL << 21),  // Stream 2/6: bit 21
    (1UL << 27)   // Stream 3/7: bit 27
};

/**
 * @brief TEIF (Transfer Error Interrupt Flag) bit masks
 * 
 * Used to check if transfer error flag is set in LISR/HISR.
 */
static const uint32_t TEIF_MASK[] = {
    (1UL << 3),   // Stream 0/4: bit 3
    (1UL << 9),   // Stream 1/5: bit 9
    (1UL << 19),  // Stream 2/6: bit 19
    (1UL << 25)   // Stream 3/7: bit 25
};

/*============================================================================
 *                          PUBLIC FUNCTIONS
 *============================================================================*/

/**
 * @brief Initialize a DMA stream with the specified configuration
 * 
 * This function configures a DMA stream for data transfer. It performs:
 * 1. Parameter validation
 * 2. Clock enabling for the DMA peripheral
 * 3. Stream disabling (required before configuration)
 * 4. Flag clearing
 * 5. Register configuration
 * 
 * @param config Pointer to DMA configuration structure
 * @return DMA_Error_t DMA_OK on success, error code otherwise
 * 
 * @note The stream must be disabled before calling DMA_Start()
 * @note DMA1 does not support Memory-to-Memory transfers
 * @note Circular mode cannot be used with M2M transfers
 * 
 * Configuration sequence (per STM32 Reference Manual):
 * 1. Disable the stream (EN = 0)
 * 2. Wait until EN bit reads 0 (hardware confirms stream is disabled)
 * 3. Clear all interrupt flags
 * 4. Configure all stream registers
 */
DMA_Error_t DMA_Init(const DMA_Config_t *config)
{
    /*------------------------------------------------------------------------
     * STEP 1: NULL POINTER VALIDATION
     * Ensure the configuration pointer is valid before dereferencing
     *------------------------------------------------------------------------*/
    if (config == NULL)
    {
        return DMA_NULLPTR;
    }
    
    /*------------------------------------------------------------------------
     * STEP 2: HARDWARE CAPABILITY VALIDATION
     * 
     * Check 1: DMA1 cannot perform Memory-to-Memory transfers
     *          This is a hardware limitation of STM32F4xx
     * 
     * Check 2: Circular mode is incompatible with M2M transfers
     *          In M2M mode, there's no external trigger, so circular
     *          would cause infinite transfer
     *------------------------------------------------------------------------*/
    if ((config->Dir == DMA_DIR_M2M && config->DMA == DMA_1) || 
        (config->Dir == DMA_DIR_M2M && config->Mode == DMA_MODE_CIRCULAR))
    {
        return DMA_NOT_SUPPORTED;
    }
    
    /*------------------------------------------------------------------------
     * STEP 3: ENABLE DMA CLOCK
     * 
     * The DMA peripheral clock must be enabled before accessing any
     * DMA registers. Clock is on AHB1 bus for both DMA1 and DMA2.
     *------------------------------------------------------------------------*/
    DMA_TypeDef *DMA;
    if (config->DMA == DMA_1)
    {
        RCC_AHB1_EnableClock(RCC_AHB1_DMA1);
        DMA = DMA1;  // Get base address of DMA1 peripheral
    }
    else if (config->DMA == DMA_2)
    {
        RCC_AHB1_EnableClock(RCC_AHB1_DMA2);
        DMA = DMA2;  // Get base address of DMA2 peripheral
    }
    
    /*------------------------------------------------------------------------
     * STEP 4-5: DISABLE STREAM AND WAIT FOR CONFIRMATION
     * 
     * Before configuring a stream, it MUST be disabled. The EN bit
     * is cleared by software, but hardware may take time to complete
     * any ongoing transfer. We poll until hardware confirms the stream
     * is fully disabled (EN reads as 0).
     * 
     * WARNING: This could hang if DMA is stuck. Consider adding timeout.
     *------------------------------------------------------------------------*/
    DMA->STREAM[config->Stream].CR.BIT.EN = 0;
    while (DMA->STREAM[config->Stream].CR.BIT.EN == 1)
    {
        // Busy-wait until hardware clears EN bit
        // Hardware automatically clears when transfer is safely stopped
    }
   
    /*------------------------------------------------------------------------
     * STEP 6: CLEAR ALL INTERRUPT FLAGS
     * 
     * Clearing flags prevents spurious interrupts when stream is enabled.
     * LIFCR (Low Interrupt Flag Clear Register): Streams 0-3
     * HIFCR (High Interrupt Flag Clear Register): Streams 4-7
     * 
     * Writing 1 to a flag bit clears it (write-1-to-clear mechanism).
     *------------------------------------------------------------------------*/
    if (config->Stream < 4)
    {
        // Streams 0-3 use Low register
        DMA->LIFCR.Reg = STREAM_CLEAR_MASK[config->Stream];
    }
    else
    {
        // Streams 4-7 use High register, but same bit pattern as 0-3
        DMA->HIFCR.Reg = STREAM_CLEAR_MASK[config->Stream - 4];
    }
    
    /*------------------------------------------------------------------------
     * STEP 7: CONFIGURE STREAM REGISTERS
     * 
     * CR (Control Register) configuration:
     * - CHSEL: Channel selection (0-7) - determines which peripheral request
     * - DIR: Data transfer direction (P2M, M2P, M2M)
     * - CIRC: Circular mode enable
     * - PINC: Peripheral address increment enable
     * - MINC: Memory address increment enable
     * - PSIZE: Peripheral data width (byte, half-word, word)
     * - MSIZE: Memory data width (byte, half-word, word)
     * - PL: Priority level (low, medium, high, very high)
     * 
     * NDTR: Number of data items to transfer (16-bit value)
     * PAR: Peripheral address (or source address for M2M)
     * M0AR: Memory 0 address (or destination address for M2M)
     *------------------------------------------------------------------------*/
    DMA->STREAM[config->Stream].CR.BIT.CHSEL = config->Channel;
    DMA->STREAM[config->Stream].CR.BIT.DIR   = config->Dir;       
    DMA->STREAM[config->Stream].CR.BIT.CIRC  = config->Mode;
    DMA->STREAM[config->Stream].CR.BIT.PINC  = config->Peripheral_INC;
    DMA->STREAM[config->Stream].CR.BIT.MINC  = config->Memory_INC;
    DMA->STREAM[config->Stream].CR.BIT.PSIZE = config->Periphera_DataSize;
    DMA->STREAM[config->Stream].CR.BIT.MSIZE = config->Memory_DataSize;
    DMA->STREAM[config->Stream].CR.BIT.PL    = config->Priority;
    DMA->STREAM[config->Stream].NDTR.Reg     = config->Transfer_Count;
    DMA->STREAM[config->Stream].PAR.Reg      = config->Peripheral_Address;
    DMA->STREAM[config->Stream].M0AR.Reg     = config->Memory_Address;

    /*------------------------------------------------------------------------
     * STEP 8: RETURN SUCCESS
     *------------------------------------------------------------------------*/
    return DMA_OK;

    /*------------------------------------------------------------------------
     * DESIGN NOTES / TODO:
     * 
     * Q: For M2M transfers, how should PAR and M0AR be interpreted?
     * A: In M2M mode, PAR = source address, M0AR = destination address
     *    The current implementation uses Peripheral_Address as source
     *    and Memory_Address as destination (see DMA_MemCpy).
     * 
     * Q: What about circular buffer with M2M?
     * A: Circular + M2M is not allowed (validated in Step 2) because
     *    M2M has no external trigger to restart the circular transfer.
     *------------------------------------------------------------------------*/
}


/**
 * @brief Start a DMA transfer with interrupt-driven completion notification
 * 
 * This function enables interrupts and starts the DMA transfer.
 * When transfer completes, the provided callback function is invoked
 * from the interrupt context.
 * 
 * @param config Pointer to DMA configuration (must match what was passed to DMA_Init)
 * @param cbf Callback function to be called on transfer completion
 * @return DMA_Error_t DMA_OK on success, error code otherwise
 * 
 * @note DMA_Init() must be called before DMA_Start()
 * @note Callback is executed in interrupt context - keep it short!
 * @note For polling mode, use DMA_GetTransferStatus() instead of callbacks
 */
DMA_Error_t DMA_Start(const DMA_Config_t *config, DMA_Cbf_t cbf)
{
    /*------------------------------------------------------------------------
     * STEP 1: PARAMETER VALIDATION
     * Both config and callback must be valid
     *------------------------------------------------------------------------*/
    if (config == NULL || cbf == NULL)
    {
        return DMA_NULLPTR;
    }

    DMA_TypeDef *DMA;
    
    /*------------------------------------------------------------------------
     * STEP 2: STORE CALLBACK AND STEP 4: ENABLE NVIC
     * 
     * The callback is stored in the appropriate array based on DMA controller.
     * NVIC must be enabled for the specific stream's IRQ to allow interrupts.
     *------------------------------------------------------------------------*/
    if (config->DMA == DMA_1)
    {
        DMA = DMA1;
        DMA1_Callbacks[config->Stream] = cbf;      // Store callback
        NVIC_EnableIRQ(DMA1_IRQn[config->Stream]); // Enable NVIC for this stream
    }
    else if (config->DMA == DMA_2)
    {
        DMA = DMA2;
        DMA2_Callbacks[config->Stream] = cbf;      // Store callback
        NVIC_EnableIRQ(DMA2_IRQn[config->Stream]); // Enable NVIC for this stream
    }
    
    /*------------------------------------------------------------------------
     * STEP 3: ENABLE TRANSFER COMPLETE INTERRUPT
     * 
     * TCIE (Transfer Complete Interrupt Enable) generates an interrupt
     * when the DMA transfer finishes (NDTR reaches 0 or error occurs).
     *------------------------------------------------------------------------*/
    DMA->STREAM[config->Stream].CR.BIT.TCIE = 1;
    
    /*------------------------------------------------------------------------
     * STEP 5: ENABLE STREAM (START TRANSFER)
     * 
     * Setting EN = 1 starts the DMA transfer. The transfer will proceed
     * automatically based on the configured direction:
     * - P2M: Waits for peripheral request, then transfers
     * - M2P: Waits for peripheral request, then transfers
     * - M2M: Starts immediately (no external trigger)
     *------------------------------------------------------------------------*/
    DMA->STREAM[config->Stream].CR.BIT.EN = 1;
    
    /*------------------------------------------------------------------------
     * STEP 6: RETURN SUCCESS
     * Transfer is now in progress, callback will be invoked on completion
     *------------------------------------------------------------------------*/
    return DMA_OK;
}


/*============================================================================
 *                          PRIVATE FUNCTIONS
 *============================================================================*/

/**
 * @brief Common interrupt handler for all DMA streams
 * 
 * This function is called by all individual stream IRQ handlers.
 * It checks the transfer complete flag, clears it, and invokes
 * the user callback if registered.
 * 
 * @param DMA Pointer to DMA peripheral (DMA1 or DMA2)
 * @param stream Stream number (0-7)
 * @param callbacks Pointer to callback array for this DMA controller
 * 
 * @note Executed in interrupt context
 * @note Uses switch-case instead of computed bit positions for clarity
 *       and to handle the non-uniform bit layout in status registers
 * 
 * TODO: Add support for other interrupt flags:
 *       - HTIF (Half Transfer) - useful for double buffering
 *       - TEIF (Transfer Error) - for error handling
 *       - DMEIF (Direct Mode Error)
 *       - FEIF (FIFO Error)
 */
static void DMA_IRQ_Common(DMA_TypeDef *DMA, uint8_t stream, DMA_Cbf_t *callbacks)
{
    /*------------------------------------------------------------------------
     * Check and clear Transfer Complete flag for the specific stream.
     * 
     * LISR/LIFCR: Streams 0-3
     * HISR/HIFCR: Streams 4-7
     * 
     * The switch handles each stream individually because the bit positions
     * are not uniformly spaced (there are reserved bits between flags).
     *------------------------------------------------------------------------*/
    switch (stream)
    {
    case DMA_STREAM0:
        if (DMA->LISR.BIT.TCIF0 == 1)  // Check Transfer Complete flag
        {
            DMA->LIFCR.BIT.CTCIF0 = 1; // Clear flag (write-1-to-clear)
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);  // Invoke user callback
            }
        }
        break;

    case DMA_STREAM1:
        if (DMA->LISR.BIT.TCIF1 == 1)
        {
            DMA->LIFCR.BIT.CTCIF1 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM2:
        if (DMA->LISR.BIT.TCIF2 == 1)
        {
            DMA->LIFCR.BIT.CTCIF2 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM3:
        if (DMA->LISR.BIT.TCIF3 == 1)
        {
            DMA->LIFCR.BIT.CTCIF3 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM4:
        if (DMA->HISR.BIT.TCIF4 == 1)  // Streams 4-7 use HISR/HIFCR
        {
            DMA->HIFCR.BIT.CTCIF4 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM5:
        if (DMA->HISR.BIT.TCIF5 == 1)
        {
            DMA->HIFCR.BIT.CTCIF5 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM6:
        if (DMA->HISR.BIT.TCIF6 == 1)
        {
            DMA->HIFCR.BIT.CTCIF6 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    case DMA_STREAM7:
        if (DMA->HISR.BIT.TCIF7 == 1)
        {
            DMA->HIFCR.BIT.CTCIF7 = 1;
            if (callbacks[stream] != NULL)
            {
                callbacks[stream](DMA_COMPLETE);
            }
        }
        break;

    default:
        // Invalid stream number - should never reach here
        break;
    }
}


/*============================================================================
 *                          INTERRUPT HANDLERS
 *============================================================================*/

/*
 * DMA1 Stream Interrupt Handlers
 * 
 * Each stream has its own IRQ handler that delegates to the common handler.
 * These must match the names in the startup file's vector table.
 */
void DMA1_Stream0_IRQHandler(void) { DMA_IRQ_Common(DMA1, 0, DMA1_Callbacks); }
void DMA1_Stream1_IRQHandler(void) { DMA_IRQ_Common(DMA1, 1, DMA1_Callbacks); }
void DMA1_Stream2_IRQHandler(void) { DMA_IRQ_Common(DMA1, 2, DMA1_Callbacks); }
void DMA1_Stream3_IRQHandler(void) { DMA_IRQ_Common(DMA1, 3, DMA1_Callbacks); }
void DMA1_Stream4_IRQHandler(void) { DMA_IRQ_Common(DMA1, 4, DMA1_Callbacks); }
void DMA1_Stream5_IRQHandler(void) { DMA_IRQ_Common(DMA1, 5, DMA1_Callbacks); }
void DMA1_Stream6_IRQHandler(void) { DMA_IRQ_Common(DMA1, 6, DMA1_Callbacks); }
void DMA1_Stream7_IRQHandler(void) { DMA_IRQ_Common(DMA1, 7, DMA1_Callbacks); }

/*
 * DMA2 Stream Interrupt Handlers
 */
void DMA2_Stream0_IRQHandler(void) { DMA_IRQ_Common(DMA2, 0, DMA2_Callbacks); }
void DMA2_Stream1_IRQHandler(void) { DMA_IRQ_Common(DMA2, 1, DMA2_Callbacks); }
void DMA2_Stream2_IRQHandler(void) { DMA_IRQ_Common(DMA2, 2, DMA2_Callbacks); }
void DMA2_Stream3_IRQHandler(void) { DMA_IRQ_Common(DMA2, 3, DMA2_Callbacks); }
void DMA2_Stream4_IRQHandler(void) { DMA_IRQ_Common(DMA2, 4, DMA2_Callbacks); }
void DMA2_Stream5_IRQHandler(void) { DMA_IRQ_Common(DMA2, 5, DMA2_Callbacks); }
void DMA2_Stream6_IRQHandler(void) { DMA_IRQ_Common(DMA2, 6, DMA2_Callbacks); }
void DMA2_Stream7_IRQHandler(void) { DMA_IRQ_Common(DMA2, 7, DMA2_Callbacks); }


/**
 * @brief Get the number of data items transferred
 * 
 * Calculates how many data items have been transferred by subtracting
 * the remaining count (NDTR) from the original count.
 * 
 * @param config Pointer to DMA configuration
 * @param NoOfDataTransferred Pointer to store the result
 * @return DMA_Error_t DMA_OK on success, error code otherwise
 * 
 * @note The unit depends on PSIZE/MSIZE configuration:
 *       - BYTE: count is in bytes
 *       - HALF_WORD: count is in 16-bit half-words
 *       - WORD: count is in 32-bit words
 * 
 * @note Can be called while transfer is in progress for progress monitoring
 */
DMA_Error_t DMA_GetTransferDataNum(const DMA_Config_t *config, uint16_t *NoOfDataTransferred)
{
    /*------------------------------------------------------------------------
     * STEP 1: PARAMETER VALIDATION
     *------------------------------------------------------------------------*/
    if (config == NULL || NoOfDataTransferred == NULL)
    {
        return DMA_NULLPTR;
    }
    
    /*------------------------------------------------------------------------
     * STEP 2: GET DMA PERIPHERAL POINTER
     *------------------------------------------------------------------------*/
    DMA_TypeDef *DMA;
    if (config->DMA == DMA_1)
    {
        DMA = DMA1;
    }
    else if (config->DMA == DMA_2)
    {
        DMA = DMA2;
    }
    
    /*------------------------------------------------------------------------
     * STEP 3: READ NDTR (Number of Data to Transfer Register)
     * 
     * NDTR is decremented after each data transfer. It shows how many
     * items are remaining to be transferred.
     *------------------------------------------------------------------------*/
    uint16_t remaining = (uint16_t)DMA->STREAM[config->Stream].NDTR.Reg;
    
    /*------------------------------------------------------------------------
     * STEP 4-5: CALCULATE AND STORE TRANSFERRED COUNT
     * 
     * transferred = original_count - remaining_count
     *------------------------------------------------------------------------*/
    *NoOfDataTransferred = config->Transfer_Count - remaining;
    
    /*------------------------------------------------------------------------
     * STEP 6: RETURN SUCCESS
     *------------------------------------------------------------------------*/
    return DMA_OK;
}


/**
 * @brief Get the current status of a DMA transfer
 * 
 * Checks the status flags and EN bit to determine the current state
 * of the DMA stream.
 * 
 * @param config Pointer to DMA configuration
 * @param status Pointer to store the status result
 * @return DMA_Error_t DMA_OK on success, error code otherwise
 * 
 * Status values:
 * - DMA_IDLE: Stream is disabled and no flags set
 * - DMA_RUNNING: Stream is enabled (transfer in progress)
 * - DMA_COMPLETE: Transfer complete flag is set
 * - DMA_ERROR: Transfer error flag is set
 * 
 * @note Error status takes priority over complete status
 */
DMA_Error_t DMA_GetTransferStatus(const DMA_Config_t *config, DMA_Status_t *status)
{
    /*------------------------------------------------------------------------
     * STEP 1: PARAMETER VALIDATION
     *------------------------------------------------------------------------*/
    if (config == NULL || status == NULL)
    {
        return DMA_NULLPTR;
    }
    
    /*------------------------------------------------------------------------
     * STEP 2: GET DMA PERIPHERAL POINTER
     *------------------------------------------------------------------------*/
    DMA_TypeDef *DMA;
    if (config->DMA == DMA_1)
    {
        DMA = DMA1;
    }
    else
    {
        DMA = DMA2;
    }
    
    /*------------------------------------------------------------------------
     * STEP 3: SELECT CORRECT STATUS REGISTER AND MASK INDEX
     * 
     * LISR: Status for Streams 0-3
     * HISR: Status for Streams 4-7
     * 
     * The mask_index maps to the TCIF_MASK/TEIF_MASK arrays where:
     * - Index 0: Streams 0 and 4
     * - Index 1: Streams 1 and 5
     * - Index 2: Streams 2 and 6
     * - Index 3: Streams 3 and 7
     *------------------------------------------------------------------------*/
    uint32_t status_reg;
    uint8_t mask_index;
    
    if (config->Stream < 4)
    {
        status_reg = DMA->LISR.Reg;
        mask_index = config->Stream;
    }
    else
    {
        status_reg = DMA->HISR.Reg;
        mask_index = config->Stream - 4;
    }
    
    /*------------------------------------------------------------------------
     * STEP 4: CHECK FLAGS AND DETERMINE STATUS
     * 
     * Priority order:
     * 1. Error (TEIF) - highest priority, indicates transfer failure
     * 2. Complete (TCIF) - transfer finished successfully
     * 3. Running (EN=1) - transfer still in progress
     * 4. Idle - default state when nothing is happening
     *------------------------------------------------------------------------*/
    if (status_reg & TEIF_MASK[mask_index])
    {
        *status = DMA_ERROR;  // Transfer error occurred
    }
    else if (status_reg & TCIF_MASK[mask_index])
    {
        *status = DMA_COMPLETE;  // Transfer finished successfully
    }
    else if (DMA->STREAM[config->Stream].CR.BIT.EN == 1)
    {
        *status = DMA_RUNNING;  // Transfer in progress
    }
    else
    {
        *status = DMA_IDLE;  // Stream is idle
    }
    
    /*------------------------------------------------------------------------
     * STEP 5: RETURN SUCCESS
     *------------------------------------------------------------------------*/
    return DMA_OK;
}


/**
 * @brief High-level memory copy using DMA
 * 
 * Provides a simple memcpy-like interface using DMA for the actual transfer.
 * Uses DMA2 Stream 0 for the transfer (only DMA2 supports M2M).
 * 
 * @param dest Destination memory address
 * @param src Source memory address
 * @param size Number of bytes to copy
 * @param cbf Callback function invoked when copy completes
 * @return DMA_Error_t DMA_OK on success, error code otherwise
 * 
 * Advantages over CPU memcpy:
 * - Frees CPU for other tasks during transfer
 * - Can achieve higher throughput for large transfers
 * - No CPU cycles consumed during the actual copy
 * 
 * Disadvantages:
 * - Overhead for small transfers
 * - Asynchronous (need callback for completion)
 * - Uses DMA2 Stream 0 exclusively (may conflict with other uses)
 * 
 * @note This is a non-blocking function - returns immediately
 * @note For blocking copy, implement wait loop using DMA_GetTransferStatus()
 * 
 * @warning This function uses DMA2 Stream 0 - ensure it's not in use elsewhere
 */
DMA_Error_t DMA_MemCpy(void *dest, const void *src, uint16_t size, DMA_Cbf_t cbf)
{
    /*------------------------------------------------------------------------
     * STEP 1: PARAMETER VALIDATION
     *------------------------------------------------------------------------*/
    if (dest == NULL || src == NULL || cbf == NULL)
    {
        return DMA_NULLPTR;
    }
    
    // Zero-size copy is a no-op, return success
    if (size == 0)
    {
        return DMA_OK;
    }
    
    /*------------------------------------------------------------------------
     * STEP 2: CREATE CONFIGURATION FOR M2M TRANSFER
     * 
     * Fixed configuration choices:
     * - DMA2: Only DMA2 supports Memory-to-Memory
     * - Stream 0: Arbitrary choice, could be parameterized
     * - Channel 0: Doesn't matter for M2M (no peripheral request)
     * - Byte size: Safe default, works for any alignment
     * - Both addresses increment: Standard for memory copy
     * - Normal mode: Single transfer (no circular)
     * - High priority: Faster completion
     * 
     * For M2M transfers:
     * - PAR (Peripheral Address) acts as SOURCE
     * - M0AR (Memory Address) acts as DESTINATION
     *------------------------------------------------------------------------*/
    DMA_Config_t config = {
        .DMA = DMA_2,                          // Must use DMA2 for M2M
        .Stream = DMA_STREAM0,                 // Using Stream 0
        .Channel = DMA_CHANNEL0,               // Channel irrelevant for M2M
        .Dir = DMA_DIR_M2M,                    // Memory-to-Memory mode
        .Peripheral_Address = (uint32_t)src,   // Source address (PAR)
        .Memory_Address = (uint32_t)dest,      // Destination address (M0AR)
        .Transfer_Count = size,                // Number of bytes
        .Periphera_DataSize = DMA_SIZE_BYTE,   // Transfer byte-by-byte
        .Memory_DataSize = DMA_SIZE_BYTE,      // Match sizes
        .Peripheral_INC = DMA_INC_YES,         // Increment source pointer
        .Memory_INC = DMA_INC_YES,             // Increment dest pointer
        .Mode = DMA_MODE_NORMAL,               // Single transfer
        .Priority = DMA_PRIORITY_HIGH          // Fast completion
    };
    
    /*------------------------------------------------------------------------
     * STEP 3: INITIALIZE AND START THE TRANSFER
     *------------------------------------------------------------------------*/
    DMA_Error_t err;
    
    // Configure the DMA stream
    err = DMA_Init(&config);
    if (err != DMA_OK)
    {
        return err;  // Return init error
    }
    
    // Start the transfer with callback
    err = DMA_Start(&config, cbf);
    if (err != DMA_OK)
    {
        return err;  // Return start error
    }
    
    /*------------------------------------------------------------------------
     * STEP 4: RETURN SUCCESS
     * Transfer is now in progress asynchronously
     *------------------------------------------------------------------------*/
    return DMA_OK;
}