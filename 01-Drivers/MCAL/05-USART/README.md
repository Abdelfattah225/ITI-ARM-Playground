# STM32F4xx UART Driver (Interrupt-Driven, Ring-Buffer)

This repository provides a **small, dependency-free** UART/USART driver for the STM32F4 family (tested on STM32F401).  
It is designed for **normal “application level” users**: all low-level register handling, GPIO setup and interrupt servicing is hidden behind a clean, easy-to-use API.

---

## 1. Key Features

* Works with **USART1, USART2 and USART6**  
  (Feel free to add more – code is fully generic.)
* **Interrupt driven** – the CPU is only interrupted when data is ready.
* Built-in **ring buffers** for both TX and RX (sizes configurable in `uart_cfg.h`).
* Fully **non-blocking**: `UART_SendBuffer()` returns immediately, `UART_ReceiveBuffer()` only copies what is already available.
* Only the **CMSIS device header** is required (no STM HAL / LL / Cube dependencies).
* Clean, well-documented code – every register access is explained.
* Compile-time configuration only – no runtime heap allocation.

---

## 2. Folder / File Layout

```
| -- uart.c                (driver implementation)
| -- uart_Interface.h  (public API)
| -- uart_cfg.h        (buffer sizes)
| -- uart_private.h    (register bit-fields – optional read only)
| -- README.md                 (this file)
```

Board- or project-specific helpers (GPIO, RCC, NVIC) are expected to live in  
`include/interface/MCAL/`, but you can adapt paths in your build system.

---

## 3. Hardware & Toolchain Requirements

* STM32F4xx device with at least one of USART1 / 2 / 6.
* Any bare-metal or RTOS environment that
  * provides the CMSIS device header (e.g. `stm32f401xe.h`)
  * executes the IRQ handlers defined in `uart.c`.
* Standard C99 compiler (tested with `arm-none-eabi-gcc 12.x`).

---

## 4. Quick Start (5 Steps)

1. **Copy the driver** (`src/uart.c` + files in `include/`) into your project.  
2. **Check Pin Mapping**

   | UART | TX pin (AF) | RX pin (AF) |
   |------|-------------|-------------|
   | 1    | PA9  (AF7)  | PA10 (AF7)  |
   | 2    | PA2  (AF7)  | PA3  (AF7)  |
   | 6    | PA11 (AF8)  | PA12 (AF8)  |

   Need other pins? Adjust the GPIO section inside `UART_Init()`.

3. **Increase/Decrease Buffer Sizes**  
   Edit `include/configuration/MCAL/uart_cfg.h`:

   ```c
   #define UART_TX_BUFFER_SIZE 128   // bytes queued for transmit
   #define UART_RX_BUFFER_SIZE 256   // bytes buffered when receiving
   ```

4. **Add Driver to Build**  
   ```Makefile
   SRC += src/uart.c
   INC += -Iinclude
   ```

5. **Write Your Application**

   ```c
   #include "uart_Interface.h"

   static UART_Handle_t uart1;           // must be global / static

   void app_init(void)
   {
       UART_Config_t cfg = {
           .uart_instance = UART_INSTANCE_1,
           .Buad_rate     = 115200,
           .word_length   = UART_WORD_8BIT,
           .Parity        = UART_PARITY_NONE,
           .Stop_bit      = UART_STOP_1
       };

       UART_Init(&cfg, &uart1);
   }

   void app_task(void)
   {
       /* ------------- transmit ------------- */
       char *msg = "Hello World\r\n";
       UART_SendBuffer(&uart1, (uint8_t*)msg, strlen(msg));

       /* ------------- receive -------------- */
       uint8_t rx[32];
       uint16_t got;
       if (UART_ReceiveBuffer(&uart1, rx, sizeof(rx), &got) == UART_OK)
       {
           // Echo back
           UART_SendBuffer(&uart1, rx, got);
       }
   }
   ```

---

## 5. API Cheat-Sheet

Function | Description
---------|------------
`UART_Init(cfg, handle)` | One-time peripheral + GPIO + IRQ setup
`UART_SendBuffer(handle, data, len)` | Queue data for TX (non-blocking)
`UART_ReceiveBuffer(handle, data, maxLen, *readLen)` | Fetch already-received bytes
`UART_GetTxFreeSpace(handle)` | Remaining TX buffer space
`UART_GetRxCount(handle)` | Bytes waiting in RX buffer
`UART_IsTxBusy(handle)` | 0 when everything is sent

Error codes returned (`UART_OK`, `UART_BUFFER_FULL`, …) are documented in `uart_Interface.h`.

---

## 6. Integrating with Startup / NVIC

The driver already defines the ISR names used in the CMSIS vector table:

```
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
```

If you use the default names (CubeMX, CMSIS), **nothing to do** – the linker will pick up these weak symbols.  
If you renamed the handlers in your startup file, just call `UART_IRQHandler_Common()` from your own ISR or copy its body.

---

## 7. Configuration Options (`uart_cfg.h`)

Macro | Purpose | Default
------|---------|---------
`UART_TX_BUFFER_SIZE` | TX ring buffer size (bytes) | 64
`UART_RX_BUFFER_SIZE` | RX ring buffer size (bytes) | 64
`BUFFER_MAX` | Legacy – can be ignored | 64

Add further compile-time flags (e.g. enable/disable DMA, choose default baud) as you wish.

---

## 8. Typical Use Patterns

1. **Stream of data**  
   Send chunks as soon as they are produced; driver takes care of pacing:
   ```c
   if (UART_GetTxFreeSpace(&uart1) >= chunk_len)
       UART_SendBuffer(&uart1, chunk, chunk_len);
   ```

2. **Wait for all bytes sent**  
   ```c
   while (UART_IsTxBusy(&uart1)) { /* idle, sleep, or feed WDT */ }
   ```

3. **Read only when enough bytes are available**  
   ```c
   if (UART_GetRxCount(&uart1) >= EXPECTED_FRAME)
       UART_ReceiveBuffer(&uart1, frame, EXPECTED_FRAME, &rcvd);
   ```

---

## 9. Extending / Porting

* **Different pins** – change GPIO config in `UART_Init()` switch-case.
* **More USART instances** – duplicate the case in `UART_Init()`, add a static handle pointer + ISR wrapper (copy-paste pattern).
* **DMA support** – buffer logic stays, only TX/RX ISR sections must be replaced with DMA callbacks.

---

## 10. Troubleshooting

Symptom | Possible Cause | Fix
--------|----------------|----
Nothing is transmitted | • GPIO pins not in AF mode<br>• Wrong baud | Check `UART_Init()` log, confirm AF mapping, clock.  
Only first byte TX, then stall | Forgot to keep `UART_Handle_t` in scope -> stack variable destroyed | Declare handle `static` or global.  
RX stops after overflow | RX buffer too small, ORE not cleared (should not happen) | Increase `UART_RX_BUFFER_SIZE`; ensure ISR not blocked for too long.  
Linker error “multiple definition of USARTx_IRQHandler” | Another library already implements same ISR | Remove duplicate or call common handler inside your own.

Enable semihosted debugging or toggle a GPIO inside `UART_IRQHandler_Common()` to confirm interrupt execution.
