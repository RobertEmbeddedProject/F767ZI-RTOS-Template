#include <stdint.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"

void assert_failed(uint8_t *file, uint32_t line);
void LED_init(void);
void button_init(void);
void LED_Blink(uint16_t position, bool paused);

//Structure and lookup table for LED position calling
typedef struct{GPIO_TypeDef *port; uint16_t pin;} LED;
static const LED tuning_led[19];
static const LED status_led[4];
