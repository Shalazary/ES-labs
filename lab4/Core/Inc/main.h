#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "init.h"

void TIM6_DAC_IRQHandler();
void TIM7_IRQHandler();
void EXTI0_IRQHandler();
int main();

#endif
