
void assert_failed(uint8_t *file, uint32_t line);
void LED_init(void);
void LED_Blink(uint16_t position);

//Structure and lookup table for LED position calling
typedef struct{GPIO_TypeDef *port; uint16_t pin;} LED;
static const LED leds[] = {
    {GPIOD, GPIO_PIN_7},   // P0 Red1
    {GPIOD, GPIO_PIN_6},   // P1 Red2
    {GPIOD, GPIO_PIN_5},   // P2 Red3
    {GPIOD, GPIO_PIN_4},   // P3 Red4
    {GPIOD, GPIO_PIN_3},   // P4 Wht1
    {GPIOE, GPIO_PIN_2},   // P5 Yel1
    {GPIOE, GPIO_PIN_4},   // P6 Yel2
    {GPIOE, GPIO_PIN_5},   // P7 Yel3
    {GPIOE, GPIO_PIN_6},   // P8 Yel4
    {GPIOF, GPIO_PIN_8},   // P9 Wht2
    {GPIOF, GPIO_PIN_13},   // P10 Grn1
    {GPIOE, GPIO_PIN_9},    // P11 Grn2
    {GPIOE, GPIO_PIN_11},   // P12 Grn3
    {GPIOF, GPIO_PIN_14},  // P13 Wht3
    {GPIOE, GPIO_PIN_13},  // P14 Yel5
    {GPIOF, GPIO_PIN_15},  // P15 Yel6
    {GPIOG, GPIO_PIN_14},  // P16 Yel7
    {GPIOG, GPIO_PIN_9},  // P17 Yel8*
    {GPIOE, GPIO_PIN_12},  // P18 Wht4
    {GPIOE, GPIO_PIN_14},  // P19 Red5
    {GPIOE, GPIO_PIN_15},  // P20 Red6
    {GPIOB, GPIO_PIN_10},  // P21 Red7
    {GPIOB, GPIO_PIN_11}   // P22 Red8
};
