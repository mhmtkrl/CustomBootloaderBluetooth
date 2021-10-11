/*
This is for GPIO register address
*/

#include <stdint.h>

//Board specific definations
#define USER_LED							5

void InitUserLED(void);
void OnUserLED(void);
void OffUserLED(void);
