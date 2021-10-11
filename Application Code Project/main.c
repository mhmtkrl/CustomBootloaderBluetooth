#include "stm32l1xx.h"                  // Device header
#include "sysTick_timer_config.h"
#include "uart_config.h"
#include "gpio_config.h"
#include "stdio.h"

#define APP_MODE 							1

#if APP_MODE == 0
	#define LED_ON_TIME		1000
	#define LED_OFF_TIME 	1000
#endif

#if APP_MODE == 1
	#define LED_ON_TIME		50
	#define LED_OFF_TIME 	500
#endif

char buf[128];
uint8_t rawData1 = 0, rawData2 = 255;

int main() {
	__enable_irq();
	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUARTforDebug();	
	InitUARTforBluetooth();
	InitUserLED();

	UARTBluetoothSend("\r\n********--The firmware update has been done successfully!--********\n");
	
	while(1) {
		sprintf(buf, "---Raw Data1 = %d\n", rawData1);
		UARTBluetoothSend(buf);
		sprintf(buf, "---Raw Data2 = %d\n", rawData2);
		UARTBluetoothSend(buf);
		rawData1++;
		rawData2--;
		
		OnUserLED();
		delayMS(LED_ON_TIME);
		OffUserLED();
		delayMS(LED_OFF_TIME);
	}	
}
