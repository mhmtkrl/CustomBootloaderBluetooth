#include "flash_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"

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

/*
This operation is used to erase a page in program memory (64 words).
• Unlock the FLASH_PECR register
• Unlock the Program memory
• Set the ERASE bit in the FLASH_PECR register
• Set the PROG bit in the FLASH_PECR register to choose program page
• Write 0x0000 0000 to the first word of the program page to erase
• Wait for the BSY bit to be cleared
*/
void program_Memory_Page_Erase(uint32_t pageBaseAddr) {
	if(!unlocking_Program_Memory()) {
		FLASH->PECR |= (1ul << 9);
		FLASH->PECR |= (1ul << 3);
		*(uint32_t*) pageBaseAddr = 0x00000000;
		while((FLASH->SR & 0x00000001));
	}
}

/*
This operation is used to write a word to the Program Memory
*/
void program_Memory_Fast_Word_Write(uint32_t addr, uint32_t data) {
	//If the program memory is unlocked
	if(!unlocking_Program_Memory()) {
		FLASH->PECR &= ~(1ul << 9);
		FLASH->PECR &= ~(1ul << 3);
		FLASH->PECR &= ~(1ul << 8);
		*(uint32_t*)addr = data;
	}
}

/*
Unlocking the option byte block, page 63, RM0038 
0 -> UNLOCKED
1 -> LOCKED
*/
uint8_t unlocking_Option_Byte_Block(void) {
	//If the FLASH PECR is unlocked then perform the Option Byte Block unlocking
	if(!unlocking_Flash_PECR_Register() && ((FLASH->PECR) & 0x04)) {
		FLASH->OPTKEYR = 0xFBEAD9C8;
		FLASH->OPTKEYR = 0x24252627;
	}
	return ((FLASH->PECR) & 0x04);
}

void locking_Program_Memory(void) {
	FLASH->PECR  |= 1ul << 0;
	FLASH->PECR |= 1ul << 1;
}

/*
Unlocking the Program Memory, page 63, RM0038 
0 -> UNLOCKED
1 -> LOCKED
*/
uint8_t unlocking_Program_Memory(void) {
	//If the FLASH PECR is unlocked then perform the Program Memory unlocking
	if(!unlocking_Flash_PECR_Register() && ((FLASH->PECR) & 0x02)) {
		FLASH->PRGKEYR = 0x8C9DAEBF;
		FLASH->PRGKEYR = 0x13141516;
	}
	return ((FLASH->PECR) & 0x02);
}

/*
Unlocking the Data EEPROM block and the FLASH_PECR register, page 62, RM0038 
0 -> UNLOCKED
1 -> LOCKED
*/
uint8_t unlocking_Flash_PECR_Register(void) {
	//If it is locked then perform unlocking
	if(((FLASH->PECR) & 0x01)) {
		FLASH->PEKEYR = 0x89ABCDEF;
		FLASH->PEKEYR = 0x02030405;
	}
	return ((FLASH->PECR) & 0x01);
}
