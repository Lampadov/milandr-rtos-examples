#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>
#include "FreeRTOS.h"
#include "task.h"

void Delay(int waitTicks);

#define LED_PERIOD 500000

#define VD6 PORT_Pin_7
#define VD7 PORT_Pin_8
#define VD8 PORT_Pin_9
#define VD9 PORT_Pin_10
#define VD10 PORT_Pin_11
#define VD11 PORT_Pin_12
#define VD12 PORT_Pin_13
#define VD13 PORT_Pin_14

#define LED_ALL VD6 | VD7 | VD8 | VD9 | VD10 | VD11 | VD12 | VD13

void AllLed (void* parameters)
{
	while(1)
	{
		vTaskDelay(100);
		PORT_SetBits(MDR_PORTD, LED_ALL);	// LED ON
		vTaskDelay(100);
		PORT_ResetBits(MDR_PORTD, LED_ALL); // LED OFF
	}
}


int main()
{
  PORT_InitTypeDef GPIOInitStruct;

  RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTD, ENABLE);
  PORT_StructInit(&GPIOInitStruct);

  GPIOInitStruct.PORT_Pin        = LED_ALL;
  GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
  GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
  GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;
  PORT_Init(MDR_PORTD, &GPIOInitStruct);

	xTaskCreate(AllLed, "ALL", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	vTaskStartScheduler();

  while (1)
  {

  }
}


void Delay(int waitTicks)
{
  int i;
  for (i = 0; i < waitTicks; i++)
  {
    __NOP();
  }
}
