#include "stm32l1xx.h"                  // Device header

#define USART2_IRQ_NUMBER			38
#define USART3_IRQ_NUMBER			39

void InitUARTforBluetooth(void);
void UARTBluetoothSend(char packet[]);
void InitUARTforDebug(void) ;
void UARTDebugSend(char packet[]) ;	

