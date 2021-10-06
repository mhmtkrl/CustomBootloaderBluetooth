#include "stm32l1xx.h"                  // Device header
#include "sysTick_timer_config.h"
#include "uart_config.h"
#include "gpio_config.h"
#include "stdio.h"


#define APP_MODE 							1
char buf[64];
int c = 0, a = 10;




#if APP_MODE == 0
	#define LED_ON_TIME		1000
	#define LED_OFF_TIME 	1000
#endif

#if APP_MODE == 1
	#define LED_ON_TIME		50
	#define LED_OFF_TIME 	500
#endif



int main() {
	__enable_irq();
	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUARTforDebug();	
	InitUARTforBluetooth();
	InitUserLED();
	
	UARTDebugSend("\r\n***************--The firmware update has been done successfully!--***************\n");
	UARTBluetoothSend("\r\n***************--The firmware update has been done successfully!--***************\n");
	
	while(1) {
		sprintf(buf, "Application Code::: %d\n", c*6);
		c++;
		UARTBluetoothSend(buf);
		
		sprintf(buf, "ADC: %d\n", a*2);
		a++;
		UARTBluetoothSend(buf);
		
		OnUserLED();
		delayMS(LED_ON_TIME);
		OffUserLED();
		delayMS(LED_OFF_TIME);
	}	
}
