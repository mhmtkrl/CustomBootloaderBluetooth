#include "flash_config.h"

uint8_t optLock;

int main() {
	unlocking_Program_Memory();
	optLock = unlocking_Option_Byte_Block();
}
