#include "main.h"
#include <stdint.h>             //provides uint8 etc fixed width
#include <stddef.h>            //provides ptrs, array size utilities
#include <FreeRTOS.h>
#include <Nucleo_F767ZI_Init.h>  //Textbook hardware init "HWInit();"
//#include <Nucleo_F767ZI_GPIO.h> //Textbook board support header
#include <stm32f7xx_hal.h>      //HAL APIs for GPIO, UART, SPI, I2C, timers,
#include <task.h>               //vTask APIs
#include <SEGGER_SYSVIEW.h>

void TestTask(void* nothing);

int main(void)
{
	HWInit();
    SEGGER_SYSVIEW_Conf();    //start SystemView
        
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
        vTaskDelay(4000/ portTICK_PERIOD_MS);
        uint8_t num = 14;
        SEGGER_SYSVIEW_PrintfHost("%d",num);
    }
}

