/*
This is for FLASH register address and some flash functions
*/

#include <stdint.h>
#include "stm32l1xx.h"                  // Device header

static void (*JumpToApplicationCode)(void);
void jumpFunction(uint32_t address);
void program_Memory_Page_Erase(uint32_t);
void program_Memory_Fast_Word_Write(uint32_t, uint32_t);
uint8_t unlocking_Option_Byte_Block(void);
void locking_Program_Memory(void);
uint8_t unlocking_Program_Memory(void);
uint8_t unlocking_Flash_PECR_Register(void);
