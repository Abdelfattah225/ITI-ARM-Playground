# DMA Driver - User Guide

## Quick Start

### 1. Include the Header

```c
#include "dma_interface.h"
```

### 2. Create Configuration

```c
DMA_Config_t dma_config = {
    .DMA                = DMA_2,
    .Stream             = DMA_STREAM0,
    .Channel            = DMA_CHANNEL0,
    .Dir                = DMA_DIR_M2P,          // Memory to Peripheral
    .Peripheral_Address = (uint32_t)&USART1->DR,
    .Memory_Address     = (uint32_t)tx_buffer,
    .Transfer_Count     = 100,
    .Periphera_DataSize = DMA_SIZE_BYTE,
    .Memory_DataSize    = DMA_SIZE_BYTE,
    .Peripheral_INC     = DMA_INC_NO,           // Peripheral address fixed
    .Memory_INC         = DMA_INC_YES,          // Memory address increments
    .Mode               = DMA_MODE_NORMAL,
    .Priority           = DMA_PRIORITY_HIGH
};
```

### 3. Create Callback Function

```c
void DMA_TransferComplete(DMA_Status_t status) {
    if (status == DMA_COMPLETE) {
        // Transfer finished successfully
    } else if (status == DMA_ERROR) {
        // Handle error
    }
}
```

### 4. Initialize and Start

```c
DMA_Init(&dma_config);
DMA_Start(&dma_config, DMA_TransferComplete);
```

---

## Available Options

### DMA Controllers

| Controller | M2M Support |
|------------|-------------|
| `DMA_1` | ❌ No |
| `DMA_2` | ✅ Yes |

### Streams & Channels

- **Streams:** `DMA_STREAM0` to `DMA_STREAM7`
- **Channels:** `DMA_CHANNEL0` to `DMA_CHANNEL7`

> Check STM32 Reference Manual for peripheral-to-stream/channel mapping.

### Transfer Direction

| Direction | Description |
|-----------|-------------|
| `DMA_DIR_P2M` | Peripheral → Memory |
| `DMA_DIR_M2P` | Memory → Peripheral |
| `DMA_DIR_M2M` | Memory → Memory (DMA2 only) |

### Data Size

| Size | Description |
|------|-------------|
| `DMA_SIZE_BYTE` | 8-bit |
| `DMA_SIZE_HALF_WORD` | 16-bit |
| `DMA_SIZE_WORD` | 32-bit |

### Mode

| Mode | Description |
|------|-------------|
| `DMA_MODE_NORMAL` | Single transfer, stops when done |
| `DMA_MODE_CIRCULAR` | Auto-restarts (not for M2M) |

### Priority

`DMA_PRIORITY_LOW` → `DMA_PRIORITY_MEDIUM` → `DMA_PRIORITY_HIGH` → `DMA_PRIORITY_VERY_HIGH`

---

## Common Use Cases

### UART TX (Memory → Peripheral)

```c
DMA_Config_t uart_tx = {
    .DMA                = DMA_2,
    .Stream             = DMA_STREAM7,
    .Channel            = DMA_CHANNEL4,
    .Dir                = DMA_DIR_M2P,
    .Peripheral_Address = (uint32_t)&USART1->DR,
    .Memory_Address     = (uint32_t)tx_data,
    .Transfer_Count     = tx_length,
    .Periphera_DataSize = DMA_SIZE_BYTE,
    .Memory_DataSize    = DMA_SIZE_BYTE,
    .Peripheral_INC     = DMA_INC_NO,
    .Memory_INC         = DMA_INC_YES,
    .Mode               = DMA_MODE_NORMAL,
    .Priority           = DMA_PRIORITY_MEDIUM
};
```

### ADC RX (Peripheral → Memory)

```c
DMA_Config_t adc_rx = {
    .DMA                = DMA_2,
    .Stream             = DMA_STREAM0,
    .Channel            = DMA_CHANNEL0,
    .Dir                = DMA_DIR_P2M,
    .Peripheral_Address = (uint32_t)&ADC1->DR,
    .Memory_Address     = (uint32_t)adc_buffer,
    .Transfer_Count     = 256,
    .Periphera_DataSize = DMA_SIZE_HALF_WORD,
    .Memory_DataSize    = DMA_SIZE_HALF_WORD,
    .Peripheral_INC     = DMA_INC_NO,
    .Memory_INC         = DMA_INC_YES,
    .Mode               = DMA_MODE_CIRCULAR,
    .Priority           = DMA_PRIORITY_HIGH
};
```

### Memory Copy (Easy Way)

```c
uint8_t src[100] = {/* data */};
uint8_t dest[100];

void CopyDone(DMA_Status_t status) {
    // Copy finished
}

DMA_MemCpy(dest, src, 100, CopyDone);
```

---

## Helper Functions

| Function | Purpose |
|----------|---------|
| `DMA_GetTransferDataNum(&config, &count)` | Get bytes transferred so far |
| `DMA_GetTransferStatus(&config, &status)` | Get current status |
| `DMA_MemCpy(dest, src, size, callback)` | Simple memory copy |

### Check Transfer Progress

```c
uint16_t transferred;
DMA_GetTransferDataNum(&dma_config, &transferred);
```

### Check Status

```c
DMA_Status_t status;
DMA_GetTransferStatus(&dma_config, &status);

switch (status) {
    case DMA_IDLE:     /* Not started */     break;
    case DMA_RUNNING:  /* In progress */     break;
    case DMA_COMPLETE: /* Finished */        break;
    case DMA_ERROR:    /* Error occurred */  break;
}
```

---

## Complete Example

```c
#include "dma_interface.h"

uint8_t src_buffer[64] = "Hello DMA!";
uint8_t dst_buffer[64];
volatile uint8_t transfer_done = 0;

void DMA_Callback(DMA_Status_t status) {
    if (status == DMA_COMPLETE) {
        transfer_done = 1;
    }
}

int main(void) {
    // Memory-to-Memory copy using DMA2
    DMA_MemCpy(dst_buffer, src_buffer, 64, DMA_Callback);
    
    // Wait for completion
    while (!transfer_done);
    
    // dst_buffer now contains "Hello DMA!"
    
    while (1);
}
```

---

## Error Codes

| Code | Meaning |
|------|---------|
| `DMA_OK` | Success |
| `DMA_NULLPTR` | NULL pointer passed |
| `DMA_NOT_SUPPORTED` | Invalid config (M2M on DMA1, Circular + M2M) |
| `DMA_STREAM_BUSY` | Stream already in use |

---

## Important Notes

⚠️ **DMA1 cannot do Memory-to-Memory** - Use DMA2 for `DMA_DIR_M2M`.

⚠️ **Circular mode not allowed with M2M** - Use `DMA_MODE_NORMAL` only.

⚠️ **Callback runs in interrupt context** - Keep it short!

⚠️ **Buffers must stay valid** - Don't use local variables that go out of scope.
