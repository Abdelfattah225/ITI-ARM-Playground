#ifndef DMA_INTERFACE_H
#define DMA_INTERFACE_H

/*============================================================================
 *                              1. INCLUDES
 *===========================================================================*/
#include "../../../lib/lib_utii.h"

/*============================================================================
 *                           2. SIMPLE ENUMS
 *===========================================================================*/
typedef enum {
    DMA_1,
    DMA_2
} DMA_Types_t;

typedef enum {
    DMA_STREAM0,
    DMA_STREAM1,
    DMA_STREAM2,
    DMA_STREAM3,
    DMA_STREAM4,
    DMA_STREAM5,
    DMA_STREAM6,
    DMA_STREAM7
} DMA_Streams_t;

typedef enum {
    DMA_CHANNEL0,
    DMA_CHANNEL1,
    DMA_CHANNEL2,
    DMA_CHANNEL3,
    DMA_CHANNEL4,
    DMA_CHANNEL5,
    DMA_CHANNEL6,
    DMA_CHANNEL7
} DMA_Channels_t;

typedef enum {
    DMA_DIR_P2M,
    DMA_DIR_M2P,
    DMA_DIR_M2M
} DMA_Direction_t;

typedef enum {
    DMA_SIZE_BYTE,
    DMA_SIZE_HALF_WORD,
    DMA_SIZE_WORD
} DMA_DataSize_t;

typedef enum {
    DMA_INC_NO,
    DMA_INC_YES
} DMA_INC_t;

typedef enum {
    DMA_MODE_NORMAL,
    DMA_MODE_CIRCULAR
} DMA_Mode_t;

typedef enum {
    DMA_PRIORITY_LOW,
    DMA_PRIORITY_MEDIUM,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH
} DMA_Priority_t;

/*============================================================================
 *                          3. DMA_Error_t ENUM
 *===========================================================================*/
typedef enum {
    DMA_NULLPTR,
    DMA_NOT_SUPPORTED,
    DMA_STREAM_BUSY,
    DMA_OK
} DMA_Error_t;

/*============================================================================
 *                         4. DMA_Status_t ENUM
 *===========================================================================*/
typedef enum {
    DMA_IDLE,
    DMA_RUNNING,
    DMA_COMPLETE,
    DMA_ERROR
} DMA_Status_t;

/*============================================================================
 *                      5. DMA_Cbf_t TYPEDEF (uses DMA_Status_t)
 *===========================================================================*/
typedef void (*DMA_Cbf_t)(DMA_Status_t status);

/*============================================================================
 *                         6. DMA_Config_t STRUCT
 *===========================================================================*/
typedef struct {
    DMA_Types_t      DMA;
    DMA_Streams_t    Stream;
    DMA_Channels_t   Channel;
    DMA_Direction_t  Dir;
    uint32_t         Peripheral_Address;
    uint32_t         Memory_Address;
    uint16_t         Transfer_Count;
    DMA_DataSize_t   Periphera_DataSize;
    DMA_DataSize_t   Memory_DataSize;
    DMA_INC_t        Peripheral_INC;
    DMA_INC_t        Memory_INC;
    DMA_Mode_t       Mode;
    DMA_Priority_t   Priority;
} DMA_Config_t;

/*============================================================================
 *                       7. FUNCTION PROTOTYPES
 *===========================================================================*/
DMA_Error_t DMA_Init(const DMA_Config_t *config);
DMA_Error_t DMA_Start(const DMA_Config_t *config, DMA_Cbf_t cbf);
DMA_Error_t DMA_GetTransferDataNum(const DMA_Config_t *config, uint16_t *NoOfDataTransferred);
DMA_Error_t DMA_GetTransferStatus(const DMA_Config_t *config, DMA_Status_t *status);
DMA_Error_t DMA_MemCpy(void *dest, const void *src, uint16_t size, DMA_Cbf_t cbf);

#endif /* DMA_INTERFACE_H */