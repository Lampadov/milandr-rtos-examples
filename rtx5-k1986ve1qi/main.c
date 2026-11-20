#include <rtx_os.h>
#include <os_tick.h>
#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>

#define USEPORT MDR_PORTD
#define CLK_USEPORT RST_CLK_PCLK_PORTD
// Структура настроек для потока
 typedef struct {
   uint32_t Led_PortPin;
   uint32_t Led_Delay;
} LED_ThreadCfg;

// Задержка мигания светодиодов в миллисекундах
#define Delay 500


// Функция потока - исполняется в отдельном потоке
void Thread_LED (void *argument)
{
	// Структура для настройки портов
	PORT_InitTypeDef GPIOInitStruct;
	// Параметры для функции, задаются извне
	LED_ThreadCfg cfgThread = *(LED_ThreadCfg *)(argument);

	// Включение тактирования
	RST_CLK_PCLKcmd (CLK_USEPORT, ENABLE);

	// Настройка вывода порта
	PORT_StructInit(&GPIOInitStruct);
	GPIOInitStruct.PORT_Pin = cfgThread.Led_PortPin;
	GPIOInitStruct.PORT_OE = PORT_OE_OUT;
	GPIOInitStruct.PORT_SPEED = PORT_SPEED_MAXFAST;
	GPIOInitStruct.PORT_MODE = PORT_MODE_DIGITAL;

	PORT_Init(USEPORT, &GPIOInitStruct);

	// Цикл исполнения - мигание светодиодом
while (1)
 {
  PORT_SetBits(USEPORT, cfgThread.Led_PortPin);
  osDelay(cfgThread.Led_Delay);

  PORT_ResetBits(USEPORT, cfgThread.Led_PortPin);
  osDelay(cfgThread.Led_Delay);
 }
}

int main(void)
{

	// Настройки для потоков
	LED_ThreadCfg Cfg_Thread1 = {PORT_Pin_7, Delay};

	// Инициализация RTOS
	osKernelInitialize();

	// Создание потоков мигания светодиодом
	osThreadNew (Thread_LED, &Cfg_Thread1, NULL);
	// Запуск RTOS
	osKernelStart();
}
