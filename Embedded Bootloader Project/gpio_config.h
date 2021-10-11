/*
This is for GPIO register address
*/

#include <stdint.h>
#include "stm32l1xx.h"                  // Device header

//Board specific definations
#define USER_LED							5



void InitUserLED(void);
void OnUserLED(void);
void OffUserLED(void);
