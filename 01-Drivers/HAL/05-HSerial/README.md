# HSerial Driver 📡

A simple, easy-to-use serial communication driver for STM32F4 microcontrollers.

---

## 📖 What is this?

HSerial lets you **send and receive data** through UART (serial port) on your STM32F4 board. It's perfect for:

- 💬 Communicating with a PC via USB-to-Serial adapter
- 🔌 Connecting to GPS, Bluetooth, WiFi modules
- 🤖 Talking to other microcontrollers
- 📟 Sending debug messages

**No need to understand DMA, interrupts, or registers** - just call simple functions!

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| ✅ Easy to use | Just 3 functions to learn |
| ✅ Non-blocking | Your code keeps running while data transfers |
| ✅ Callbacks | Get notified when transfer is done |
| ✅ Multiple UARTs | Use up to 3 serial ports simultaneously |
| ✅ Full-duplex | Send and receive at the same time |
| ✅ Error handling | Know when something goes wrong |

---

## 🎯 Supported Hardware

| Board | Supported |
|-------|-----------|
| STM32F401 (Nucleo, BlackPill) | ✅ Yes |
| STM32F411 (Nucleo, BlackPill) | ✅ Yes |
| STM32F407 (Discovery) | ✅ Yes |
| STM32F446 (Nucleo) | ✅ Yes |

---

## 📌 Pin Connections

Connect your serial device to these pins:

| UART | TX Pin | RX Pin | Common Use |
|------|--------|--------|------------|
| USART1 | PA9 | PA10 | General purpose |
| USART2 | PA2 | PA3 | ST-Link Virtual COM (Nucleo boards) |
| USART6 | PA11 | PA12 | General purpose |

### Wiring Diagram

```
Your STM32          External Device
┌─────────┐         ┌─────────┐
│      TX ├────────►│ RX      │
│         │         │         │
│      RX │◄────────┤ TX      │
│         │         │         │
│     GND ├─────────┤ GND     │
└─────────┘         └─────────┘

⚠️ Important: Cross TX→RX and RX→TX!
⚠️ Important: Always connect GND!
```

---

## 🚀 Quick Start

### Step 1: Include the header

```c
#include "hserial_interface.h"
```

### Step 2: Initialize the UART

```c
// Initialize USART2 at 115200 baud rate
HSerial_Init(HSERIAL_USART2, 115200);
```

### Step 3: Create a callback function

```c
// This function is called when transmission is done
void MyCallback(HSerial_Status_t status)
{
    if (status == HSERIAL_TX_COMPLETE)
    {
        // Success! Data was sent
    }
    else if (status == HSERIAL_TX_ERROR)
    {
        // Something went wrong
    }
}
```

### Step 4: Send data

```c
uint8_t message[] = "Hello World!\r\n";
HSerial_Send(HSERIAL_USART2, message, sizeof(message) - 1, MyCallback);
```

**That's it!** 🎉

---

## 📚 API Reference

### HSerial_Init()

**Purpose:** Set up a serial port before using it.

```c
HSerial_Error_t HSerial_Init(HSerial_Instance_t uart, uint32_t baudrate);
```

| Parameter | Description | Example Values |
|-----------|-------------|----------------|
| `uart` | Which serial port to use | `HSERIAL_USART1`, `HSERIAL_USART2`, `HSERIAL_USART6` |
| `baudrate` | Communication speed | `9600`, `115200`, `921600` |

**Returns:** `HSERIAL_OK` if successful

**Example:**
```c
// Setup USART1 at 9600 baud (slow, reliable)
HSerial_Init(HSERIAL_USART1, 9600);

// Setup USART2 at 115200 baud (common speed)
HSerial_Init(HSERIAL_USART2, 115200);

// Setup USART6 at 921600 baud (fast)
HSerial_Init(HSERIAL_USART6, 921600);
```

---

### HSerial_Send()

**Purpose:** Send data out through the serial port.

```c
HSerial_Error_t HSerial_Send(
    HSerial_Instance_t uart,      // Which port
    uint8_t *buffer,              // Data to send
    uint16_t size,                // How many bytes
    HSerial_Callback_t callback   // Function to call when done
);
```

| Parameter | Description |
|-----------|-------------|
| `uart` | Which serial port |
| `buffer` | Pointer to your data |
| `size` | Number of bytes to send |
| `callback` | Your function that gets called when done |

**Returns:**

| Value | Meaning |
|-------|---------|
| `HSERIAL_OK` | Transfer started successfully |
| `HSERIAL_BUSY` | Previous transfer still running, try again later |
| `HSERIAL_NULLPTR` | You passed NULL for buffer or callback |
| `HSERIAL_ERROR` | Something went wrong |

**Example:**
```c
uint8_t data[] = "Hello!";
HSerial_Send(HSERIAL_USART2, data, 6, MyCallback);
```

---

### HSerial_Receive()

**Purpose:** Receive data from the serial port.

```c
HSerial_Error_t HSerial_Receive(
    HSerial_Instance_t uart,      // Which port
    uint8_t *buffer,              // Where to store received data
    uint16_t size,                // How many bytes to wait for
    HSerial_Callback_t callback   // Function to call when done
);
```

| Parameter | Description |
|-----------|-------------|
| `uart` | Which serial port |
| `buffer` | Where to store incoming data |
| `size` | Number of bytes to receive |
| `callback` | Your function that gets called when all bytes received |

**Example:**
```c
uint8_t rx_buffer[100];
HSerial_Receive(HSERIAL_USART2, rx_buffer, 100, MyRxCallback);
```

---

## 💡 Complete Examples

### Example 1: Send a Message

```c
#include "hserial_interface.h"

// Flag to track if we can send
volatile uint8_t tx_done = 1;

// Called when transmission completes
void TxCallback(HSerial_Status_t status)
{
    if (status == HSERIAL_TX_COMPLETE)
    {
        tx_done = 1;  // Ready to send again
    }
}

int main(void)
{
    // Initialize system (your clock setup here)
    
    // Initialize UART
    HSerial_Init(HSERIAL_USART2, 115200);
    
    // Message to send
    uint8_t message[] = "Hello from STM32!\r\n";
    
    while (1)
    {
        // Send message if previous one is done
        if (tx_done)
        {
            tx_done = 0;
            HSerial_Send(HSERIAL_USART2, message, sizeof(message) - 1, TxCallback);
        }
        
        // Do other things here...
    }
}
```

---

### Example 2: Receive Fixed-Size Data

```c
#include "hserial_interface.h"

// Buffer for received data
uint8_t rx_buffer[10];

// Flag to indicate data received
volatile uint8_t data_ready = 0;

// Called when all bytes received
void RxCallback(HSerial_Status_t status)
{
    if (status == HSERIAL_RX_COMPLETE)
    {
        data_ready = 1;
    }
}

int main(void)
{
    // Initialize UART
    HSerial_Init(HSERIAL_USART2, 115200);
    
    // Start waiting for 10 bytes
    HSerial_Receive(HSERIAL_USART2, rx_buffer, 10, RxCallback);
    
    while (1)
    {
        if (data_ready)
        {
            // Process received data
            // rx_buffer now contains 10 bytes
            
            data_ready = 0;
            
            // Start receiving next 10 bytes
            HSerial_Receive(HSERIAL_USART2, rx_buffer, 10, RxCallback);
        }
    }
}
```

---

### Example 3: Echo (Receive and Send Back)

```c
#include "hserial_interface.h"

uint8_t buffer[1];  // Single byte buffer
volatile uint8_t byte_received = 0;

void RxCallback(HSerial_Status_t status)
{
    if (status == HSERIAL_RX_COMPLETE)
    {
        byte_received = 1;
    }
}

void TxCallback(HSerial_Status_t status)
{
    // Start receiving next byte after sending
    HSerial_Receive(HSERIAL_USART2, buffer, 1, RxCallback);
}

int main(void)
{
    HSerial_Init(HSERIAL_USART2, 115200);
    
    // Start receiving first byte
    HSerial_Receive(HSERIAL_USART2, buffer, 1, RxCallback);
    
    while (1)
    {
        if (byte_received)
        {
            byte_received = 0;
            
            // Send the received byte back
            HSerial_Send(HSERIAL_USART2, buffer, 1, TxCallback);
        }
    }
}
```

---

### Example 4: Send Numbers and Sensor Data

```c
#include "hserial_interface.h"
#include <stdio.h>   // For sprintf

uint8_t tx_buffer[100];
volatile uint8_t tx_ready = 1;

void TxCallback(HSerial_Status_t status)
{
    tx_ready = 1;
}

void SendNumber(int number)
{
    if (tx_ready)
    {
        tx_ready = 0;
        
        // Convert number to string
        int len = sprintf((char*)tx_buffer, "Value: %d\r\n", number);
        
        HSerial_Send(HSERIAL_USART2, tx_buffer, len, TxCallback);
    }
}

int main(void)
{
    HSerial_Init(HSERIAL_USART2, 115200);
    
    int sensor_value = 0;
    
    while (1)
    {
        // Simulate reading a sensor
        sensor_value++;
        
        // Send the value
        SendNumber(sensor_value);
        
        // Delay (replace with your delay function)
        for (volatile int i = 0; i < 1000000; i++);
    }
}
```

---

### Example 5: Simultaneous TX and RX

```c
#include "hserial_interface.h"

uint8_t tx_buffer[] = "Ping!\r\n";
uint8_t rx_buffer[20];

volatile uint8_t tx_done = 1;
volatile uint8_t rx_done = 0;

void TxCallback(HSerial_Status_t status)
{
    tx_done = 1;
}

void RxCallback(HSerial_Status_t status)
{
    rx_done = 1;
}

int main(void)
{
    HSerial_Init(HSERIAL_USART2, 115200);
    
    // Start receiving (waits in background)
    HSerial_Receive(HSERIAL_USART2, rx_buffer, 20, RxCallback);
    
    while (1)
    {
        // Send periodically
        if (tx_done)
        {
            tx_done = 0;
            HSerial_Send(HSERIAL_USART2, tx_buffer, sizeof(tx_buffer) - 1, TxCallback);
        }
        
        // Check if we received something
        if (rx_done)
        {
            rx_done = 0;
            
            // Process rx_buffer...
            
            // Start receiving again
            HSerial_Receive(HSERIAL_USART2, rx_buffer, 20, RxCallback);
        }
    }
}
```

---

## ⚙️ Common Baud Rates

| Baud Rate | Use Case |
|-----------|----------|
| 9600 | Slow but very reliable, GPS modules |
| 19200 | Legacy devices |
| 38400 | Some Bluetooth modules |
| 57600 | Moderate speed |
| **115200** | **Most common, recommended** |
| 230400 | Fast communication |
| 460800 | High speed |
| 921600 | Very high speed, short cables only |

**Tip:** Both devices must use the **same baud rate**!

---

## ❓ FAQ

### Q: My data looks like garbage?

**A:** Check these things:
1. ✅ Same baud rate on both devices?
2. ✅ TX connected to RX and RX connected to TX?
3. ✅ GND connected?
4. ✅ Correct voltage levels? (3.3V for STM32)

### Q: HSerial_Send returns HSERIAL_BUSY?

**A:** The previous transfer is still running. Wait for the callback before sending again.

```c
// Wrong ❌
HSerial_Send(...);
HSerial_Send(...);  // Will return BUSY!

// Correct ✅
HSerial_Send(..., MyCallback);
// Wait for MyCallback to be called
// Then send again
```

### Q: My callback is never called?

**A:** Make sure:
1. ✅ Interrupts are enabled globally (`__enable_irq()`)
2. ✅ DMA interrupt is enabled in NVIC
3. ✅ You're not stuck in a blocking function

### Q: How do I receive variable-length messages?

**A:** For messages where you don't know the length in advance:

**Option 1:** Receive byte-by-byte
```c
uint8_t byte;
HSerial_Receive(HSERIAL_USART2, &byte, 1, ByteReceivedCallback);
```

**Option 2:** Receive a large buffer and parse it
```c
uint8_t buffer[256];
HSerial_Receive(HSERIAL_USART2, buffer, 256, BufferFullCallback);
// Parse buffer to find message boundaries
```

### Q: Can I use printf?

**A:** Yes! Redirect printf to UART:

```c
// Add this to your code
int _write(int file, char *ptr, int len)
{
    static volatile uint8_t printf_busy = 0;
    
    while (printf_busy);  // Wait if busy
    printf_busy = 1;
    
    HSerial_Send(HSERIAL_USART2, (uint8_t*)ptr, len, PrintfCallback);
    
    return len;
}

void PrintfCallback(HSerial_Status_t status)
{
    printf_busy = 0;
}

// Now you can use:
printf("Hello %d\r\n", 42);
```

### Q: What's the maximum data size?

**A:** Up to **65535 bytes** (uint16_t max) per transfer.

### Q: Can I use multiple UARTs at the same time?

**A:** Yes! Each UART is independent:

```c
HSerial_Init(HSERIAL_USART1, 9600);    // For GPS
HSerial_Init(HSERIAL_USART2, 115200);  // For debug
HSerial_Init(HSERIAL_USART6, 38400);   // For Bluetooth

// All can send/receive simultaneously
HSerial_Send(HSERIAL_USART1, gps_cmd, ...);
HSerial_Send(HSERIAL_USART2, debug_msg, ...);
HSerial_Receive(HSERIAL_USART6, bt_buffer, ...);
```

---
 |

---

## 📁 File Structure

```
your_project/
├── include/
│   └── interface/
│       └── HAL/
│           └── hserial_interface.h    ← Include this
├── src/
│   └── HAL/
│       └── hserial_program.c          ← Driver implementation
└── main.c                             ← Your code
```

---

## 📜 License

This driver is provided as-is for educational purposes.

---

**Happy coding!** 🚀
