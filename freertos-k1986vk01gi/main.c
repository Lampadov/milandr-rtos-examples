/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    21/12/2022
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2022 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986VK01_port.h"
#include "MDR1986VK01_timer.h"
#include "MDR1986VK01_clk.h"
#include "demoboard.h"
#include "leds.h"

#include "FreeRTOS.h"
#include "task.h"


#include <string.h>
#include <stdlib.h>

/** @addtogroup  __MDR1986VK01_StdPeriph_Examples MDR1986VK01 StdPeriph Examples
 *  @{
 */

/** @addtogroup __MDR1986VK01_EVALBOARD MDR1986VK01 Demo Board
 *  @{
 */

/** @addtogroup freertos_port freertos_port
 *  @{
 */

#define ANABG_IREFEN                (1 << 0)
#define ANABG_BFEN(x)               ((x & 0x3F) << 1)
#define ANABG_BGEN                  (1 << 7)
#define ANABG_BFEXT(x)              ((x & 0xF) << 8)
#define ANABG_SWMODE(x)             ((x & 0x3F) << 12)
#define ANABG_EXTMODE               (1 << 18)
#define ANABG_IRECMODE              (1 << 19)
#define ANABG_VRECMODE              (1 << 20)
#define ANABG_ZMODE                 (1 << 21)

void RedLed (void* parameters) {
	while(1)
	{
		vTaskDelay(200);
		Led_Set(LED_RED);
		vTaskDelay(200);
		Led_Reset(LED_RED);
	}
}

void GreenLed (void* parameters) {
	while(1)
	{
		vTaskDelay(100);
		Led_Set(LED_GREEN);
		vTaskDelay(100);
		Led_Reset(LED_GREEN);
	}
}

void BlueLed (void* parameters) {
	while(1)
	{
		vTaskDelay(200);
		Led_Set(LED_BLUE);
		vTaskDelay(200);
		Led_Reset(LED_BLUE);
	}
}

void ClockInit()
{
    MDR_CLK->KEY = 0x8555AAA1;
    MDR_CLK->PER0_CLK =  0xFFFFFFFF;
    MDR_CLK->PER1_CLK =  0xFFFFFFFF;

    MDR_COMP0->ANABG_CTRL = ANABG_IREFEN | ANABG_BGEN |
                      ANABG_BFEN( 0x3F ) | ANABG_SWMODE( 0x3F );


    MDR_BKP->KEY = 0x8555AAA1;

    CLK_XSEConfig( CLK_HSE0, CLK_XSE_ON );
    if( !CLK_XSEWaitReady( CLK_HSE0 ) )
        while(1){}

    CLK_SetSystemClock( MAX_CLK_HSE0 );


    CLK_PllConfig( &MDR_CLK->PLL[2], PLL_CLK_HSE0, 10, 1, 0 );

    CLK_PllState( &MDR_CLK->PLL[2], ENABLE );
    if( !CLK_PLLWaitReady( &MDR_CLK->PLL[2] ) )
        while(1){}

    CLK_SetSystemClock( MAX_CLK_PLL2 );
}

static void vShifterInit( void )
{
    PORT_InitTypeDef ShifterPort;
    PORT_StructInit( &ShifterPort );

    MDR_PORTA->KEY = 0x8555AAA1;

    ShifterPort.PORT_Pin     = PORT_Pin_31;
    ShifterPort.PORT_SOE     = PORT_SOE_OUT;
    ShifterPort.PORT_SFUNC   = PORT_SFUNC_PORT;
    ShifterPort.PORT_SANALOG = PORT_SANALOG_DIGITAL;
    ShifterPort.PORT_SPWR    = PORT_SPWR_10;

    PORT_Init( MDR_PORTA, &ShifterPort );

    BUF_OE_OFF;
}

int main()
{
    ClockInit();

    vShifterInit();

    LedInit();

    xTaskCreate(RedLed, "RED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(GreenLed, "GREEN", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(BlueLed, "BLUE", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

		vTaskStartScheduler();

    while(1)
    {

    }

}

/** @} */ /* End of group freertos_port */

/** @} */ /* End of group __MDR1986VK01_EVALBOARD */

/** @} */ /* End of group __MDR1986VK01_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2022 Milandr *********/

/* END OF FILE main.c */
