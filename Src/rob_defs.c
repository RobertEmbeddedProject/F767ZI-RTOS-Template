#include "rob_defs.h"

//extern volatile bool paused;   //extern tells other files that this exists
                               //never initialize when declaring as extern


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
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

                 //Port  //Struct
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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
static const LED tuning_led[19] = {
    {GPIOD, GPIO_PIN_7},   // P0 Red1
    {GPIOD, GPIO_PIN_6},   // P1 Red2
    {GPIOD, GPIO_PIN_5},   // P2 Red3
    {GPIOD, GPIO_PIN_4},   // P3 Red4
    {GPIOE, GPIO_PIN_2},   // P4 Yel1
    {GPIOE, GPIO_PIN_4},   // P5 Yel2
    {GPIOE, GPIO_PIN_5},   // P6 Yel3
    {GPIOE, GPIO_PIN_6},   // P7 Yel4
    {GPIOF, GPIO_PIN_13},  // P8 Grn1
    {GPIOE, GPIO_PIN_9},   // P9 Grn2
    {GPIOE, GPIO_PIN_11},  // P10 Grn3
    {GPIOE, GPIO_PIN_13},  // P11 Yel5
    {GPIOF, GPIO_PIN_15},  // P12 Yel6
    {GPIOG, GPIO_PIN_14},  // P13 Yel7
    {GPIOG, GPIO_PIN_9},   // P14 Yel8
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

void LED_ON(uint16_t position){
    HAL_GPIO_WritePin(tuning_led[position].port, tuning_led[position].pin, GPIO_PIN_SET);
}

void LED_OFF(uint16_t position){
    HAL_GPIO_WritePin(tuning_led[position].port, tuning_led[position].pin, GPIO_PIN_RESET);
}

void blink_status(){
    HAL_GPIO_WritePin(status_led[0].port, status_led[0].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(status_led[1].port, status_led[1].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(status_led[2].port, status_led[2].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(status_led[3].port, status_led[3].pin, GPIO_PIN_SET);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    HAL_GPIO_WritePin(status_led[0].port, status_led[0].pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(status_led[1].port, status_led[1].pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(status_led[2].port, status_led[2].pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(status_led[3].port, status_led[3].pin, GPIO_PIN_RESET);
    vTaskDelay(50 / portTICK_PERIOD_MS);
}
