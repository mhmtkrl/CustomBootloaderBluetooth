#include "flash_config.h"

uint8_t pelock;

int main() {
	pelock = unlocking_Program_Memory();
}
