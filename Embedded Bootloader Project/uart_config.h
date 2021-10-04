/*
This is for UART register address
*/

#include <stdint.h>

//Register Boundary Address Table 5 page 47 RM0038
#define usart2_base						0x40004400
#define usart3_base						0x40004800
//USART Register Map Table 158 page 743 RM0038
#define usart_sr_offset 		0x00
#define usart_dr_offset 		0x04
#define usart_brr_offset		0x08
#define usart_cr1_offset		0x0C 
#define usart_cr2_offset		0x10
#define usart_cr3_offset		0x14 
#define usart_gtpr_offset		0x18

//USART Registers
typedef struct {
	uint32_t *SR;				//Program/Erase Control Register
	uint32_t *DR;			//Program/erase key register
	uint32_t *BRR;		//Program memory key register
	uint32_t *CR1;		//Option byte key register
	uint32_t *CR2;					//Status register
	uint32_t *CR3;		//Option byte key register
	uint32_t *GTPR;					//Status register
}USARTRegister;

static USARTRegister USART2 = {
	(uint32_t*)(usart2_base + usart_sr_offset), 
	(uint32_t*)(usart2_base + usart_dr_offset),
	(uint32_t*)(usart2_base + usart_brr_offset),
	(uint32_t*)(usart2_base + usart_cr1_offset),
	(uint32_t*)(usart2_base + usart_cr2_offset),
	(uint32_t*)(usart2_base + usart_cr3_offset),
	(uint32_t*)(usart2_base + usart_gtpr_offset)
};

static USARTRegister USART3 = {
	(uint32_t*)(usart3_base + usart_sr_offset), 
	(uint32_t*)(usart3_base + usart_dr_offset),
	(uint32_t*)(usart3_base + usart_brr_offset),
	(uint32_t*)(usart3_base + usart_cr1_offset),
	(uint32_t*)(usart3_base + usart_cr2_offset),
	(uint32_t*)(usart3_base + usart_cr3_offset),
	(uint32_t*)(usart3_base + usart_gtpr_offset)
};

void InitUARTforDebug(void);
