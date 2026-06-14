#include "./os/sched.h"
#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/HAL/LEDMatrix.h"
#include "../lib/lib_utii.h"
#include "sched.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"
#include "../include/interface/MCAL/spi_interface.h"



// Define the variable HERE (only once!)
SPI_Config_t spi_config = {
    .BAUD_RATE = SPI_BR_DIV16,
    .BIDI_MODE = SPI_BIDI_2LINE,
    .BIDI_OUTPUT = SPI_BIDI_OUTPUT_ENABLED,
    .BIT_ORDER = SPI_LSB_FIRST,
    .CPHA = SPI_CPHA_1ST_EDGE,
    .CPOL = SPI_CPOL_LOW,
    .CRC = SPI_CRC_DISABLED,
    .DATA_SIZE = SPI_DATA_SIZE_8BIT,
    .DMA = SPI_DMA_DISABLED,
    .DUPLEX = SPI_DUPLEX_FULL,
    .INTERRUPT = SPI_IRQ_ENABLED,
    .MODE = SPI_MASTER_MODE,
    .PORT = SPI_PORT_1,
    .PROTOCOL = SPI_FRF_MOTOROLA,
    .SSM = SPI_SSM_SOFTWARE,
    .SSOE = SPI_SSOE_ENABLED
};


// Test buffers
uint8_t txBuffer[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44};
uint8_t rxBuffer[8] = {0};

// Test status
volatile uint8_t transferComplete = 0;

// ========== ADD THIS LINE ==========
volatile uint32_t isrCallCount = 0;    // <-- A DD THIS!
// ===================================

void SPI_TransferCallback(void)
{
    transferComplete = 1;
}

int main()
{
    RCC_Init();
    NVIC_Init();
    SYSTICK_Init();
    SPI_Init(&spi_config);
    SPI_TransmitReceive_IT(SPI_PORT_1, rxBuffer, txBuffer, 4, SPI_TransferCallback);
    
    while (!transferComplete);
    
    while (1)
    {
        // Breakpoint here after transfer completes
    }
}
