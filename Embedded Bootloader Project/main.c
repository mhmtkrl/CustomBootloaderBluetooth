#include "flash_config.h"
#include "program_memory.h"
#include "gpio_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"
#include "cmsis_armcc.h"


#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_ETX 0x03
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_SUB 0x1A

__packed typedef struct {
	uint8_t pakcet_SOH;
	uint8_t packetNumber;
	uint8_t pakcetNumberComp;
	uint32_t packetData[32];
	uint16_t packetCRC;
}BootPacket;

BootPacket bootPacket[10];
uint8_t packetPosition = 0;
int8_t fileTransferComplete = 0;

int8_t pos = 0;
int16_t j = 0;

uint16_t base =  0x100;
uint32_t adr = 0;
uint8_t byte0, byte1, byte2, byte3;



void (*JumpToApplicationCode)(void);

void USART3_IRQHandler(void) {
	//If it is a RX ISR
	if(*USART3.SR & (1ul << 5)) {
		receivedBluetoothPacket[receivedBluetoothIndex] = *USART3.DR;
		((uint8_t *)(&bootPacket[packetPosition]))[receivedBluetoothIndex] = (uint8_t)receivedBluetoothPacket[receivedBluetoothIndex];
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
void SysTick_Handler(void) {
	sysTickCount++;
}
void jumpFunction(uint32_t codeAddress) {
	JumpToApplicationCode = (void (*)(void)) (*((uint32_t *)(codeAddress + 4)));
	__disable_irq();

	*sysTickCSR = 0;
	*(uint32_t*)0xE000ED08 = codeAddress;
	__set_MSP(*(uint32_t*)codeAddress);
	UARTDebugSend("\r\n---->Jumping to the Application Code!\r\n");
	UARTBluetoothSend("\r\n---->Jumping to the Application Code!\r\n");
	JumpToApplicationCode();
}

int main() {
 	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUserLED();
	InitUARTforDebug();	
	InitUARTforBluetooth();
	
	UARTBluetoothSend(bluetoothWelcomeMessage);
	welcomeMessage();
	
	program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS);
	
	*USART3.DR = XMODEM_NAK;
	
	
	while(1) {
		if(fileTransferComplete) {
			UARTDebugSend("\r\nA new file has been received!\r\n");
			OnUserLED();
		
			sprintf(msg, "+++SECTOR %d is erased!\n", (((uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS) & 0x000FF000) >> 12);
			UARTDebugSend(msg);
			sprintf(msg, "+++Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
			UARTDebugSend(msg);
			/////////////////////////////////////////////
			
			for(pos = 0 ; pos < packetPosition ; pos++) {
				sprintf(msg, "\r\nWriting to the allocated memory area...\n");
				UARTDebugSend(msg);
				UARTBluetoothSend(msg);
				if(adr % 512) {
					program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS + base);
					base += 0x100;
				}
				for(j = 0 ; j < 32 ; j++) {
					byte3 = (bootPacket[pos].packetData[j]&0xFF000000) >> 24;
					byte2 = (bootPacket[pos].packetData[j]&0x00FF0000) >> 16;
					byte1 = (bootPacket[pos].packetData[j]&0x0000FF00) >> 8;
					byte0 = (bootPacket[pos].packetData[j]&0x000000FF) >> 0;
					program_Memory_Fast_Word_Write((APPLICATION_FIRMWARE_BASE_ADDRESS + adr), bootPacket[pos].packetData[j]);
					//delayMS(30);
					sprintf(msg, "-->@0x%X = 0x%X\n", (APPLICATION_FIRMWARE_BASE_ADDRESS + adr), bootPacket[pos].packetData[j]);
					//sprintf(msg, "0x%X, 0x%X, 0x%X, 0x%X => %c%c%c%c, 0x%X\n", bootPacket[pos].pakcet_SOH, bootPacket[pos].packetNumber, bootPacket[pos].pakcetNumberComp, bootPacket[pos].packetData[j], byte0, byte1, byte2, byte3, bootPacket[pos].packetCRC);
					UARTDebugSend(msg);
					UARTBluetoothSend(msg);
					adr += 4;
				}
		  }
			sprintf(msg, "\n:REPORT: %d bytes memory has been used!\n", adr-4);
			UARTDebugSend(msg);
			UARTBluetoothSend(msg);
			UARTDebugSend("\r\nEnd of the bootloader process!\r\n");
			UARTBluetoothSend("\r\nEnd of the bootloader process!\r\n");
			OffUserLED();
			fileTransferComplete = 0;
			*FLASH.PECR  |= 1ul << 0;
			*FLASH.PECR |= 1ul << 1;
			jumpFunction(APPLICATION_FIRMWARE_BASE_ADDRESS);
		}
	}
}

void welcomeMessage(void) {
	//Welcome Message
	UARTDebugSend(debugWelcomeMessage);
	//-UARTBluetoothSend(bluetoothWelcomeMessage);
	sprintf(msg, "--Bootloader Size               : %d kB\n", (uint8_t)BOOTLOADER_SIZE_KB);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : SECTOR %d\n", (((uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS) & 0x000FF000) >> 12);
	UARTDebugSend(msg);
	sprintf(msg, "--Application Code Base Address : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
	sprintf(msg, "--Current Data @ 0x%X      : 0x%X\n", (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS, *(uint32_t*)APPLICATION_FIRMWARE_BASE_ADDRESS);
	UARTDebugSend(msg);
}
