#include "sysTick_timer_config.h"

void InitSysTickTimerInMiliseconds(uint16_t delayMS, uint32_t clockFreq) {
	uint32_t reloadValue = 0;
	//Clock source -> Core Clock
	*sysTickCSR |= 1ul << 2;
	//SysTick Handler is enabled
	*sysTickCSR |= 1ul << 1;
	//Calculate RELOAD Value 
	reloadValue = (delayMS * (clockFreq / 1000)) - 1;
	*sysTickRVR = reloadValue;
	//SysTick Enable
	//*sysTickCSR |= 1ul << 0;
}

void delayMS(int16_t del) {
	//SysTick Enable
	*sysTickCSR |= 1ul << 0;
	while(sysTickCount <= del);
	//SysTick Disable
	*sysTickCSR &= ~(1ul << 0);
	sysTickCount = 0;
}

void SysTick_Handler(void) {
	sysTickCount++;
}


