#include "flash_config.h"

uint8_t optLock;

int main() {
	program_Memory_Page_Erase(0x08001000);
	program_Memory_Fast_Word_Write(0x08001000, 0xAABBCCDD);
}
