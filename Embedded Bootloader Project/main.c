#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"

int8_t pinStatus = 0;

int main() {
	initSysTickTimerInMiliseconds(1000, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	
	while(1) {
		
	}
}

void SysTick_Handler(void) {
	pinStatus++;
	if(pinStatus == 1) {
		OnUserLED();
	}
	if(pinStatus == 2) {
		OffUserLED();
		pinStatus = 0;
	}
}
