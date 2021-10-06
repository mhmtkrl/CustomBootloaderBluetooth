#include "stm32l1xx.h"                  // Device header
#include "sysTick_timer_config.h"

#define APP_MODE 							1



#define USART2_IRQ_NUMBER			38


#if APP_MODE == 0
	#define LED_ON_TIME		1000
	#define LED_OFF_TIME 	1000
#endif

#if APP_MODE == 1
	#define LED_ON_TIME		50
	#define LED_OFF_TIME 	500
#endif

void InitUARTforDebug(void) {
	//AHB is enabled for GPIOA
	RCC->AHBENR |= 1ul << 0;
	//AFIO7 for PA2
	GPIOA->AFR[0] |= (7ul << 8);
	//AFIO7 for PA3
	GPIOA->AFR[0]|= (7ul << 12);
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
	NVIC->ISER[1] |= 1ul << (USART2_IRQ_NUMBER%32);
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

int main() {
	__enable_irq();
	InitSysTickTimerInMiliseconds(1, (uint32_t)CLOCK_FREQ);
	InitUARTforDebug();	
	RCC->AHBENR |= 1ul << 0;  //AHB is enabled
	GPIOA->MODER |= 0xA8000400;	//PA5 Output
	
	UARTDebugSend("\r\n***************This is Application Code!***************\n");
	
	while(1) {
		GPIOA->ODR |= 1ul << 5;
		delayMS(LED_ON_TIME);
		GPIOA->ODR &= ~(1ul << 5);
		delayMS(LED_OFF_TIME);
	}	
}
