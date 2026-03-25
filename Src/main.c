#include "main.h"
#include "rob_defs.h"
#include <stdint.h>                 //provides uint8 etc fixed width
#include <stddef.h>                 //provides ptrs, array size utilities
#include <FreeRTOS.h>
//#include <Nucleo_F767ZI_Init.h>   //Textbook hardware init "HWInit();"
//#include <Nucleo_F767ZI_GPIO.h>   //Textbook board support header
#include <stm32f7xx_hal_gpio.h>     //ST Hal
#include <stm32f7xx_hal.h>          //ST HAL
#include <task.h>                   //vTask APIs
#include <SEGGER_SYSVIEW.h>

void TestTask(void* nothing);

int main(void)
{
	HAL_Init();
    __HAL_RCC_GPIOB_CLK_ENABLE(); //Enable GPIO Port B Clock
    __HAL_RCC_GPIOD_CLK_ENABLE(); //Enable GPIO Port D Clock
    __HAL_RCC_GPIOE_CLK_ENABLE(); //Enable GPIO Port E Clock
    __HAL_RCC_GPIOF_CLK_ENABLE(); //Enable GPIO Port F Clock
    __HAL_RCC_GPIOG_CLK_ENABLE(); //Enable GPIO Port G Clock
    
    SEGGER_SYSVIEW_Conf();        //start SystemView
    LED_init();                   //initialize LED output
        
    if (xTaskCreate(TestTask, "TestTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS){
         while(1); }


    vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
        SEGGER_SYSVIEW_PrintfHost("Problem!");
	}
}

void TestTask(void* nothing){
    while(1){
        for(int i=0; i<19; i++){
            SEGGER_SYSVIEW_PrintfHost("i=%d", i);
            LED_Blink(i);
            if(i==18){
                for(int j=18; j>=0; j--){
                    SEGGER_SYSVIEW_PrintfHost("j=%d", j);
                    LED_Blink(j);
                }
            }
        }
    }
}
