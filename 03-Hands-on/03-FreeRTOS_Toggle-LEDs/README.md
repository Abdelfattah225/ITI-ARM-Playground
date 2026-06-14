
# FreeRTOS LED Toggle Task

## Task Description

This task demonstrates basic multitasking using **FreeRTOS**.

The objective is to:

* Initialize system clock and GPIO
* Initialize LEDs using the HAL layer
* Create two FreeRTOS tasks
* Toggle two different LEDs at different time intervals
* Handle RTOS hooks for system safety

---

## What I Implemented

### 1️⃣ System Initialization

* `RCC_Init()` → Initialize system clock
* `LED_init()` → Initialize LED configuration

---

### 2️⃣ Created Two Tasks

#### Task 1

* Toggles `LED_DOOROPEN`
* Delay: 1000 ticks
* Priority: 1

#### Task 2

* Toggles `LED_WARNING`
* Delay: 500 ticks
* Priority: 2

Both tasks:

* Run inside an infinite loop
* Use `vTaskDelay()` for periodic execution

---

### 3️⃣ Started Scheduler

```c
vTaskStartScheduler();
```

The scheduler handles task switching based on priority and delays.

---

## FreeRTOS Hooks Implemented

### Idle Hook

Runs when no task is ready.

```c
void vApplicationIdleHook(void);
```

---

### Tick Hook

Runs every system tick interrupt.

```c
void vApplicationTickHook(void);
```

---

### Stack Overflow Hook

If stack overflow occurs, system enters infinite loop.

```c
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
```

---

### Malloc Failed Hook

If dynamic memory allocation fails, system enters infinite loop.

```c
void vApplicationMallocFailedHook(void);
```

---

## What I Learned From This Task

* How to create tasks using `xTaskCreate()`
* How task priorities affect scheduling
* How to use `vTaskDelay()` for timing
* How FreeRTOS hooks work
* Basic RTOS debugging structure
* Separation between MCAL and HAL layers

---


