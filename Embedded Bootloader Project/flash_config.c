#include "flash_config.h"

/*
Unlocking the Program Memory, page 62, RM0038 
0 -> UNLOCKED
1 -> LOCKED
*/
uint8_t unlocking_Program_Memory(void) {
	//If the FLASH PECR is unlocked then perform the Program Memory unlocking
	if(!unlocking_Flash_PECR_Register()) {
		*FLASH.PRGKEYR = 0x8C9DAEBF;
		*FLASH.PRGKEYR = 0x13141516;
	}
	return ((*FLASH.PECR) & 0x01);
}

/*
Unlocking the Data EEPROM block and the FLASH_PECR register, page 62, RM0038 
0 -> UNLOCKED
1 -> LOCKED
*/
uint8_t unlocking_Flash_PECR_Register(void) {
	//If it is locked then perform unlocking
	if(((*FLASH.PECR) & 0x01)) {
		*FLASH.PEKEYR = 0x89ABCDEF;
		*FLASH.PEKEYR = 0x02030405;
	}
	return ((*FLASH.PECR) & 0x01);
}
