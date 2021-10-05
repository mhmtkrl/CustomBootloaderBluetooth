#include "uart_config.h"
#include "clock_config.h"
#include "gpio_config.h"

/*
This function for USART2 that is used for debugging
The USART2 interface available on PA2 and PA3 of the STM32 microcontroller can be connected to ST-LINK MCU.
Page 25. UM1724

Table 21: page 51 UM1724
PA2 -> USART2_TX
PA3 -> UART2_RX

Table 9: Alternate Function Input/Output Page 47 DocID025433
PA2 -> USART2_TX	-> AFIO7
PA3 -> UART2_RX		-> AFIO7

Procedure:
1.Enable the USART by writing the UE bit in USART_CR1 register to 1.
2. Program the M bit in USART_CR1 to define the word length.
3. Program the number of stop bits in USART_CR2.
4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take place. 
Configure the DMA register as explained in multibuffer communication.
5. Select the desired baud rate using the USART_BRR register.
6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
7. Write the data to send in the USART_DR register (this clears the TXE bit). 
Repeat this for each data to be transmitted in case of single buffer.
8. After writing the last data into the USART_DR register, wait until TC=1. This indicates that the transmission of the last frame is complete. 
This is required for instance when the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
*/
void InitUARTforDebug(void) {
	//AHB is enabled for GPIOA
	*RCC.AHBENR |= 1ul << 0;
	//PA2 is alternate function mode
	*GPIOA.MODER &= ~(1ul << 4);
	*GPIOA.MODER |= (1ul << 5);
	//PA3 is alternate function mode
	*GPIOA.MODER &= ~(1ul << 6);
	*GPIOA.MODER |= (1ul << 7);
	//AFIO7 for PA2
	*GPIOA.AFRL |= (7ul << 8);
	//AFIO7 for PA3
	*GPIOA.AFRL |= (7ul << 12);
	
	//USART2 clock enable
	*RCC.APB1ENR |= (1ul << 17);
	//Enable USART2
	*USART2.CR1 |= (1ul << 13);
	//1 Start bit, 8 Data bits, n Stop bit
	*USART2.CR1 &= ~(1ul << 12);
	//1 Stop bit
	*USART2.CR2 &= ~(1ul << 12);
	*USART2.CR2 &= ~(1ul << 13);
	//oversampling by 16
	*USART2.CR1 &= ~(1ul << 15);
	//BRR
	*USART2.BRR = 0x00000012;
	//Transmitter enable
	*USART2.CR1 |= (1ul << 3);
}

void UARTDebugSend(uint8_t packet[]) {
	int16_t i = 0;
	while(packet[i] != '\0') {
		*USART2.DR = packet[i];
		while(!(*USART2.SR & 0x00000040));
		i++;
	}
}














