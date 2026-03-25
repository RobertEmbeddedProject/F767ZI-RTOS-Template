#include "main.h"
#include "rob_defs.h"

void BlinkTask(void* nothing);

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

    vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
        SEGGER_SYSVIEW_PrintfHost("Problem!");
	}
}

void BlinkTask(void* nothing){
    int delay = 15;

    for(int i=0; i<5; i++){
        blink_status();
    }

    while(1){
        for(int i=0; i<19; i++){
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
