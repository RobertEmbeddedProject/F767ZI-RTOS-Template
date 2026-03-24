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

    /*
    23 LED Outputs, 4 are just for status:
    P0  Red1 = PD_7
    P1  Red2 = PD_6
    P2  Red3 = PD_5
    P3  Red4 = PD_4

    P4  Yel1 = PE_2
    P5  Yel2 = PE_4
    P6  Yel3 = PE_5
    P7  Yel4 = PE_6
    
    P08 Grn1 = PF_8
    P09 Grn2 = PF_13
    P10 Grn3 = PE_9
    
    P11 Yel5 = PF_14
    P12 Yel6 = PE_13
    P13 Yel7 = PF_15
    P14 Yel8 = PG_14
    
    P15 Red5 = PE_14
    P16 Red6 = PE_15
    P17 Red7 = PB_10
    P18 Red8 = PB_11

    P19  Wht1 = PD_3
    P20  Wht2 = PE_3
    P21 Wht3 = PE_11
    P22 Wht4 = PE_12
    */


void LED_Blink(uint16_t position){
    uint16_t delay = 80;
    HAL_GPIO_WritePin(acc_leds[position].port, acc_leds[position].pin, GPIO_PIN_SET);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    HAL_GPIO_WritePin(acc_leds[position].port, acc_leds[position].pin, GPIO_PIN_RESET);
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

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
                        GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13| GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
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