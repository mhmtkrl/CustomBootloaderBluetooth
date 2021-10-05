#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

uint16_t pckCnt = 0;
char msg[64];

int main() {
 	initSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	
	while(1) {
		sprintf(msg, "Packet Number = %d\n", pckCnt);
		UARTDebugSend(msg);
		pckCnt++;

		OnUserLED();
		delayMS(100);
		OffUserLED();
		delayMS(100);
	}
}
