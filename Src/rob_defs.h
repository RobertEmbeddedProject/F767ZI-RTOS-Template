
void assert_failed(uint8_t *file, uint32_t line);
void LED_init(void);
void LED_Blink(uint16_t position);

//Structure and lookup table for LED position calling
typedef struct{GPIO_TypeDef *port; uint16_t pin;} LED;
static const LED acc_leds[19] = {
    {GPIOD, GPIO_PIN_7},   // Red1
    {GPIOD, GPIO_PIN_6},   // Red2
    {GPIOD, GPIO_PIN_5},   // Red3
    {GPIOD, GPIO_PIN_4},   // Red4
    
    {GPIOE, GPIO_PIN_2},   // Yel1
    {GPIOE, GPIO_PIN_4},   // Yel2
    {GPIOE, GPIO_PIN_5},   // Yel3
    {GPIOE, GPIO_PIN_6},   // Yel4

    {GPIOF, GPIO_PIN_8},   // Grn1
    {GPIOF, GPIO_PIN_13},  // Grn2
    {GPIOE, GPIO_PIN_9},   // Grn3
    
    {GPIOF, GPIO_PIN_14},  // Yel5
    {GPIOE, GPIO_PIN_13},  // Yel6
    {GPIOF, GPIO_PIN_15},  // Yel7
    {GPIOG, GPIO_PIN_14},  // Yel8
    
    {GPIOE, GPIO_PIN_14},  // Red5
    {GPIOE, GPIO_PIN_15},  // Red6
    {GPIOB, GPIO_PIN_10},  // Red7
    {GPIOB, GPIO_PIN_11}   // Red8
};

static const LED stat_leds[4] = {
    {GPIOD, GPIO_PIN_3},   // Wht1
    {GPIOE, GPIO_PIN_3},   // Wht2
    {GPIOE, GPIO_PIN_11},  // Wht3
    {GPIOE, GPIO_PIN_12}   // Wht4
};
