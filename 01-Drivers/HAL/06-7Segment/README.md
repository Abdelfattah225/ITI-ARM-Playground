
# Seven Segment Display Driver

## Overview

A simple driver for controlling single-digit seven segment displays on STM32 microcontrollers. Supports both **Common Anode** and **Common Cathode** displays.

## Segment Layout

```
    AAA
   F   B
    GGG
   E   C
    DDD  .DP
```

| Index | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7  |
|-------|---|---|---|---|---|---|---|----|
| Seg   | A | B | C | D | E | F | G | DP |

## Configuration

### 1. Select Display Type (`7seg_cfg_t.h`)

```c
#define SEVEN_SEG_TYPE    SEVEN_SEG_COMMON_ANODE    /* or SEVEN_SEG_COMMON_CATHODE */
```

### 2. Configure Pins (`7seg_cfg.c`)

```c
Seven_SEG_cfg_t my7Seg_cfg = {
    .segmentPort = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA},
    .segmentPins = {GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7}
};
```

## API

| Function | Description |
|----------|-------------|
| `Seven_SEG_Init(cfg)` | Initialize display pins |
| `Seven_SEG_DisplayDigit(cfg, digit)` | Display digit (0-9) |
| `Seven_SEG_Clear(cfg)` | Turn off all segments |
| `Seven_SEG_DisplayDot(cfg, state)` | Control decimal point (SEVEN_SEG_DP_ON / SEVEN_SEG_DP_OFF) |

**Returns:** `SEVEN_SEGMENT_OK` or `SEVEN_SEGMENT_ERROR`

## Quick Example

```c
#include "7seg.h"

extern Seven_SEG_cfg_t my7Seg_cfg;

int main(void)
{
    Seven_SEG_Init(&my7Seg_cfg);
    Seven_SEG_DisplayDigit(&my7Seg_cfg, 5);
    Seven_SEG_DisplayDot(&my7Seg_cfg, SEVEN_SEG_DP_ON);
    
    while(1);
}
```
