/*
This is for UART register address
*/

#include <stdint.h>
#include "stdio.h"
#include "NVIC_controller.h"

//Register Boundary Address Table 5 page 47 RM0038
#define usart2_base						0x40004400
#define usart3_base						0x40004800
//USART Register Map Table 158 page 743 RM0038
#define usart_sr_offset 		0x00
#define usart_dr_offset 		0x04
#define usart_brr_offset		0x08
#define usart_cr1_offset		0x0C 
#define usart_cr2_offset		0x10
#define usart_cr3_offset		0x14 
#define usart_gtpr_offset		0x18

//USART Registers
typedef struct {
	uint32_t *SR;				//Program/Erase Control Register
	uint32_t *DR;			//Program/erase key register
	uint32_t *BRR;		//Program memory key register
	uint32_t *CR1;		//Option byte key register
	uint32_t *CR2;					//Status register
	uint32_t *CR3;		//Option byte key register
	uint32_t *GTPR;					//Status register
}USARTRegister;

static USARTRegister USART2 = {
	(uint32_t*)(usart2_base + usart_sr_offset), 
	(uint32_t*)(usart2_base + usart_dr_offset),
	(uint32_t*)(usart2_base + usart_brr_offset),
	(uint32_t*)(usart2_base + usart_cr1_offset),
	(uint32_t*)(usart2_base + usart_cr2_offset),
	(uint32_t*)(usart2_base + usart_cr3_offset),
	(uint32_t*)(usart2_base + usart_gtpr_offset)
};

static USARTRegister USART3 = {
	(uint32_t*)(usart3_base + usart_sr_offset), 
	(uint32_t*)(usart3_base + usart_dr_offset),
	(uint32_t*)(usart3_base + usart_brr_offset),
	(uint32_t*)(usart3_base + usart_cr1_offset),
	(uint32_t*)(usart3_base + usart_cr2_offset),
	(uint32_t*)(usart3_base + usart_cr3_offset),
	(uint32_t*)(usart3_base + usart_gtpr_offset)
};

static char receivedDebugPacket[64];
static char receivedDebugPacketToDebug[64];
static int16_t receivedDebugIndex = 0;
static int16_t clearDebugPacket = 0;
static char debugWelcomeMessage[] = "\r\n**********Debug Welcome Screen**********\r";

static char receivedBluetoothPacket[64];
static char receivedBluetoothPacketToDebug[64];
static int16_t receivedBluetoothIndex = 0;
static int16_t clearBluetoothPacket = 0;
static char bluetoothWelcomeMessage[] = "\r\n**********Bluetooth Welcome Screen**********\r";

void InitUARTforBluetooth(void);
void UARTBluetoothSend(char packet[]);
void InitUARTforDebug(void);
void UARTDebugSend(char packet[]);
