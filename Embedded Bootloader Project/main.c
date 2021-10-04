#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"


int main() {
	initSysTickTimerInMiliseconds(10, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	
	while(1) {
		OnUserLED();
		delayMS(1000);
		OffUserLED();
		delayMS(1000);
	}
}



