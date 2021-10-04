/*
This is for Reset and Clock register functions
*/

#include <stdint.h>

//2.097 MHz
#define CLOCK_FREQ 					2097000		
//Register Boundary Address Table 36 page 168 RM0038
#define rcc_base						0x40023800
//RCC Register Map Table 24 page 91 RM0038
#define ahbenr_offset		  	0x1C
#define apb2enr_offset			0x20
#define apb1enr_offset			0x24

//RCC Registers
typedef struct {
	uint32_t *AHBENR;					//AHB peripheral clock enable register
	uint32_t *APB2ENR;				//APB2 peripheral clock enable register
	uint32_t *APB1ENR;				//APB1 peripheral clock enable register
}RCCRegister;

static RCCRegister RCC = {
	(uint32_t*)(rcc_base + ahbenr_offset),
	(uint32_t*)(rcc_base + apb2enr_offset),
	(uint32_t*)(rcc_base + apb1enr_offset)
};
