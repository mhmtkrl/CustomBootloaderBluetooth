#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_ETX 0x03
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_SUB 0x1A

typedef struct {
	uint8_t pakcet_SOH;
	uint8_t packetNumber;
	uint8_t pakcetNumberComp;
	unsigned char packetData[128];
	uint16_t packetCRC;
}BootPacket;

BootPacket bootPacket[10];
uint8_t packetPosition = 0;
int8_t fileTransferComplete = 0;

int8_t pos = 0;
int16_t j = 0;

void USART3_IRQHandler(void) {
	//If it is a RX ISR
	if(*USART3.SR & (1ul << 5)) {
		receivedBluetoothPacket[receivedBluetoothIndex] = *USART3.DR;
		((uint8_t *)(&bootPacket[packetPosition]))[receivedBluetoothIndex] = receivedBluetoothPacket[receivedBluetoothIndex];
		receivedBluetoothIndex++;	
			
		if(bootPacket[packetPosition].pakcet_SOH == XMODEM_EOT) {
			*USART3.DR = XMODEM_ACK;
			fileTransferComplete = 1;
		}
		if(receivedBluetoothIndex == 132) {
			*USART3.DR = XMODEM_ACK;
			packetPosition++;
			receivedBluetoothIndex = 0;
		}
	}
	*USART3.SR &= ~(1ul << 5);	//The RXNE flag can also be cleared by writing a zero to it
}

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	//ERASE and WRITE a WORD @APPLICATION_FIRMWARE_BASE_ADDRESS
	program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS);
	program_Memory_Fast_Word_Write(APPLICATION_FIRMWARE_BASE_ADDRESS, 0xAABBCCDD);
	
	welcomeMessage();
	
	
	*USART3.DR = XMODEM_NAK;
	
	while(1) {
		if(fileTransferComplete) {
			UARTDebugSend("file transfer is done\r\n");
			OnUserLED();
			UARTDebugSend("\r\nByte by Byte\r\n");
			for(pos = 0 ; pos < packetPosition ; pos++) {
				sprintf(msg, "Received Packet[%d]: ", pos);
				UARTDebugSend(msg);
				for(j = 0 ; j < 128 ; j++) {
					sprintf(msg, "%c", bootPacket[pos].packetData[j]);
					UARTDebugSend(msg);
				}
				UARTDebugSend("\r\n");
			}
			UARTDebugSend("\r\nEnd of bootloader process\r\n");
			OffUserLED();
			fileTransferComplete = 0;
		}
	}
}

void welcomeMessage(void) {
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	//UARTBluetoothSend(bluetoothWelcomeMessage);
	sprintf(msg, "--Bootloader Size               : %d kB\n", (uint8_t)BOOTLOADER_SIZE_KB);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : SECTOR %d\n", (((uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS) & 0x000FF000) >> 12);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
	sprintf(msg, "--Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
}
