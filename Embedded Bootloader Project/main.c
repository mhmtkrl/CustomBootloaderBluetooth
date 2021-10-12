/*
Author: Mehmet KORAL
Date  : 11.10.2021
Title : FUOTB -> Firmware Update Over The Bluetooth
	Bluetooth Module is HC05
	PB10 -> USART3_TX	  ->  HC05_RX
	PB11 -> USART3_RX		->  HC05_TX
	
	USB cable can be used for debugging!
	USART2 connects to the ST-LINK!
	
	When you press the reset button, the bootloader code will be launched.
*/

#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"
#include "xmodem_protocol.h"

uint8_t packetPosition = 0;
int8_t fileTransferComplete = 0;
int8_t pos = 0;
int16_t j = 0;
uint32_t adr = 0;

void USART3_IRQHandler(void) {
	//If it is a RX ISR
	if(USART3->SR & (1ul << 5)) {
		receivedBluetoothPacket[receivedBluetoothIndex] = USART3->DR;
		((uint8_t *)(&bootPacket[packetPosition]))[receivedBluetoothIndex] = (uint8_t)receivedBluetoothPacket[receivedBluetoothIndex];
		receivedBluetoothIndex++;	
			
		if(bootPacket[packetPosition].pakcet_SOH == XMODEM_EOT) {
			fileTransferComplete = 1;
			USART3->DR = XMODEM_ACK;
		}
		else {
			if(receivedBluetoothIndex == 132) {
				packetPosition++;
				receivedBluetoothIndex = 0;
				USART3->DR = XMODEM_ACK;
			}
		}
	}
	USART3->SR &= ~(1ul << 5);	//The RXNE flag can also be cleared by writing a zero to it
}

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	
	welcomeMessage();
	
	USART3->DR = XMODEM_NAK;	
	
	while(1) {
		if(fileTransferComplete) {
			UARTDebugSend("\r\nA new file has been received!\r\n");
			OnUserLED();

			for(pos = 0 ; pos < packetPosition ; pos++) {
				sprintf(msg, "\r\nWriting to the allocated memory area...\n");
				UARTDebugSend(msg);
				UARTBluetoothSend(msg);
				if(adr % 256 == 0) {
					program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS + VECTOR_BASE);
					VECTOR_BASE += 0x100;
				}
				locking_Program_Memory();
				for(j = 0 ; j < 32 ; j++) {
					program_Memory_Fast_Word_Write((APPLICATION_FIRMWARE_BASE_ADDRESS + adr), bootPacket[pos].packetData[j]);
					sprintf(msg, "-->@0x%X = 0x%X\n", (APPLICATION_FIRMWARE_BASE_ADDRESS + adr), bootPacket[pos].packetData[j]);
					UARTDebugSend(msg);
					UARTBluetoothSend(msg);
					adr += 4;
				}
				locking_Program_Memory();
		  }
			sprintf(msg, "\n:REPORT: %d bytes memory has been used!\n", adr-4);
			UARTBluetoothSend(msg);
			UARTBluetoothSend("\r\nEnd of the bootloader process!\r\n");
			OffUserLED();
			fileTransferComplete = 0;
			locking_Program_Memory();
			BootJump( ( uint32_t * )APPLICATION_FIRMWARE_BASE_ADDRESS ) ;
		}
	}
}

void welcomeMessage(void) {
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	UARTBluetoothSend(bluetoothWelcomeMessage);
	sprintf(msg, "--Bootloader Size               : %d kB\n", (uint8_t)BOOTLOADER_SIZE_KB);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : SECTOR %d\n", (((uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS) & 0x000FF000) >> 12);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
	sprintf(msg, "--Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
}
