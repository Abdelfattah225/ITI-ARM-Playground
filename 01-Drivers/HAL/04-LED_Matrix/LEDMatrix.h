#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <stdint.h>
#include "../../../include/interface/MCAL/GPIO_Interface.h"

/* ============================================
 * Configuration Macros
 * ============================================ */
#define LMAX_ROWS           8
#define LMAX_COLS           8
#define LED_MATRIX_MAX      1   /* Number of LED matrices in your project */

/* ============================================
 * Type Definitions
 * ============================================ */

/* Matrix Type */
typedef enum
{
    LMAX_COMMON_CATHODE = 0,
    LMAX_COMMON_ANODE
} LMAX_Type_t;

/* Error Codes */
typedef enum
{
    LMAX_OK = 0,
    LMAX_ERROR,
    LMAX_INVALID_PARAM,
    LMAX_NOT_INITIALIZED
} LMAX_enu_err_t;

/* Matrix Names/IDs */
typedef enum
{
    LMAX_1 = 0,
    /* Add more if needed: LMAX_2, LMAX_3, etc. */
    LMAX_COUNT
} LMAX_names_t;

/* Pin State */
typedef enum
{
    LMAX_PIN_LOW = 0,
    LMAX_PIN_HIGH
} LMAX_PinState_t;

/* Matrix Configuration Structure */
typedef struct
{
    LMAX_Type_t type;                       /* Common cathode or anode */
    GPIO_TypeDef *Row_ports[LMAX_ROWS];     /* Port for each row */
    uint16_t Row_pins[LMAX_ROWS];           /* Pin number for each row */
    GPIO_TypeDef *Column_ports[LMAX_COLS];  /* Port for each column */
    uint16_t Column_pins[LMAX_COLS];        /* Pin number for each column */
} LMAX_Cfg_t;

/* Runtime State Structure */
typedef struct
{
    uint8_t enabled;                        /* Matrix enabled flag */
    uint8_t currentRow;                     /* Current row being scanned */
    uint8_t frameBuffer[LMAX_ROWS];         /* Display buffer */
    uint8_t brightness;                     /* Brightness level (0-100) */
} LMAX_State_t;

/* ============================================
 * External Configuration Array (defined in LEDMatrix_Cfg.c)
 * ============================================ */
extern const LMAX_Cfg_t LMAX_Cfg[LED_MATRIX_MAX];

/* ============================================
 * API Functions
 * ============================================ */

/**
 * @brief Initialize a specific LED matrix
 * @param name Matrix identifier
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_Init(LMAX_names_t name);

/**
 * @brief Enable a specific LED matrix
 * @param name Matrix identifier
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_Enable(LMAX_names_t name);

/**
 * @brief Disable a specific LED matrix
 * @param name Matrix identifier
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_Disable(LMAX_names_t name);

/**
 * @brief Set the entire frame buffer
 * @param name Matrix identifier
 * @param buffer Array of LMAX_ROWS bytes
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_SetBuffer(LMAX_names_t name, const uint8_t buffer[LMAX_ROWS]);

/**
 * @brief Clear the display
 * @param name Matrix identifier
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_Clear(LMAX_names_t name);

/**
 * @brief Fill the display (all LEDs on)
 * @param name Matrix identifier
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_Fill(LMAX_names_t name);

/**
 * @brief Set a single pixel
 * @param name Matrix identifier
 * @param row Row index (0 to LMAX_ROWS-1)
 * @param col Column index (0 to LMAX_COLS-1)
 * @param state LMAX_PIN_HIGH or LMAX_PIN_LOW
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_SetPixel(LMAX_names_t name, uint8_t row, uint8_t col, LMAX_PinState_t state);

/**
 * @brief Get a single pixel value
 * @param name Matrix identifier
 * @param row Row index
 * @param col Column index
 * @return Pixel state (0 or 1)
 */
uint8_t LMAX_GetPixel(LMAX_names_t name, uint8_t row, uint8_t col);

/**
 * @brief Set a single row
 * @param name Matrix identifier
 * @param row Row index
 * @param data 8-bit row data
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_SetRow(LMAX_names_t name, uint8_t row, uint8_t data);

/**
 * @brief Set a single column
 * @param name Matrix identifier
 * @param col Column index
 * @param data 8-bit column data
 * @return LMAX_OK on success
 */
LMAX_enu_err_t LMAX_SetColumn(LMAX_names_t name, uint8_t col, uint8_t data);

/**
 * @brief Refresh task - call from scheduler
 * @param arg Unused (for scheduler compatibility)
 */
void LMAX_RefreshTask(void *arg);

/**
 * @brief Get pointer to frame buffer for direct access
 * @param name Matrix identifier
 * @return Pointer to buffer or NULL on error
 */
uint8_t* LMAX_GetBufferPtr(LMAX_names_t name);

#endif /* LEDMATRIX_H */