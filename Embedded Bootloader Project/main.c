#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

uint8_t msg[] = "Hello World!\n";

int main() {
 	initSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	
	while(1) {
		UARTDebugSend(msg);
		OnUserLED();
		delayMS(100);
		OffUserLED();
		delayMS(500);
	}
}
