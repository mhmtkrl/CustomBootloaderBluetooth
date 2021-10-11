#include "uart_config.h"
#include "clock_config.h"
#include "gpio_config.h"
#include "program_memory.h"
#include "flash_config.h"

/*
This function for USART3 that is used for communicating to the host device over Bluetooth

Table 9: Alternate Function Input/Output Page 47 DocID025433
PB10 -> USART3_TX	  -> AFIO7
PB11 -> USART3_RX		-> AFIO7
*/
void InitUARTforBluetooth(void) {
	//AHB is enabled for GPIOB
	RCC->AHBENR |= 1ul << 1;
	//AFIO7 for PB10
	GPIOB->AFR[1] |= (7ul << 8);
	//AFIO7 for PB11
	GPIOB->AFR[1] |= (7ul << 12);
	//PB10 is alternate function mode
	GPIOB->MODER &= ~(1ul << 20);
	GPIOB->MODER |= (1ul << 21);
	//PB11 is alternate function mode
	GPIOB->MODER &= ~(1ul << 22);
	GPIOB->MODER |= (1ul << 23);
	
	//USART3 clock enable
	RCC->APB1ENR |= (1ul << 18);
	//Enable USART3
	USART3->CR1 |= (1ul << 13);
	//1 Start bit, 8 Data bits, n Stop bit
	USART3->CR1 &= ~(1ul << 12);
	//1 Stop bit
	USART3->CR2 &= ~(1ul << 12);
	USART3->CR2 &= ~(1ul << 13);
	//oversampling by 16
	USART3->CR1 &= ~(1ul << 15);
	//BRR
	//OVER8 = 0;
	//USARTDIV = Fclk / (8*2*BaudRate)
	//USARTDIV = 2097000/16*115200 = 1.1376..
	//Mantissa -> 1
	//Fraction -> 0.1376 * 16 ~= 2;
	//USARTDIV = 0x12;
	USART3->BRR = 0x00000012;
	//RXNE interrupt enable
	USART3->CR1 |= (1ul << 5);
//	//Receiver enable
	USART3->CR1 |= (1ul << 2);
	NVIC_EnableIRQ(USART3_IRQn);
	//Transmitter enable
	USART3->CR1 |= (1ul << 3);
}

void UARTBluetoothSend(char packet[]) {
	int16_t i = 0;
	while(packet[i] != '\0') {
		USART3->DR = packet[i];
		//If a frame is transmitted (after the stop bit) and the TXE bit is set, the TC bit goes high.
		while(!(USART3->SR & (1ul << 6)));
		i++;
	}
}


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
	RCC->AHBENR |= 1ul << 0;
	//AFIO7 for PA2
	GPIOA->AFR[0] |= (7ul << 8);
	//AFIO7 for PA3
	GPIOA->AFR[0] |= (7ul << 12);
	//PA2 is alternate function mode
	GPIOA->MODER &= ~(1ul << 4);
	GPIOA->MODER |= (1ul << 5);
	//PA3 is alternate function mode
	GPIOA->MODER &= ~(1ul << 6);
	GPIOA->MODER |= (1ul << 7);
	
	//USART2 clock enable
	RCC->APB1ENR |= (1ul << 17);
	//Enable USART2
	USART2->CR1 |= (1ul << 13);
	//1 Start bit, 8 Data bits, n Stop bit
	USART2->CR1 &= ~(1ul << 12);
	//1 Stop bit
	USART2->CR2 &= ~(1ul << 12);
	USART2->CR2 &= ~(1ul << 13);
	//oversampling by 16
	USART2->CR1 &= ~(1ul << 15);
	//BRR
	//OVER8 = 0;
	//USARTDIV = Fclk / (8*2*BaudRate)
	//USARTDIV = 2097000/16*115200 = 1.1376..
	//Mantissa -> 1
	//Fraction -> 0.1376 * 16 ~= 2;
	//USARTDIV = 0x12;
	USART2->BRR = 0x00000012;
	//RXNE interrupt enable
	USART2->CR1 |= (1ul << 5);
	//Receiver enable
	USART2->CR1 |= (1ul << 2);
	NVIC_EnableIRQ(USART2_IRQn);
	//Transmitter enable
	USART2->CR1 |= (1ul << 3);
}

void UARTDebugSend(char packet[]) {
	int16_t i = 0;
	while(packet[i] != '\0') {
		USART2->DR = packet[i];
		//If a frame is transmitted (after the stop bit) and the TXE bit is set, the TC bit goes high.
		while(!(USART2->SR & (1ul << 6)));
		i++;
	}
}

void USART2_IRQHandler(void) {
	//If it is a RX ISR
	if(USART2->SR & (1ul << 5)) {
		if(USART2->DR != '\n') {		//When press enter 
			receivedDebugPacket[receivedDebugIndex] = USART2->DR;
			receivedDebugIndex++;
		}else {
			sprintf(receivedDebugPacketToDebug, "From Terminal  : %s", receivedDebugPacket);
			UARTDebugSend(receivedDebugPacketToDebug);
			for(clearDebugPacket = 0; clearDebugPacket < receivedDebugIndex ; clearDebugPacket++) receivedDebugPacket[clearDebugPacket] = '\0';
			receivedDebugIndex = 0;
		}
		USART2->SR &= ~(1ul << 5);	//The RXNE flag can also be cleared by writing a zero to it
	}
}

