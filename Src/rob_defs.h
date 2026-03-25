#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>            //provides ptrs, array size utilities
#include <FreeRTOS.h>
#include <semphr.h>                 //semaphore APIs
#include <stm32f7xx_hal.h>      //ST HAL
#include <task.h>               //vTask APIs
#include <SEGGER_SYSVIEW.h>

void assert_failed(uint8_t *file, uint32_t line);
void LED_init(void);
void LED_ON(uint16_t position);
void LED_OFF(uint16_t position);
void blink_status();
void button_init(void);

//Structure and lookup table for LED position calling
typedef struct{GPIO_TypeDef *port; uint16_t pin;} LED;
