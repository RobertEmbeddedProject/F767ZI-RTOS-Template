#include "main.h"
#include <stdint.h>
#include <stddef.h>

/*
 * These are the STM32 GPIO pins physically wired to the LCD module.
 *
 * CS  = Chip Select. Active low. Must be low while talking to the display.
 * DC  = Data/Command. Low = command byte, High = data bytes.
 * RST = Hardware reset pin for the LCD controller.
 *
 * These come from YOUR board wiring, not from LVGL.
 */
#define LCD_CS_GPIO_Port    GPIOD
#define LCD_CS_Pin          GPIO_PIN_14

#define LCD_DC_GPIO_Port    GPIOD
#define LCD_DC_Pin          GPIO_PIN_15

#define LCD_RST_GPIO_Port   GPIOF
#define LCD_RST_Pin         GPIO_PIN_12

/*
 * Logical display size in the current orientation.
 * ILI9341 native resolution is 240x320.
 */
#define LCD_WIDTH           240U
#define LCD_HEIGHT          320U

/*
 * RGB565 color constants.
 * Format:
 *   RRRRRGGGGGGBBBBB
 *
 * These values are already in the 16-bit format expected by the LCD
 * once transmitted high byte first, then low byte.
 */
#define COLOR_BLACK         0x0000
#define COLOR_BLUE          0x001F
#define COLOR_RED           0xF800
#define COLOR_GREEN         0x07E0
#define COLOR_YELLOW        0xFFE0
#define COLOR_WHITE         0xFFFF

/*
 * Global SPI handle used by HAL.
 * HAL_SPI_Init() configures the peripheral described by this struct.
 */
SPI_HandleTypeDef hspi1;

/* If BSP already provides Error_Handler, only declare it here */
void Error_Handler(void);

/*
 * Local forward declarations.
 * These keep the file organized into:
 *  1) STM32 peripheral init
 *  2) LCD low-level bus helpers
 *  3) LCD controller commands
 *  4) Simple drawing primitives
 */
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

static void LCD_Select(void);
static void LCD_Unselect(void);
static void LCD_DC_Command(void);
static void LCD_DC_Data(void);
static void LCD_Reset(void);

static void LCD_WriteCommand(uint8_t cmd);
static void LCD_WriteData(const uint8_t *data, uint16_t size);
static void LCD_WriteDataByte(uint8_t data);

static void LCD_Init(void);
static void LCD_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
static void LCD_FillScreen(uint16_t color);

int main(void)
{
    /*
     * HAL_Init():
     * - initializes the HAL library
     * - sets up SysTick for HAL_Delay()
     * - performs basic MCU runtime setup
     *
     * This is not display-specific.
     */
    HAL_Init();

    /*
     * Configure the STM32 pins used for LCD control signals.
     * CS, DC, and RST are ordinary GPIO outputs.
     */
    MX_GPIO_Init();

    /*
     * Configure SPI1 as the byte stream used to send commands and pixel data
     * to the ILI9341 controller.
     */
    MX_SPI1_Init();

    /*
     * Start with CS inactive.
     * "Unselected" means the LCD is NOT currently listening to SPI traffic.
     */
    LCD_Unselect();
    HAL_Delay(50);

    /*
     * Reset the display and send the ILI9341 initialization sequence.
     * After this, the controller is awake, configured, and ready for pixel writes.
     */
    LCD_Init();

    HAL_Delay(50);

    /*
     * Basic smoke test:
     * fill the whole screen with a few solid colors.
     *
     * If these work, the essentials are proven:
     * - SPI is working
     * - GPIO control pins are correct
     * - LCD init sequence is valid
     * - pixel data format is likely correct
     */
    LCD_FillScreen(COLOR_BLACK);
    //HAL_Delay(100);

    //LCD_FillScreen(COLOR_RED);
    //HAL_Delay(100);

    //LCD_FillScreen(COLOR_GREEN);
    //HAL_Delay(100);

    //LCD_FillScreen(COLOR_BLUE);
    //HAL_Delay(100);

    /*
     * Draw a few rectangles to prove partial-region writes work,
     * not just full-screen writes.
     */
    for(int i=0; i<5; i++){
    LCD_FillRect(10, 10, 100, 60, COLOR_RED);
    LCD_FillRect(130, 10, 100, 60, COLOR_GREEN);
    LCD_FillRect(10, 90, 220, 80, COLOR_BLUE);
    LCD_FillRect(40, 200, 160, 80, COLOR_YELLOW);

    LCD_FillRect(10, 10, 100, 60, COLOR_GREEN);
    LCD_FillRect(130, 10, 100, 60, COLOR_BLUE);
    LCD_FillRect(10, 90, 220, 80, COLOR_YELLOW);
    LCD_FillRect(40, 200, 160, 80, COLOR_RED);

    LCD_FillRect(10, 10, 100, 60, COLOR_BLUE);
    LCD_FillRect(130, 10, 100, 60, COLOR_YELLOW);
    LCD_FillRect(10, 90, 220, 80, COLOR_RED);
    LCD_FillRect(40, 200, 160, 80, COLOR_GREEN);

    }

    while (1)
    {
        /*
         * Nothing else to do.
         * This demo is intentionally simple:
         * initialize once, draw once, then idle forever.
         */
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*
     * Enable the peripheral clocks for the GPIO ports that contain
     * the LCD control pins.
     *
     * Without these, the GPIO hardware blocks are not usable.
     */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    /*
     * Configure CS and DC as push-pull outputs.
     * These are software-driven control signals.
     */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = LCD_CS_Pin | LCD_DC_Pin;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*
     * Configure RESET pin the same way.
     */
    GPIO_InitStruct.Pin = LCD_RST_Pin;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*
     * Set safe idle levels:
     * - CS high = not selected
     * - DC high = data mode by default
     * - RST high = not held in reset
     */
    HAL_GPIO_WritePin(GPIOD, LCD_CS_Pin | LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOF, LCD_RST_Pin, GPIO_PIN_SET);
}

static void MX_SPI1_Init(void)
{
    /*
     * Bind this handle to SPI1 hardware.
     */
    hspi1.Instance = SPI1;

    /*
     * SPI settings that worked for this display:
     *
     * MASTER         -> STM32 drives the clock
     * 1LINE          -> write-only path to LCD
     * 8BIT           -> one byte at a time, normal for command/data writes
     * POLARITY_LOW   -> clock idle low
     * PHASE_1EDGE    -> sample on first edge
     * NSS_SOFT       -> chip select handled manually with GPIO
     * PRESCALER_64   -> conservative speed for reliable bring-up
     * MSB first      -> high bit shifted first
     */
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_1LINE;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;

    /*
     * HAL_SPI_Init() takes the desired settings above and programs
     * the STM32 SPI peripheral registers.
     */
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void LCD_Select(void)
{
    /* Active-low chip select: low means the LCD will accept SPI traffic. */
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

static void LCD_Unselect(void)
{
    /* High means the LCD ignores SPI traffic. */
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static void LCD_DC_Command(void)
{
    /*
     * D/C low means the next byte(s) on SPI are interpreted as command bytes.
     */
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
}

static void LCD_DC_Data(void)
{
    /*
     * D/C high means the next byte(s) on SPI are interpreted as data bytes.
     */
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
}

static void LCD_Reset(void)
{
    /*
     * Hardware reset pulse.
     *
     * The exact timing is based on common ILI9341 bring-up practice:
     * start high, pulse low, then wait long enough after releasing reset
     * for the controller to stabilize.
     */
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(5);

    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);

    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

static void LCD_WriteCommand(uint8_t cmd)
{
    /*
     * A command transaction is:
     *  1) assert CS
     *  2) set D/C = command
     *  3) transmit one command byte
     *  4) deassert CS
     */
    LCD_Select();
    LCD_DC_Command();

    if (HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        Error_Handler();
    }

    LCD_Unselect();
}

static void LCD_WriteData(const uint8_t *data, uint16_t size)
{
    /*
     * A data transaction is:
     *  1) assert CS
     *  2) set D/C = data
     *  3) transmit one or more data bytes
     *  4) deassert CS
     */
    LCD_Select();
    LCD_DC_Data();

    if (HAL_SPI_Transmit(&hspi1, (uint8_t *)data, size, HAL_MAX_DELAY) != HAL_OK)
    {
        Error_Handler();
    }

    LCD_Unselect();
}

static void LCD_WriteDataByte(uint8_t data)
{
    /* Convenience helper for single-byte register writes. */
    LCD_WriteData(&data, 1);
}

static void LCD_Init(void)
{
    /*
     * ===== HARD RESET =====
     * Ensures controller starts from a known state
     */
    LCD_Reset();

    /*
     * 0x01 - Software Reset
     * Resets internal registers (recommended even after HW reset)
     */
    LCD_WriteCommand(0x01);
    HAL_Delay(120);

    /*
     * ===== POWER CONTROL SEQUENCE =====
     * These are mostly analog/power settings from reference init sequences.
     * Typically taken from known-good code for the specific panel.
     */

    /*
     * 0xCB - Power Control A
     */
    { uint8_t d[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
      LCD_WriteCommand(0xCB);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xCF - Power Control B
     */
    { uint8_t d[] = {0x00, 0xC1, 0x30};
      LCD_WriteCommand(0xCF);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xE8 - Driver Timing Control A
     */
    { uint8_t d[] = {0x85, 0x00, 0x78};
      LCD_WriteCommand(0xE8);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xEA - Driver Timing Control B
     */
    { uint8_t d[] = {0x00, 0x00};
      LCD_WriteCommand(0xEA);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xED - Power On Sequence Control
     */
    { uint8_t d[] = {0x64, 0x03, 0x12, 0x81};
      LCD_WriteCommand(0xED);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xF7 - Pump Ratio Control
     */
    LCD_WriteCommand(0xF7);
    LCD_WriteDataByte(0x20);

    /*
     * 0xC0 - Power Control 1 (VRH)
     */
    LCD_WriteCommand(0xC0);
    LCD_WriteDataByte(0x23);

    /*
     * 0xC1 - Power Control 2 (SAP/BT)
     */
    LCD_WriteCommand(0xC1);
    LCD_WriteDataByte(0x10);

    /*
     * 0xC5 - VCOM Control 1
     */
    { uint8_t d[] = {0x3E, 0x28};
      LCD_WriteCommand(0xC5);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xC7 - VCOM Control 2
     */
    LCD_WriteCommand(0xC7);
    LCD_WriteDataByte(0x86);

    /*
     * ===== MEMORY / PIXEL FORMAT =====
     */

    /*
     * 0x36 - MADCTL (Memory Access Control)
     *
     * Bit meaning (important):
     *  MY, MX  -> row/column flip
     *  MV      -> row/column swap (rotation)
     *  BGR     -> color order
     *
     * 0x48 = typical portrait + BGR setting
     */
    LCD_WriteCommand(0x36);
    LCD_WriteDataByte(0x48);

    /*
     * 0x3A - COLMOD (Pixel Format Set)
     *
     * 0x55 = 16 bits per pixel (RGB565)
     */
    LCD_WriteCommand(0x3A);
    LCD_WriteDataByte(0x55);

    /*
     * ===== FRAME / DISPLAY TIMING =====
     */

    /*
     * 0xB1 - Frame Rate Control
     */
    { uint8_t d[] = {0x00, 0x18};
      LCD_WriteCommand(0xB1);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xB6 - Display Function Control
     */
    { uint8_t d[] = {0x08, 0x82, 0x27};
      LCD_WriteCommand(0xB6);
      LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xF2 - Enable 3G Gamma Control
     */
    LCD_WriteCommand(0xF2);
    LCD_WriteDataByte(0x00);

    /*
     * 0x26 - Gamma Set
     */
    LCD_WriteCommand(0x26);
    LCD_WriteDataByte(0x01);

    /*
     * 0xE0 - Positive Gamma Correction
     */
    {
        uint8_t d[] = {
            0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
            0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00
        };
        LCD_WriteCommand(0xE0);
        LCD_WriteData(d, sizeof(d));
    }

    /*
     * 0xE1 - Negative Gamma Correction
     */
    {
        uint8_t d[] = {
            0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
            0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F
        };
        LCD_WriteCommand(0xE1);
        LCD_WriteData(d, sizeof(d));
    }

    /*
     * ===== EXIT SLEEP / TURN ON =====
     */

    /*
     * 0x11 - Sleep Out
     * Must wait after this per datasheet
     */
    LCD_WriteCommand(0x11);
    HAL_Delay(120);

    /*
     * 0x29 - Display ON
     * Panel begins displaying GRAM contents
     */
    LCD_WriteCommand(0x29);
    HAL_Delay(20);
}

static void LCD_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    /*
     * The ILI9341 writes pixels into a rectangular "window".
     *
     * 0x2A = Column Address Set
     * 0x2B = Page/Row Address Set
     * 0x2C = Memory Write
     *
     * After 0x2C, the next stream of pixel bytes fills this region.
     */
    LCD_WriteCommand(0x2A);
    data[0] = (uint8_t)(x0 >> 8);
    data[1] = (uint8_t)(x0 & 0xFF);
    data[2] = (uint8_t)(x1 >> 8);
    data[3] = (uint8_t)(x1 & 0xFF);
    LCD_WriteData(data, 4);

    LCD_WriteCommand(0x2B);
    data[0] = (uint8_t)(y0 >> 8);
    data[1] = (uint8_t)(y0 & 0xFF);
    data[2] = (uint8_t)(y1 >> 8);
    data[3] = (uint8_t)(y1 & 0xFF);
    LCD_WriteData(data, 4);

    /* After this command, pixel data bytes are interpreted as RAM write data. */
    LCD_WriteCommand(0x2C);
}

static void LCD_FillScreen(uint16_t color)
{
    /* Full-screen fill is just a rectangle covering the whole display. */
    LCD_FillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

static void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t pixels;
    uint8_t hi;
    uint8_t lo;
    uint8_t block[128];
    uint16_t i;

    /*
     * Reject rectangles that start fully outside the screen.
     */
    if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
    {
        return;
    }

    /*
     * Clip width/height so we never write past the edge of the display.
     */
    if ((x + w) > LCD_WIDTH)
    {
        w = LCD_WIDTH - x;
    }

    if ((y + h) > LCD_HEIGHT)
    {
        h = LCD_HEIGHT - y;
    }

    /*
     * Tell the LCD which region of GRAM we are about to fill.
     */
    LCD_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    /*
     * RGB565 is sent high byte first, then low byte.
     */
    hi = (uint8_t)(color >> 8);
    lo = (uint8_t)(color & 0xFF);

    /*
     * Build a small reusable transmission block containing repeated copies
     * of the same color. This avoids transmitting one pixel at a time.
     */
    for (i = 0; i < sizeof(block); i += 2)
    {
        block[i] = hi;
        block[i + 1] = lo;
    }

    pixels = (uint32_t)w * (uint32_t)h;

    /*
     * Enter one long data transfer for the rectangle.
     * Since 0x2C was already sent by LCD_SetAddressWindow(), the controller
     * now treats each following byte pair as pixel data.
     */
    LCD_Select();
    LCD_DC_Data();

    while (pixels > 0U)
    {
        /*
         * Send as many repeated-color pixels as fit in our local block.
         * block[128] holds 64 RGB565 pixels because each pixel is 2 bytes.
         */
        uint16_t chunk_pixels = (pixels > (sizeof(block) / 2U)) ? (sizeof(block) / 2U) : (uint16_t)pixels;
        uint16_t chunk_bytes = (uint16_t)(chunk_pixels * 2U);

        if (HAL_SPI_Transmit(&hspi1, block, chunk_bytes, HAL_MAX_DELAY) != HAL_OK)
        {
            Error_Handler();
        }

        pixels -= chunk_pixels;
    }

    LCD_Unselect();
}
