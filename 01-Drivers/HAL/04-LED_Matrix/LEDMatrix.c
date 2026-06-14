#include "../../../include/interface/HAL/LEDMatrix.h"
#include <string.h>

/* ============================================
 * Private Variables
 * ============================================ */
static LMAX_State_t LED_Matrix[LED_MATRIX_MAX];

/* ============================================
 * Private Functions
 * ============================================ */

static void LMAX_DisableAllRows(const LMAX_Cfg_t *cfg)
{
    uint8_t i;
    uint8_t disable_state = (cfg->type == LMAX_COMMON_CATHODE) ? 0 : 1;
    
    for (i = 0; i < LMAX_ROWS; i++)
    {
        GPIO_WritePin(cfg->Row_ports[i], cfg->Row_pins[i], disable_state);
    }
}

static void LMAX_EnableRow(const LMAX_Cfg_t *cfg, uint8_t row)
{
    uint8_t enable_state = (cfg->type == LMAX_COMMON_CATHODE) ? 1 : 0;
    GPIO_WritePin(cfg->Row_ports[row], cfg->Row_pins[row], enable_state);
}
static void LMAX_WriteColumns(const LMAX_Cfg_t *cfg, uint8_t data)
{
    uint8_t i;
    uint8_t bit;
    
    /* Invert the data here */
    data = ~data;  /* ← ADD THIS LINE */
    
    for (i = 0; i < LMAX_COLS; i++)
    {
        bit = (data >> i) & 0x01;
        
        if (cfg->type == LMAX_COMMON_CATHODE)
        {
            GPIO_WritePin(cfg->Column_ports[i], cfg->Column_pins[i], bit);
        }
        else
        {
            GPIO_WritePin(cfg->Column_ports[i], cfg->Column_pins[i], !bit);
        }
    }
}
static void LMAX_ClearColumns(const LMAX_Cfg_t *cfg)
{
    uint8_t i;
    uint8_t off_state = (cfg->type == LMAX_COMMON_CATHODE) ? 0 : 1;
    
    for (i = 0; i < LMAX_COLS; i++)
    {
        GPIO_WritePin(cfg->Column_ports[i], cfg->Column_pins[i], off_state);
    }
}

/* ============================================
 * Public Functions
 * ============================================ */

LMAX_enu_err_t LMAX_Init(LMAX_names_t name)
{
    uint8_t i;
    GPIO_PinConfig_t pinConfig;
    
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    const LMAX_Cfg_t *cfg = &LMAX_Cfg[name];
    
    /* Configure GPIO pins as push-pull outputs */
    pinConfig.Mode = GPIO_MODE_OUTPUT;
    pinConfig.OutputType = GPIO_OUTPUT_PUSHPULL;
    pinConfig.Speed = GPIO_SPEED_VERYHIGH;
    pinConfig.PuPd = GPIO_PUPD_NONE;
    
    /* Initialize row pins */
    for (i = 0; i < LMAX_ROWS; i++)
    {
        pinConfig.Pin = cfg->Row_pins[i];
        GPIO_PinInit(cfg->Row_ports[i], &pinConfig);
    }
    
    /* Initialize column pins */
    for (i = 0; i < LMAX_COLS; i++)
    {
        pinConfig.Pin = cfg->Column_pins[i];
        GPIO_PinInit(cfg->Column_ports[i], &pinConfig);
    }
    
    /* Initialize runtime state */
    LED_Matrix[name].enabled = 0;
    LED_Matrix[name].currentRow = 0;
    LED_Matrix[name].brightness = 100;
    memset(LED_Matrix[name].frameBuffer, 0, LMAX_ROWS);
    
    /* Ensure all LEDs are off */
    LMAX_DisableAllRows(cfg);
    LMAX_ClearColumns(cfg);
    
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_Enable(LMAX_names_t name)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    LED_Matrix[name].enabled = 1;
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_Disable(LMAX_names_t name)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    LED_Matrix[name].enabled = 0;
    
    /* Turn off all LEDs */
    LMAX_DisableAllRows(&LMAX_Cfg[name]);
    LMAX_ClearColumns(&LMAX_Cfg[name]);
    
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_SetBuffer(LMAX_names_t name, const uint8_t buffer[LMAX_ROWS])
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    if (buffer == NULL)
    {
        return LMAX_INVALID_PARAM;
    }
    
    memcpy(LED_Matrix[name].frameBuffer, buffer, LMAX_ROWS);
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_Clear(LMAX_names_t name)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    memset(LED_Matrix[name].frameBuffer, 0x00, LMAX_ROWS);
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_Fill(LMAX_names_t name)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    memset(LED_Matrix[name].frameBuffer, 0xFF, LMAX_ROWS);
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_SetPixel(LMAX_names_t name, uint8_t row, uint8_t col, LMAX_PinState_t state)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    if (row >= LMAX_ROWS || col >= LMAX_COLS)
    {
        return LMAX_INVALID_PARAM;
    }
    
    if (state == LMAX_PIN_HIGH)
    {
        LED_Matrix[name].frameBuffer[row] |= (1U << col);
    }
    else
    {
        LED_Matrix[name].frameBuffer[row] &= ~(1U << col);
    }
    
    return LMAX_OK;
}

uint8_t LMAX_GetPixel(LMAX_names_t name, uint8_t row, uint8_t col)
{
    if (name >= LED_MATRIX_MAX || row >= LMAX_ROWS || col >= LMAX_COLS)
    {
        return 0;
    }
    
    return (LED_Matrix[name].frameBuffer[row] >> col) & 0x01;
}

LMAX_enu_err_t LMAX_SetRow(LMAX_names_t name, uint8_t row, uint8_t data)
{
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    if (row >= LMAX_ROWS)
    {
        return LMAX_INVALID_PARAM;
    }
    
    LED_Matrix[name].frameBuffer[row] = data;
    return LMAX_OK;
}

LMAX_enu_err_t LMAX_SetColumn(LMAX_names_t name, uint8_t col, uint8_t data)
{
    uint8_t i;
    
    if (name >= LED_MATRIX_MAX)
    {
        return LMAX_ERROR;
    }
    
    if (col >= LMAX_COLS)
    {
        return LMAX_INVALID_PARAM;
    }
    
    for (i = 0; i < LMAX_ROWS; i++)
    {
        if (data & (1U << i))
        {
            LED_Matrix[name].frameBuffer[i] |= (1U << col);
        }
        else
        {
            LED_Matrix[name].frameBuffer[i] &= ~(1U << col);
        }
    }
    
    return LMAX_OK;
}

uint8_t* LMAX_GetBufferPtr(LMAX_names_t name)
{
    if (name >= LED_MATRIX_MAX)
    {
        return NULL;
    }
    
    return LED_Matrix[name].frameBuffer;
}

void LMAX_RefreshTask(void *arg)
{
    (void)arg;  /* Unused parameter */
    
    LMAX_names_t name;
    const LMAX_Cfg_t *cfg;
    LMAX_State_t *state;
    
    /* Process each configured matrix */
    for (name = (LMAX_names_t)0; name < LED_MATRIX_MAX; name++)
    {
        cfg = &LMAX_Cfg[name];
        state = &LED_Matrix[name];
        
        /* Skip disabled matrices */
        if (!state->enabled)
        {
            continue;
        }
        
        /* Step 1: Disable all rows (prevents ghosting) */
        LMAX_DisableAllRows(cfg);
        
        /* Step 2: Write column data for current row */
        LMAX_WriteColumns(cfg, state->frameBuffer[state->currentRow]);
        
        /* Step 3: Enable current row */
        LMAX_EnableRow(cfg, state->currentRow);
        
        /* Step 4: Advance to next row */
        state->currentRow++;
        if (state->currentRow >= LMAX_ROWS)
        {
            state->currentRow = 0;
        }
    }
}