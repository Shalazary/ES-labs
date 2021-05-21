#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"

void clockInit();
void perfialInit();
void LEDsInit();
void ButtonsInit();
void delay(uint32_t delay);

#endif
