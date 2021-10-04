#include "flash_config.h"

//Reset value of PECR is 0x0000 0007, so let's check it
uint32_t pecrValue;

int main() {
	pecrValue = *FLASH.PECR;
}
