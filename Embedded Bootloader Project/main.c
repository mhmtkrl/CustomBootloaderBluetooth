#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

char msg[64];

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	//ERASE and WRITE a WORD @APPLICATION_FIRMWARE_BASE_ADDRESS
	program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS);
	program_Memory_Fast_Word_Write(APPLICATION_FIRMWARE_BASE_ADDRESS, 0xAABBCCDD);
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	sprintf(msg, "--Bootloader Size               : %d kB\n", (uint8_t)BOOTLOADER_SIZE_KB);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
	sprintf(msg, "--Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
	
	UARTBluetoothSend(bluetoothWelcomeMessage);
	
	while(1) {
		
	}
}
