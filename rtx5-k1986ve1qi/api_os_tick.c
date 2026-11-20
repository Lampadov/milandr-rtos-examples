/*******************************************************************************
 * @File: api_os_tick.c
 * @Author: Milandr, Lampadov
 * @Date: 02.12.2020
 *******************************************************************************/

// Подключение заголовочных файлов
#include <rtx_os.h>
#include <os_tick.h>
#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>

// Задание параметров под конкретный таймер
#define TIMER MDR_TIMER1
#define INT_TIMER TIMER1_IRQn
#define CLK_TIMER RST_CLK_PCLK_TIMER1

int32_t  OS_Tick_Setup (uint32_t freq, IRQHandler_t handler) {

	uint32_t load;
	uint32_t clktim;
	uint32_t divtim = TIMER->PSG;

	RST_CLK_PCLKcmd (CLK_TIMER, ENABLE); // Включение тактирования блока таймера 1

	// ВАЖНО! Дополнительная настройка тактирования для таймеров. Может выполняться для некоторых таймеров в регистре UART_CLK (см. спецификацию)
	MDR_RST_CLK->TIM_CLOCK = (1<<24); //Включение тактовой частоты таймера 1, TIM1_CLK == HCLK

  // Настраиваем работу основного счетчика
	TIMER->CNTRL = 0x00000000;//Режим инициализации таймера
  TIMER->CNT = 0x00000000;//Начальное значение счетчика
  TIMER->PSG = 0x00000000;//Предделитель частоты
  TIMER->IE  = 0x00000002;//Разрешение генерировать прерывание при CNT = ARR

	SystemCoreClockUpdate(); // Обновление системной частоты
	clktim = (SystemCoreClock/(divtim + 1)); // Подсчёт системной частоты таймера с учётом бита деления PSG

	load = (clktim/ freq) - 1U; // Расчёт значения для основания счёта

	while (load > 0xFFFF) // Пока полученное значение ARR больше разрядности таймера, инкрементируем значение переменной divtim (PSG), чтобы попасть в диапазон
	{
		divtim++;
		clktim = (SystemCoreClock/(divtim + 1));
		load = (clktim / freq) - 1U;
	}

	NVIC_EnableIRQ(INT_TIMER);
  NVIC_SetPriority(INT_TIMER, 192);
	TIMER->PSG = divtim;
  TIMER->ARR = load;
  return (0);
}

void  OS_Tick_Enable (void) {
   TIMER->CNTRL = 0x00000001; //Счет вверх по TIM_CLK
}

int32_t  OS_Tick_GetIRQn (void) {
  return (INT_TIMER); // Какое прерывание получать в ОСРВ
}

void  OS_Tick_AcknowledgeIRQ (void) {
	TIMER->STATUS = 0x00; // Фактически обработчик прерываний. Сброс статуса вызова прерывания
}

uint32_t OS_Tick_GetClock (void) {
  return (SystemCoreClock); // Мониторинг тактовой частоты процессора
}

uint32_t OS_Tick_GetInterval (void) {
}

uint32_t OS_Tick_GetCount (void) {
}


uint32_t OS_Tick_GetOverflow (void) {
}
