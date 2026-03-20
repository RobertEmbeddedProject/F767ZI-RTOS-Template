
//Robs ISR Testing Program

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <SEGGER_SYSVIEW.h>
#include <Nucleo_F767ZI_GPIO.h>
#include <Nucleo_F767ZI_Init.h>
#include <stm32f7xx_hal.h>
#include <stdint.h>

// some common variables to use for each task
// 128 * 4 = 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 128

void GreenTaskA( void* argument);
void BlueTaskB( void* argumet );
void RedTaskC( void* argumet );

volatile uint32_t flag = 0;

int main(void)
{
	HWInit();
	SEGGER_SYSVIEW_Conf();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);	//ensure proper priority grouping for freeRTOS

	assert_param(xTaskCreate(GreenTaskA, "GreenTaskA", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL) == pdPASS);
	assert_param(xTaskCreate(BlueTaskB, "BlueTaskB", STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) == pdPASS);
	assert_param(xTaskCreate(RedTaskC, "RedTaskC", STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, NULL) == pdPASS);
	vTaskStartScheduler();

	//If you wound up here, then scheduler may have failed.
	while(1)
	{
	}
}

void GreenTaskA( void* argument )
{
	uint_fast8_t count = 0;
	while(1)
	{
		//every 5 times through the loop, set the flag
		if(++count >= 5)
		{
			count = 0;
			SEGGER_SYSVIEW_PrintfHost("Task A (green LED) sets flag");
			flag = 1;	//set 'flag' to 1 to "signal" BlueTaskB to run
		}
		GreenLed.On();
		vTaskDelay(300/portTICK_PERIOD_MS);
		GreenLed.Off();
		vTaskDelay(300/portTICK_PERIOD_MS);
	}
}

void BlueTaskB( void* argument )
{
	while(1)
	{
		for(uint_fast8_t i = 0; i < 3; i++)
		{
			BlueLed.On();
			vTaskDelay(300/portTICK_PERIOD_MS);
			BlueLed.Off();
			vTaskDelay(300/portTICK_PERIOD_MS);
		}
	}
}

void RedTaskC(void* argument){
	while(1){
		SEGGER_SYSVIEW_PrintfHost("Custom Red Task");
		RedLed.On();
		vTaskDelay(100/portTICK_PERIOD_MS);
		RedLed.Off();
		vTaskDelay(100/portTICK_PERIOD_MS);
		RedLed.On();
		vTaskDelay(100/portTICK_PERIOD_MS);
		RedLed.Off();
		vTaskDelay(2000/portTICK_PERIOD_MS);


	}
}

