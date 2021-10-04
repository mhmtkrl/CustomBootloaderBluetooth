#include "flash_config.h"
#include "program_memory.h"

int16_t i = 0;

int main() {
	program_Memory_Page_Erase(page16);
	for(i = 0 ; i < 128 ; i+=4) {
		program_Memory_Fast_Word_Write((page16 + i), i);
	}
}
