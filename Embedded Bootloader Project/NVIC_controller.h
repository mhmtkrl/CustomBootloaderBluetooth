/*
This is for NVIC register address
*/

#include <stdint.h>

//Cortex M4 Devices Generic User Guide Tabke 4-2 NVIC Register Summary
#define NVIC_ISER1						0xE000E104
#define NVIC_ISPR1						0XE000E204
#define NVIC_ICPR1						0XE000E284


//Table 51. Vector table RM0038
#define USART2_IRQ_NUMBER			38

//USART Registers
typedef struct {
	uint32_t *ISER1;				//Interrupt Set-enable Registers	
	uint32_t *ISPR1;				//Interrupt Set-pending Registers
	uint32_t *ICPR1;				// Interrupt Clear-pending Registers
}NVICRegister;

static NVICRegister NVIC = {
	(uint32_t*)(NVIC_ISER1),
	(uint32_t*)(NVIC_ISPR1),
	(uint32_t*)(NVIC_ICPR1)
};
