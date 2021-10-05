#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	//ERASE and WRITE a WORD @APPLICATION_FIRMWARE_BASE_ADDRESS
	program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS);
	program_Memory_Fast_Word_Write(APPLICATION_FIRMWARE_BASE_ADDRESS, 0xAABBCCDD);
	
	welcomeMessage();
	
	while(1) {
		
	}
}

void welcomeMessage(void) {
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	UARTBluetoothSend(bluetoothWelcomeMessage);
	sprintf(msg, "--Bootloader Size               : %d kB\n", (uint8_t)BOOTLOADER_SIZE_KB);
	UARTBluetoothSend(msg);
	sprintf(msg, "--Application Code Base Address : SECTOR %d\n", (((uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS) & 0x000FF000) >> 12);
	UARTBluetoothSend(msg);
	sprintf(msg, "--Application Code Base Address : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTBluetoothSend(msg);
	sprintf(msg, "--Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTBluetoothSend(msg);
}
