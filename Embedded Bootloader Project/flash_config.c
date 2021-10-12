#include "flash_config.h"
#include "sysTick_timer_config.h"
#include "uart_config.h"
#include <stdio.h>

/*
https://developer.arm.com/documentation/ka002218/latest
Arm Compiler 5
*/


void BootJump( uint32_t *Address ) {
	//@(APPLICATION_FIRMWARE_BASE_ADDRESS + 4)
	reset_handler_function reset_handler = (reset_handler_function) (( uint32_t )*(uint32_t *)(Address + 1));
	//Disable Interrupts
 __disable_irq();
	//Disable SysTick
	SysTick->CTRL = 0;
	//Clear SysTick's exception pending bit
	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
	//Disable individual fault handlers if the bootloader used them
	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | 
                 SCB_SHCSR_BUSFAULTENA_Msk | 
                 SCB_SHCSR_MEMFAULTENA_Msk ) ;
	//Activate the MSP, if the core is found to currently run with the PSP. 
	//As the compiler might still use the stack, the PSP needs to be copied to the MSP before this
	if( CONTROL_SPSEL_Msk & __get_CONTROL( ) )
	{  /* MSP is not active */
		__set_MSP( __get_PSP( ) ) ;
		__set_CONTROL( __get_CONTROL( ) & ~CONTROL_SPSEL_Msk ) ;
	}
	//Load the vector table address of the user application into SCB->VTOR register
	SCB->VTOR = ( uint32_t )Address ;
	/*The final part is to set the MSP to the value found in the user application vector table and then 
	load the PC with the reset vector value of the user application. 
	This can't be done in C, as it is always possible, that the compiler uses the current SP. 
	But that would be gone after setting the new MSP. So, a call to a small assembler function is done.
	*/
	//Address is an argument of the function, it is actually a pointer that points APPLICATION_FIRMWARE_BASE_ADDRESS
	//There is a SP at this address, and next instruction's address will be loaded into PC
	 __set_MSP(( uint32_t )*(uint32_t *)Address);
 
  reset_handler();
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
