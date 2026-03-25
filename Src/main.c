#include "main.h"
#include "rob_defs.h"
#include <stdbool.h>
#include <stdint.h>                 //provides uint8 etc fixed width
#include <stddef.h>                 //provides ptrs, array size utilities
#include <FreeRTOS.h>
//#include <Nucleo_F767ZI_Init.h>   //Textbook hardware init "HWInit();"
//#include <Nucleo_F767ZI_GPIO.h>   //Textbook board support header
#include <stm32f7xx_hal_gpio.h>     //ST Hal
#include <stm32f7xx_hal.h>          //ST HAL
#include <task.h>                   //vTask APIs
#include <semphr.h>                 //semaphore APIs
#include <SEGGER_SYSVIEW.h>

void BlinkTask(void* nothing);
void ButtonTask(void* nothing);

//create storage for a pointer to a semaphore
//SemaphoreHandle_t semPtr = NULL;
//xSemaphoreGive(semPtr);
//if(xSemaphoreTake(semPtr, portMAX_DELAY) != pdPASS){
    //}

int main(void)
{
    //create a semaphore using the FreeRTOS Heap
	//semPtr = xSemaphoreCreateBinary();
	//assert_param(semPtr != NULL);

	HAL_Init();
    __HAL_RCC_GPIOB_CLK_ENABLE(); //Enable GPIO Port B Clock
    __HAL_RCC_GPIOC_CLK_ENABLE(); //Enable GPIO Port C Clock
    __HAL_RCC_GPIOD_CLK_ENABLE(); //Enable GPIO Port D Clock
    __HAL_RCC_GPIOE_CLK_ENABLE(); //Enable GPIO Port E Clock
    __HAL_RCC_GPIOF_CLK_ENABLE(); //Enable GPIO Port F Clock
    __HAL_RCC_GPIOG_CLK_ENABLE(); //Enable GPIO Port G Clock
    
    SEGGER_SYSVIEW_Conf();        //start SystemView
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    LED_init();                   //initialize LED output
    button_init();

        
    if (xTaskCreate(BlinkTask, "BlinkTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS){
         while(1); }
    if (xTaskCreate(ButtonTask, "ButtonTask", 128, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS){
         while(1); }


    vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
        SEGGER_SYSVIEW_PrintfHost("Problem!");
	}
}

void BlinkTask(void* nothing){
    int delay = 15;
    while(1){
        for(int i=0; i<19; i++){
            SEGGER_SYSVIEW_PrintfHost("i=%d", i);
            LED_ON(i);
            vTaskDelay(delay / portTICK_PERIOD_MS);
            if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET){
                LED_OFF(i);
            }
            else{
                i--;
            }
            
            if(i==18){
                for(int j=18; j>=0; j--){
                    SEGGER_SYSVIEW_PrintfHost("j=%d", j);
                    LED_ON(j);
                    vTaskDelay(delay / portTICK_PERIOD_MS);
                    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET){
                        LED_OFF(j);
                    }
                    else{
                        j++;
                    }
                }
            }
        }
    }
}

void ButtonTask(void* nothing){
    while(1){
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_SET){
            SEGGER_SYSVIEW_PrintfHost("PRESSED");
        }
        else{vTaskDelay(10 / portTICK_PERIOD_MS);}
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

