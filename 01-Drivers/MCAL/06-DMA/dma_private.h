/******************************************************************************
 * @file    DMA_private.h
 * @author  
 * @brief   DMA Driver Private Header File for STM32F401CC
 * @version 1.0
 ******************************************************************************/

#ifndef DMA_PRIVATE_H
#define DMA_PRIVATE_H

#include <stdint.h>

/*============================================================================*/
/*                         BASE ADDRESSES                                      */
/*============================================================================*/

#define DMA1_BASE_ADDRESS    0x40026000UL
#define DMA2_BASE_ADDRESS    0x40026400UL

/*============================================================================*/
/*                    DMA INTERRUPT STATUS REGISTERS                           */
/*============================================================================*/

/* DMA Low Interrupt Status Register (LISR) - Streams 0-3 */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t FEIF0   : 1;   /* Bit 0:  Stream 0 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 1:  Reserved */
        uint32_t DMEIF0  : 1;   /* Bit 2:  Stream 0 direct mode error interrupt flag */
        uint32_t TEIF0   : 1;   /* Bit 3:  Stream 0 transfer error interrupt flag */
        uint32_t HTIF0   : 1;   /* Bit 4:  Stream 0 half transfer interrupt flag */
        uint32_t TCIF0   : 1;   /* Bit 5:  Stream 0 transfer complete interrupt flag */
        uint32_t FEIF1   : 1;   /* Bit 6:  Stream 1 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 7:  Reserved */
        uint32_t DMEIF1  : 1;   /* Bit 8:  Stream 1 direct mode error interrupt flag */
        uint32_t TEIF1   : 1;   /* Bit 9:  Stream 1 transfer error interrupt flag */
        uint32_t HTIF1   : 1;   /* Bit 10: Stream 1 half transfer interrupt flag */
        uint32_t TCIF1   : 1;   /* Bit 11: Stream 1 transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 12-15: Reserved */
        uint32_t FEIF2   : 1;   /* Bit 16: Stream 2 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 17: Reserved */
        uint32_t DMEIF2  : 1;   /* Bit 18: Stream 2 direct mode error interrupt flag */
        uint32_t TEIF2   : 1;   /* Bit 19: Stream 2 transfer error interrupt flag */
        uint32_t HTIF2   : 1;   /* Bit 20: Stream 2 half transfer interrupt flag */
        uint32_t TCIF2   : 1;   /* Bit 21: Stream 2 transfer complete interrupt flag */
        uint32_t FEIF3   : 1;   /* Bit 22: Stream 3 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 23: Reserved */
        uint32_t DMEIF3  : 1;   /* Bit 24: Stream 3 direct mode error interrupt flag */
        uint32_t TEIF3   : 1;   /* Bit 25: Stream 3 transfer error interrupt flag */
        uint32_t HTIF3   : 1;   /* Bit 26: Stream 3 half transfer interrupt flag */
        uint32_t TCIF3   : 1;   /* Bit 27: Stream 3 transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 28-31: Reserved */
    } BIT;
} DMA_LISR_t;

/* DMA High Interrupt Status Register (HISR) - Streams 4-7 */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t FEIF4   : 1;   /* Bit 0:  Stream 4 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 1:  Reserved */
        uint32_t DMEIF4  : 1;   /* Bit 2:  Stream 4 direct mode error interrupt flag */
        uint32_t TEIF4   : 1;   /* Bit 3:  Stream 4 transfer error interrupt flag */
        uint32_t HTIF4   : 1;   /* Bit 4:  Stream 4 half transfer interrupt flag */
        uint32_t TCIF4   : 1;   /* Bit 5:  Stream 4 transfer complete interrupt flag */
        uint32_t FEIF5   : 1;   /* Bit 6:  Stream 5 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 7:  Reserved */
        uint32_t DMEIF5  : 1;   /* Bit 8:  Stream 5 direct mode error interrupt flag */
        uint32_t TEIF5   : 1;   /* Bit 9:  Stream 5 transfer error interrupt flag */
        uint32_t HTIF5   : 1;   /* Bit 10: Stream 5 half transfer interrupt flag */
        uint32_t TCIF5   : 1;   /* Bit 11: Stream 5 transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 12-15: Reserved */
        uint32_t FEIF6   : 1;   /* Bit 16: Stream 6 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 17: Reserved */
        uint32_t DMEIF6  : 1;   /* Bit 18: Stream 6 direct mode error interrupt flag */
        uint32_t TEIF6   : 1;   /* Bit 19: Stream 6 transfer error interrupt flag */
        uint32_t HTIF6   : 1;   /* Bit 20: Stream 6 half transfer interrupt flag */
        uint32_t TCIF6   : 1;   /* Bit 21: Stream 6 transfer complete interrupt flag */
        uint32_t FEIF7   : 1;   /* Bit 22: Stream 7 FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 23: Reserved */
        uint32_t DMEIF7  : 1;   /* Bit 24: Stream 7 direct mode error interrupt flag */
        uint32_t TEIF7   : 1;   /* Bit 25: Stream 7 transfer error interrupt flag */
        uint32_t HTIF7   : 1;   /* Bit 26: Stream 7 half transfer interrupt flag */
        uint32_t TCIF7   : 1;   /* Bit 27: Stream 7 transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 28-31: Reserved */
    } BIT;
} DMA_HISR_t;

/*============================================================================*/
/*               DMA INTERRUPT FLAG CLEAR REGISTERS                            */
/*============================================================================*/

/* DMA Low Interrupt Flag Clear Register (LIFCR) - Streams 0-3 */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t CFEIF0  : 1;   /* Bit 0:  Stream 0 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 1:  Reserved */
        uint32_t CDMEIF0 : 1;   /* Bit 2:  Stream 0 clear direct mode error interrupt flag */
        uint32_t CTEIF0  : 1;   /* Bit 3:  Stream 0 clear transfer error interrupt flag */
        uint32_t CHTIF0  : 1;   /* Bit 4:  Stream 0 clear half transfer interrupt flag */
        uint32_t CTCIF0  : 1;   /* Bit 5:  Stream 0 clear transfer complete interrupt flag */
        uint32_t CFEIF1  : 1;   /* Bit 6:  Stream 1 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 7:  Reserved */
        uint32_t CDMEIF1 : 1;   /* Bit 8:  Stream 1 clear direct mode error interrupt flag */
        uint32_t CTEIF1  : 1;   /* Bit 9:  Stream 1 clear transfer error interrupt flag */
        uint32_t CHTIF1  : 1;   /* Bit 10: Stream 1 clear half transfer interrupt flag */
        uint32_t CTCIF1  : 1;   /* Bit 11: Stream 1 clear transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 12-15: Reserved */
        uint32_t CFEIF2  : 1;   /* Bit 16: Stream 2 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 17: Reserved */
        uint32_t CDMEIF2 : 1;   /* Bit 18: Stream 2 clear direct mode error interrupt flag */
        uint32_t CTEIF2  : 1;   /* Bit 19: Stream 2 clear transfer error interrupt flag */
        uint32_t CHTIF2  : 1;   /* Bit 20: Stream 2 clear half transfer interrupt flag */
        uint32_t CTCIF2  : 1;   /* Bit 21: Stream 2 clear transfer complete interrupt flag */
        uint32_t CFEIF3  : 1;   /* Bit 22: Stream 3 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 23: Reserved */
        uint32_t CDMEIF3 : 1;   /* Bit 24: Stream 3 clear direct mode error interrupt flag */
        uint32_t CTEIF3  : 1;   /* Bit 25: Stream 3 clear transfer error interrupt flag */
        uint32_t CHTIF3  : 1;   /* Bit 26: Stream 3 clear half transfer interrupt flag */
        uint32_t CTCIF3  : 1;   /* Bit 27: Stream 3 clear transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 28-31: Reserved */
    } BIT;
} DMA_LIFCR_t;

/* DMA High Interrupt Flag Clear Register (HIFCR) - Streams 4-7 */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t CFEIF4  : 1;   /* Bit 0:  Stream 4 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 1:  Reserved */
        uint32_t CDMEIF4 : 1;   /* Bit 2:  Stream 4 clear direct mode error interrupt flag */
        uint32_t CTEIF4  : 1;   /* Bit 3:  Stream 4 clear transfer error interrupt flag */
        uint32_t CHTIF4  : 1;   /* Bit 4:  Stream 4 clear half transfer interrupt flag */
        uint32_t CTCIF4  : 1;   /* Bit 5:  Stream 4 clear transfer complete interrupt flag */
        uint32_t CFEIF5  : 1;   /* Bit 6:  Stream 5 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 7:  Reserved */
        uint32_t CDMEIF5 : 1;   /* Bit 8:  Stream 5 clear direct mode error interrupt flag */
        uint32_t CTEIF5  : 1;   /* Bit 9:  Stream 5 clear transfer error interrupt flag */
        uint32_t CHTIF5  : 1;   /* Bit 10: Stream 5 clear half transfer interrupt flag */
        uint32_t CTCIF5  : 1;   /* Bit 11: Stream 5 clear transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 12-15: Reserved */
        uint32_t CFEIF6  : 1;   /* Bit 16: Stream 6 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 17: Reserved */
        uint32_t CDMEIF6 : 1;   /* Bit 18: Stream 6 clear direct mode error interrupt flag */
        uint32_t CTEIF6  : 1;   /* Bit 19: Stream 6 clear transfer error interrupt flag */
        uint32_t CHTIF6  : 1;   /* Bit 20: Stream 6 clear half transfer interrupt flag */
        uint32_t CTCIF6  : 1;   /* Bit 21: Stream 6 clear transfer complete interrupt flag */
        uint32_t CFEIF7  : 1;   /* Bit 22: Stream 7 clear FIFO error interrupt flag */
        uint32_t         : 1;   /* Bit 23: Reserved */
        uint32_t CDMEIF7 : 1;   /* Bit 24: Stream 7 clear direct mode error interrupt flag */
        uint32_t CTEIF7  : 1;   /* Bit 25: Stream 7 clear transfer error interrupt flag */
        uint32_t CHTIF7  : 1;   /* Bit 26: Stream 7 clear half transfer interrupt flag */
        uint32_t CTCIF7  : 1;   /* Bit 27: Stream 7 clear transfer complete interrupt flag */
        uint32_t         : 4;   /* Bits 28-31: Reserved */
    } BIT;
} DMA_HIFCR_t;

/*============================================================================*/
/*                    DMA STREAM CONFIGURATION REGISTER                        */
/*============================================================================*/

/* DMA Stream x Configuration Register (SxCR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t EN      : 1;   /* Bit 0:     Stream enable */
        uint32_t DMEIE   : 1;   /* Bit 1:     Direct mode error interrupt enable */
        uint32_t TEIE    : 1;   /* Bit 2:     Transfer error interrupt enable */
        uint32_t HTIE    : 1;   /* Bit 3:     Half transfer interrupt enable */
        uint32_t TCIE    : 1;   /* Bit 4:     Transfer complete interrupt enable */
        uint32_t PFCTRL  : 1;   /* Bit 5:     Peripheral flow controller */
        uint32_t DIR     : 2;   /* Bits 6-7:  Data transfer direction */
        uint32_t CIRC    : 1;   /* Bit 8:     Circular mode */
        uint32_t PINC    : 1;   /* Bit 9:     Peripheral increment mode */
        uint32_t MINC    : 1;   /* Bit 10:    Memory increment mode */
        uint32_t PSIZE   : 2;   /* Bits 11-12: Peripheral data size */
        uint32_t MSIZE   : 2;   /* Bits 13-14: Memory data size */
        uint32_t PINCOS  : 1;   /* Bit 15:    Peripheral increment offset size */
        uint32_t PL      : 2;   /* Bits 16-17: Priority level */
        uint32_t DBM     : 1;   /* Bit 18:    Double buffer mode */
        uint32_t CT      : 1;   /* Bit 19:    Current target */
        uint32_t         : 1;   /* Bit 20:    Reserved */
        uint32_t PBURST  : 2;   /* Bits 21-22: Peripheral burst transfer config */
        uint32_t MBURST  : 2;   /* Bits 23-24: Memory burst transfer config */
        uint32_t CHSEL   : 3;   /* Bits 25-27: Channel selection */
        uint32_t         : 4;   /* Bits 28-31: Reserved */
    } BIT;
} DMA_SxCR_t;

/*============================================================================*/
/*                DMA STREAM NUMBER OF DATA REGISTER                           */
/*============================================================================*/

/* DMA Stream x Number of Data Register (SxNDTR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t NDT     : 16;  /* Bits 0-15:  Number of data items to transfer */
        uint32_t         : 16;  /* Bits 16-31: Reserved */
    } BIT;
} DMA_SxNDTR_t;

/*============================================================================*/
/*                DMA STREAM PERIPHERAL ADDRESS REGISTER                       */
/*============================================================================*/

/* DMA Stream x Peripheral Address Register (SxPAR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t PAR     : 32;  /* Bits 0-31: Peripheral address */
    } BIT;
} DMA_SxPAR_t;

/*============================================================================*/
/*                DMA STREAM MEMORY ADDRESS REGISTERS                          */
/*============================================================================*/

/* DMA Stream x Memory 0 Address Register (SxM0AR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t M0A     : 32;  /* Bits 0-31: Memory 0 address */
    } BIT;
} DMA_SxM0AR_t;

/* DMA Stream x Memory 1 Address Register (SxM1AR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t M1A     : 32;  /* Bits 0-31: Memory 1 address */
    } BIT;
} DMA_SxM1AR_t;

/*============================================================================*/
/*                    DMA STREAM FIFO CONTROL REGISTER                         */
/*============================================================================*/

/* DMA Stream x FIFO Control Register (SxFCR) */
typedef union {
    uint32_t Reg;
    struct {
        uint32_t FTH     : 2;   /* Bits 0-1:  FIFO threshold selection */
        uint32_t DMDIS   : 1;   /* Bit 2:     Direct mode disable */
        uint32_t FS      : 3;   /* Bits 3-5:  FIFO status */
        uint32_t         : 1;   /* Bit 6:     Reserved */
        uint32_t FEIE    : 1;   /* Bit 7:     FIFO error interrupt enable */
        uint32_t         : 24;  /* Bits 8-31: Reserved */
    } BIT;
} DMA_SxFCR_t;

/*============================================================================*/
/*                         DMA STREAM STRUCTURE                                */
/*============================================================================*/

typedef struct {
    volatile DMA_SxCR_t    CR;      /* DMA stream x configuration register,   Offset: 0x00 */
    volatile DMA_SxNDTR_t  NDTR;    /* DMA stream x number of data register,  Offset: 0x04 */
    volatile DMA_SxPAR_t   PAR;     /* DMA stream x peripheral address reg,   Offset: 0x08 */
    volatile DMA_SxM0AR_t  M0AR;    /* DMA stream x memory 0 address reg,     Offset: 0x0C */
    volatile DMA_SxM1AR_t  M1AR;    /* DMA stream x memory 1 address reg,     Offset: 0x10 */
    volatile DMA_SxFCR_t   FCR;     /* DMA stream x FIFO control register,    Offset: 0x14 */
} DMA_Stream_t;

/*============================================================================*/
/*                         DMA CONTROLLER STRUCTURE                            */
/*============================================================================*/

typedef struct {
    volatile DMA_LISR_t   LISR;           /* DMA low interrupt status register,       Offset: 0x00 */
    volatile DMA_HISR_t   HISR;           /* DMA high interrupt status register,      Offset: 0x04 */
    volatile DMA_LIFCR_t  LIFCR;          /* DMA low interrupt flag clear register,   Offset: 0x08 */
    volatile DMA_HIFCR_t  HIFCR;          /* DMA high interrupt flag clear register,  Offset: 0x0C */
    volatile DMA_Stream_t STREAM[8];      /* DMA streams 0-7,                         Offset: 0x10 */
} DMA_TypeDef;

/*============================================================================*/
/*                         DMA PERIPHERAL INSTANCES                            */
/*============================================================================*/

#define DMA1    ((DMA_TypeDef *) DMA1_BASE_ADDRESS)
#define DMA2    ((DMA_TypeDef *) DMA2_BASE_ADDRESS)

/*============================================================================*/
/*                         CONFIGURATION VALUES                                */
/*============================================================================*/



/* Burst Transfer Configuration */
#define DMA_BURST_SINGLE            0x00U
#define DMA_BURST_INCR4             0x01U
#define DMA_BURST_INCR8             0x02U
#define DMA_BURST_INCR16            0x03U

/* FIFO Threshold Selection */
#define DMA_FIFO_THRESHOLD_1_4      0x00U
#define DMA_FIFO_THRESHOLD_1_2      0x01U
#define DMA_FIFO_THRESHOLD_3_4      0x02U
#define DMA_FIFO_THRESHOLD_FULL     0x03U

/* FIFO Status */
#define DMA_FIFO_STATUS_LESS_1_4    0x00U
#define DMA_FIFO_STATUS_LESS_1_2    0x01U
#define DMA_FIFO_STATUS_LESS_3_4    0x02U
#define DMA_FIFO_STATUS_LESS_FULL   0x03U
#define DMA_FIFO_STATUS_EMPTY       0x04U
#define DMA_FIFO_STATUS_FULL        0x05U

/*============================================================================*/
/*                         NUMBER OF STREAMS                                   */
/*============================================================================*/

#define DMA1_NUM_STREAMS            6U   /* DMA1 has streams 0-5 on STM32F401 */
#define DMA2_NUM_STREAMS            8U   /* DMA2 has streams 0-7 on STM32F401 */

#endif /* DMA_PRIVATE_H */