#include "clock_config.h"
#include "gpio_config.h"

void InitUserLED() {
	//AHB is enabled for GPIOA
	RCC->AHBENR |= 1ul << 0;
	//PA5 is output
	GPIOA->MODER |= 1ul << 10;
	GPIOA->MODER &= ~(1ul << 11);
}

void OnUserLED() {
	//PA5 Lojik-0 -> off
	GPIOA->BSRR |= 1ul << 21;
	//PA5 Lojik-1 -> on
	GPIOA->BSRR |= 1ul << 5;
}

void OffUserLED() {
	//PA5 Lojik-0 -> off
	GPIOA->BSRR |= 1ul << 5;
	//PA5 Lojik-1 -> on
	GPIOA->BSRR |= 1ul << 21;
}
