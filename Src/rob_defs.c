#include "main.h"
#include "rob_defs.h"
#include <stdint.h>             //provides uint8 etc fixed width
#include <stddef.h>            //provides ptrs, array size utilities
#include <FreeRTOS.h>
//#include <Nucleo_F767ZI_Init.h>  //Textbook hardware init "HWInit();"
//#include <Nucleo_F767ZI_GPIO.h> //Textbook board support header
#include <stm32f7xx_hal_gpio.h>   //ST Hal
#include <stm32f7xx_hal.h>      //ST HAL
#include <task.h>               //vTask APIs
#include <SEGGER_SYSVIEW.h>

void LED_Blink(uint16_t position){
    uint16_t delay = 10;
    HAL_GPIO_WritePin(tuning_led[position].port, tuning_led[position].pin, GPIO_PIN_SET);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    HAL_GPIO_WritePin(tuning_led[position].port, tuning_led[position].pin, GPIO_PIN_RESET);
    vTaskDelay(delay / portTICK_PERIOD_MS);
}

void LED_init(void){  //reference Drivers/STM32F7xx_HAL_Driver/stm32f7xx_hal_gpio.h
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

                 //Port  //Struct
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //combine each 0b000.. bitwise operator to one mapping before initialization
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
                        GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13| GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void button_init(void){
    /*
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_0
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

                 //Port  //Struct
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    */
}

//checks if task creation was successful
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1)
    {
        // optional breakpoint location
    }
}