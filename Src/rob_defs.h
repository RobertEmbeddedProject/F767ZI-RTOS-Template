
void assert_failed(uint8_t *file, uint32_t line);
void LED_init(void);
void LED_Blink(uint16_t position);

//Structure and lookup table for LED position calling
typedef struct{GPIO_TypeDef *port; uint16_t pin;} LED;
static const LED tuning_led[19] = {
    {GPIOD, GPIO_PIN_7},   // P0 Red1
    {GPIOD, GPIO_PIN_6},   // P1 Red2
    {GPIOD, GPIO_PIN_5},   // P2 Red3
    {GPIOD, GPIO_PIN_4},   // P3 Red4
    {GPIOE, GPIO_PIN_2},   // P4 Yel1
    {GPIOE, GPIO_PIN_4},   // P5 Yel2
    {GPIOE, GPIO_PIN_5},   // P6 Yel3
    {GPIOE, GPIO_PIN_6},   // P7 Yel4
    {GPIOF, GPIO_PIN_13},   // P8 Grn1
    {GPIOE, GPIO_PIN_9},    // P9 Grn2
    {GPIOE, GPIO_PIN_11},   // P10 Grn3
    {GPIOE, GPIO_PIN_13},  // P11 Yel5
    {GPIOF, GPIO_PIN_15},  // P12 Yel6
    {GPIOG, GPIO_PIN_14},  // P13 Yel7
    {GPIOG, GPIO_PIN_9},    // P14 Yel8
    {GPIOE, GPIO_PIN_14},  // P15 Red5
    {GPIOE, GPIO_PIN_15},  // P16 Red6
    {GPIOB, GPIO_PIN_10},  // P17 Red7
    {GPIOB, GPIO_PIN_11}   // P18 Red8
};

static const LED status_led[4] = {
    {GPIOD, GPIO_PIN_3},   // P0 Wht1
    {GPIOF, GPIO_PIN_8},   // P1 Wht2
    {GPIOF, GPIO_PIN_14},  // P2 Wht3
    {GPIOE, GPIO_PIN_12},  // P3 Wht4
};