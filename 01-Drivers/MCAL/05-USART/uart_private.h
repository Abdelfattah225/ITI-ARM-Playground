#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

#include <stdint.h>

// ============================================================================
// Base Addresses (STM32F4xx)
// ============================================================================
#define USART1_BASE     0x40011000U
#define USART2_BASE     0x40004400U
#define USART6_BASE     0x40011400U


// ============================================================================
// UART Status Register (SR) - Offset: 0x00
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t PE    : 1;  // Bit 0:  Parity Error
        uint32_t FE    : 1;  // Bit 1:  Framing Error
        uint32_t NF    : 1;  // Bit 2:  Noise detected Flag
        uint32_t ORE   : 1;  // Bit 3:  Overrun Error
        uint32_t IDLE  : 1;  // Bit 4:  IDLE line detected
        uint32_t RXNE  : 1;  // Bit 5:  Read data register Not Empty
        uint32_t TC    : 1;  // Bit 6:  Transmission Complete
        uint32_t TXE   : 1;  // Bit 7:  Transmit data register Empty
        uint32_t LBD   : 1;  // Bit 8:  LIN Break Detection flag
        uint32_t CTS   : 1;  // Bit 9:  CTS flag (hardware flow control)
        uint32_t RES   : 22; // Bits 10-31: Reserved
    } BIT;
} USART_SR;

// ============================================================================
// UART Data Register (DR) - Offset: 0x04
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t DR    : 9;  // Bits 0-8:  Data value (8 or 9 bits)
        uint32_t RES   : 23; // Bits 9-31: Reserved
    } BIT;
} USART_DR;

// ============================================================================
// UART Baud Rate Register (BRR) - Offset: 0x08
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t DIV_Fraction : 4;  // Bits 0-3:   Fraction of USARTDIV
        uint32_t DIV_Mantissa : 12; // Bits 4-15:  Mantissa of USARTDIV
        uint32_t RES          : 16; // Bits 16-31: Reserved
    } BIT;
} USART_BRR;

// ============================================================================
// UART Control Register 1 (CR1) - Offset: 0x0C
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t SBK     : 1;  // Bit 0:  Send Break
        uint32_t RWU     : 1;  // Bit 1:  Receiver Wakeup
        uint32_t RE      : 1;  // Bit 2:  Receiver Enable
        uint32_t TE      : 1;  // Bit 3:  Transmitter Enable
        uint32_t IDLEIE  : 1;  // Bit 4:  IDLE Interrupt Enable
        uint32_t RXNEIE  : 1;  // Bit 5:  RXNE Interrupt Enable
        uint32_t TCIE    : 1;  // Bit 6:  Transmission Complete Interrupt Enable
        uint32_t TXEIE   : 1;  // Bit 7:  TXE Interrupt Enable
        uint32_t PEIE    : 1;  // Bit 8:  PE Interrupt Enable
        uint32_t PS      : 1;  // Bit 9:  Parity Selection (0=Even, 1=Odd)
        uint32_t PCE     : 1;  // Bit 10: Parity Control Enable
        uint32_t WAKE    : 1;  // Bit 11: Wakeup Method (0=Idle, 1=Address Mark)
        uint32_t M       : 1;  // Bit 12: Word Length (0=8 bits, 1=9 bits)
        uint32_t UE      : 1;  // Bit 13: USART Enable
        uint32_t RES     : 1;  // Bit 14: Reserved
        uint32_t OVER8   : 1;  // Bit 15: Oversampling Mode (0=16, 1=8)
        uint32_t RES2    : 16; // Bits 16-31: Reserved
    } BIT;
} USART_CR1;

// ============================================================================
// UART Control Register 2 (CR2) - Offset: 0x10
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t ADD     : 4;  // Bits 0-3:   Address of USART node
        uint32_t RES     : 1;  // Bit 4:     Reserved
        uint32_t LBDL    : 1;  // Bit 5:     LIN Break Detection Length
        uint32_t LBDIE   : 1;  // Bit 6:     LIN Break Detection Interrupt Enable
        uint32_t RES2    : 1;  // Bit 7:     Reserved
        uint32_t LBCL    : 1;  // Bit 8:     Last Bit Clock Pulse
        uint32_t CPHA    : 1;  // Bit 9:     Clock Phase
        uint32_t CPOL    : 1;  // Bit 10:    Clock Polarity
        uint32_t CLKEN   : 1;  // Bit 11:    Clock Enable
        uint32_t STOP    : 2;  // Bits 12-13: STOP bits (00=1, 01=0.5, 10=2, 11=1.5)
        uint32_t LINEN   : 1;  // Bit 14:    LIN mode Enable
        uint32_t RES3    : 17; // Bits 15-31: Reserved
    } BIT;
} USART_CR2;

// ============================================================================
// UART Control Register 3 (CR3) - Offset: 0x14
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t EIE     : 1;  // Bit 0:  Error Interrupt Enable
        uint32_t IREN    : 1;  // Bit 1:  IrDA mode Enable
        uint32_t IRLP    : 1;  // Bit 2:  IrDA Low-Power
        uint32_t HDSEL   : 1;  // Bit 3:  Half-Duplex Selection
        uint32_t NACK    : 1;  // Bit 4:  Smartcard NACK Enable
        uint32_t SCEN    : 1;  // Bit 5:  Smartcard mode Enable
        uint32_t DMAR    : 1;  // Bit 6:  DMA Enable Receiver
        uint32_t DMAT    : 1;  // Bit 7:  DMA Enable Transmitter
        uint32_t RTSE    : 1;  // Bit 8:  RTS Enable
        uint32_t CTSE    : 1;  // Bit 9:  CTS Enable
        uint32_t CTSIE   : 1;  // Bit 10: CTS Interrupt Enable
        uint32_t ONEBIT  : 1;  // Bit 11: One sample bit method Enable
        uint32_t RES     : 20; // Bits 12-31: Reserved
    } BIT;
} USART_CR3;

// ============================================================================
// UART Guard Time and Prescaler Register (GTPR) - Offset: 0x18
// ============================================================================
typedef union {
    uint32_t REG;
    struct {
        uint32_t PSC     : 8;  // Bits 0-7:   Prescaler value
        uint32_t GT      : 8;  // Bits 8-15:  Guard time value
        uint32_t RES     : 16; // Bits 16-31: Reserved
    } BIT;
} USART_GTPR;


typedef struct {
    USART_SR   SR;    // Offset: 0x00 - Status Register
    USART_DR   DR;    // Offset: 0x04 - Data Register
    USART_BRR  BRR;   // Offset: 0x08 - Baud Rate Register
    USART_CR1  CR1;   // Offset: 0x0C - Control Register 1
    USART_CR2  CR2;   // Offset: 0x10 - Control Register 2
    USART_CR3  CR3;   // Offset: 0x14 - Control Register 3
    USART_GTPR GTPR;  // Offset: 0x18 - Guard Time and Prescaler Register
} USART_TypeDef;


// ============================================================================
// Peripheral Pointers
// ============================================================================
#define USART1          ((volatile USART_TypeDef*)USART1_BASE)
#define USART2          ((volatile USART_TypeDef*)USART2_BASE)
#define USART6          ((volatile USART_TypeDef*)USART6_BASE)



#endif