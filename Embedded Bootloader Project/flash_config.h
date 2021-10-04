/*
This is for FLASH register address and some flash functions
*/

#include <stdint.h>

//Register Boundary Address Table 5 page 47 RM0038
#define flash_base						0x40023C00
//FLASH Register Map Table 24 page 91 RM0038
#define flash_pecr_offset 		0x04
#define flash_pekeyr_offset 	0x0C
#define flash_prgkeyr_offset	0x10
#define flash_optkeyr_offset	0x14

//FLASH Registers
typedef struct {
	uint32_t *PECR;				//Program/Erase Control Register
	uint32_t *PEKEYR;			//Program/erase key register
	uint32_t *PRGKEYR;		//Program memory key register
	uint32_t *OPTKEYR;		//Option byte key register
}flashRegister;

static flashRegister FLASH = {
	(uint32_t*)(flash_base + flash_pecr_offset), 
	(uint32_t*)(flash_base + flash_pekeyr_offset),
	(uint32_t*)(flash_base + flash_prgkeyr_offset),
	(uint32_t*)(flash_base + flash_optkeyr_offset)
};

uint8_t unlocking_Option_Byte_Block(void);
uint8_t unlocking_Program_Memory(void);
uint8_t unlocking_Flash_PECR_Register(void);
