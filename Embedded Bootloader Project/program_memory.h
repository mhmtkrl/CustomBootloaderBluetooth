#include <stdint.h>
//Table 11 NVM module organization (Cat.5 devices) page 56 RM0038
#define PAGE_SIZE										  256													//256 bytes
#define PROGRAM_MEMORY_BASE_ADDRESS		0x08000000

static uint16_t VECTOR_BASE = 0x000;

//Bootloader size is 32 kB
//Each sector is 4 kB, so first 7 Sectors are used for the bootloader 
#define BOOTLOADER_SIZE_KB		32
#define APPLICATION_FIRMWARE_BASE_ADDRESS		((PROGRAM_MEMORY_BASE_ADDRESS) + ((BOOTLOADER_SIZE_KB) * 1024))
