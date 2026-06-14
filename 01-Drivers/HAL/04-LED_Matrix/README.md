
# LED Matrix Driver (STM32)

8x8 LED matrix driver with row multiplexing.

## Quick Start

```c
#include "LEDMatrix.h"

const uint8_t Pattern[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};

int main(void)
{
    Sched_Init(1);
    LMAX_Init(LMAX_1);
    LMAX_SetBuffer(LMAX_1, Pattern);
    LMAX_Enable(LMAX_1);
    
    static Runnable_t Task = {LMAX_RefreshTask, 1, 0, NULL, 0};
    Sched_Register_Runnable(&Task);
    Sched_Start();
}
```

## Pin Configuration (`LEDMatrix_Cfg.c`)

```c
const LMAX_Cfg_t LMAX_Cfg[LED_MATRIX_MAX] = {
    [LMAX_1] = {
        .type = LMAX_COMMON_CATHODE,
        .Row_ports = {GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA},
        .Row_pins = {0,1,2,3,4,5,6,7},
        .Column_ports = {GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB},
        .Column_pins = {0,1,2,3,4,5,6,7}
    }
};
```

## API

| Function | Description |
|----------|-------------|
| `LMAX_Init(name)` | Initialize |
| `LMAX_Enable(name)` | Enable display |
| `LMAX_Disable(name)` | Disable display |
| `LMAX_SetBuffer(name, buf)` | Set 8-byte buffer |
| `LMAX_Clear(name)` | Clear all |
| `LMAX_SetPixel(name, row, col, state)` | Set pixel |
| `LMAX_RefreshTask(arg)` | Scheduler callback |

