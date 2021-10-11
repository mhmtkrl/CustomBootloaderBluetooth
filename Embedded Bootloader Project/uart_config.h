/*
This is for UART register address
*/

#include <stdint.h>
#include "stdio.h"
#include "stm32l1xx.h"                  // Device header

static char msg[64];

static char receivedDebugPacket[255];
static char receivedDebugPacketToDebug[255];
static int16_t receivedDebugIndex = 0;
static int16_t clearDebugPacket = 0;
static char debugWelcomeMessage[] = "\r\n*****************BOOT MODE!*****************\n";

static char receivedBluetoothPacket[255];
static char receivedBluetoothPacketToDebug[255];
static int16_t receivedBluetoothIndex = 0;
static int16_t clearBluetoothPacket = 0;
static char bluetoothWelcomeMessage[] = "\r\n*****************BOOT MODE!*****************\n";

void welcomeMessage(void);
void InitUARTforBluetooth(void);
void UARTBluetoothSend(char packet[]);
void InitUARTforDebug(void);
void UARTDebugSend(char packet[]);
