#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

uint16_t pckCnt = 0;
char msg[64];

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	UARTBluetoothSend(bluetoothWelcomeMessage);
	
	while(1) {
		
	}
}
