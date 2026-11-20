# FreeRTOS и RTX5 на МК Миландр

Начнём с мигания светодиодов. По нему удобно проверить, запустился ли планировщик и срабатывают ли задержки. В двух проектах это делает FreeRTOS. В третьем используем RTX5 и проверяем, как привязать системный тик к аппаратному таймеру.

| Пример | Целевая плата | Что произойдёт после запуска |
| --- | --- | --- |
| [FreeRTOS на К1986ВЕ1QI](freertos-k1986ve1qi/) | К1986ВЕ1QI | Одна задача переключает светодиоды с задержкой 100 тиков |
| [FreeRTOS на К1986ВК01GI](freertos-k1986vk01gi/) | К1986ВК01GI | Три задачи независимо переключают красный, зелёный и синий каналы |
| [RTX5 с OS Tick API](rtx5-k1986ve1qi/) | К1986ВЕ1QI | Светодиод переключается каждые 500 тиков, источник тика — TIMER1 |

## FreeRTOS

Оба проекта используют ядро **FreeRTOS 10.5.1** и **Arm Compiler 5.06**. У них разные файлы порта и настройки в `FreeRTOSConfig.h`. Например, на К1986ВК01GI используется порт Cortex-M4F. В статье разобран выбор порта и для К1986ВЕ1QI. [Статья о портировании][freertos].

- Для **К1986ВЕ1QI** откройте [FreeRTOS_1QI.uvprojx](freertos-k1986ve1qi/FreeRTOS_1QI.uvprojx). Нужен пакет `Milandr.MDR32FxQI.1.0`. В **Manage Run-Time Environment** проверьте Startup, PORT и RST_CLK.
- Для **К1986ВК01GI** откройте [FreeRTOS_VK01.uvprojx](freertos-k1986vk01gi/FreeRTOS_VK01.uvprojx). Используются `Keil.MDR1986VK01.1.3` и CMSIS Core 5.5.0. Проверьте Startup, DEMOBOARD, Leds, CLK, PORT и TIMER. Выберите цель **FLASH** или **RAM**.

Возьмите `RTE/Device/` из архива выбранного проекта и скопируйте в его каталог. Для К1986ВК01GI потребуется ещё `RTE/Board_Support/`. [Оба архива][freertos].

## RTX5 с тиком от TIMER1

Откройте [HelloWorld.uvprojx](rtx5-k1986ve1qi/HelloWorld.uvprojx) с **Arm Compiler 5.06** и `Milandr.MDR32FxQI.1.0`. В **Manage Run-Time Environment** восстановите Startup, PORT, RST_CLK и TIMER.

| Что взять | Куда положить |
| --- | --- |
| `RTX_CM0.lib` из архива `RTOS2_OSTICKAPI_K1986BE1QI.zip` | `rtx5-k1986ve1qi/RTX_CM0.lib` |
| `CMSIS/` из пакета `ARM.CMSIS.5.4.0` | `rtx5-k1986ve1qi/cmsis-5.4.0/CMSIS/` |

В сгенерированном startup-файле направьте вектор **TIMER1** на `SysTick_Handler`. В [api_os_tick.c](rtx5-k1986ve1qi/api_os_tick.c) TIMER1 формирует тик с частотой **1 кГц**. В `OS_Tick_AcknowledgeIRQ()` сбрасывается флаг прерывания таймера. Прошивка переключает светодиод через `osDelay(500)`, поэтому одну смену состояния ожидаем примерно раз в полсекунды. [Настройка вектора и архив RTX5][rtx].

При переносе примера в репозиторий поправлен инкремент предделителя при расчёте ARR. Три функции мониторинга тика оставлены заглушками, как в статье.

Автор статей — **Илья Лампадов**. Ядро FreeRTOS распространяется по [MIT](LICENSES/FreeRTOS-MIT.txt). К файлам конфигурации RTX с уведомлением Arm относится [Apache-2.0](LICENSES/Apache-2.0.txt).

[freertos]: https://support.milandr.ru/base/primenenie/programmirovanie-32-razryadnykh-mk/osobennosti-primeneniya-know-how/49937/
[rtx]: https://support.milandr.ru/base/primenenie/programmirovanie-32-razryadnykh-mk/osobennosti-primeneniya-know-how/24210/
