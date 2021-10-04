/*
This is for Reset and Clock register functions
*/

#include <stdint.h>

//Register Boundary Address Table 36 page 168 RM0038
#define rcc_base						0x40023800
//RCC Register Map Table 24 page 91 RM0038
#define ahbenr_offset		  	0x1C

//RCC Registers
typedef struct {
	uint32_t *AHBENR;				//AHB peripheral clock enable register
}RCCRegister;

static RCCRegister RCC = {
	(uint32_t*)(rcc_base + ahbenr_offset)
};
