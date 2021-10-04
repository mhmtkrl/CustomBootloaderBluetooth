#include "flash_config.h"
#include "program_memory.h"

int16_t i = 0;
uint32_t adr;

int main() {
	adr = (uint32_t)APPLICATION_FIRMWARE_BASE_ADDRESS;
	program_Memory_Page_Erase(APPLICATION_FIRMWARE_BASE_ADDRESS);
	for(i = 0 ; i <= 128 ; i+=4) {
		program_Memory_Fast_Word_Write((APPLICATION_FIRMWARE_BASE_ADDRESS + i), i);
	}
}
