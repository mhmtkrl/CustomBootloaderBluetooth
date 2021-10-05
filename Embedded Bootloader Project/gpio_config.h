/*
This is for GPIO register address
*/

#include <stdint.h>

//Board specific definations
#define USER_LED							5

//Register Boundary Address Table 5 page 47 RM0038
#define gpioa_base						0x40020000
#define gpiob_base						0x40020400
#define gpioc_base						0x40020800
//FLASH Register Map Table 40 page 189 RM0038
#define moder_offset 					0x00
#define idr_offset 						0x10
#define bsrr_offset						0x18
#define afrl_offset						0x20
#define afrh_offset						0x24

//FLASH Registers
typedef struct {
	uint32_t *MODER;				 //GPIO port mode register
	uint32_t *IDR;					 //GPIO port input data register
	uint32_t *BSRR;					 //GPIO port bit set/reset register
	uint32_t *AFRL;					 //GPIO alternate function low register
	uint32_t *AFRH;					 //GPIO alternate function high register
}GPIORegister;

static GPIORegister GPIOA = {
	(uint32_t*)(gpioa_base + moder_offset), 
	(uint32_t*)(gpioa_base + idr_offset),
	(uint32_t*)(gpioa_base + bsrr_offset),
	(uint32_t*)(gpioa_base + afrl_offset),
	(uint32_t*)(gpioa_base + afrh_offset)
};

static GPIORegister GPIOB = {
	(uint32_t*)(gpiob_base + moder_offset), 
	(uint32_t*)(gpiob_base + idr_offset),
	(uint32_t*)(gpiob_base + bsrr_offset),
	(uint32_t*)(gpiob_base + afrl_offset),
	(uint32_t*)(gpiob_base + afrh_offset)
};

static GPIORegister GPIOC = {
	(uint32_t*)(gpioc_base + moder_offset), 
	(uint32_t*)(gpioc_base + idr_offset),
	(uint32_t*)(gpioc_base + bsrr_offset),
	(uint32_t*)(gpioc_base + afrl_offset),
	(uint32_t*)(gpioc_base + afrh_offset)
};

void InitUserLED(void);
void OnUserLED(void);
void OffUserLED(void);
