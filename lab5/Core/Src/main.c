#include "main.h"

debouncePin button;

void TIM6_DAC_IRQHandler()
{
	if(TIM6->SR & TIM_SR_UIF)
	{
		if(GPIOD->ODR & GPIO_ODR_OD13 && GPIOD->ODR & GPIO_ODR_OD15)
			GPIOD->BSRR |= GPIO_BSRR_BR13 | GPIO_BSRR_BR15;
		else
			GPIOD->BSRR |= GPIO_BSRR_BS13 | GPIO_BSRR_BS15;

		TIM6->SR &= ~TIM_SR_UIF;
	}
}

void TIM7_IRQHandler()
{
	if(TIM7->SR & TIM_SR_UIF)
	{
		if(GPIOD->ODR & GPIO_ODR_OD12 && GPIOD->ODR & GPIO_ODR_OD14)
			GPIOD->BSRR |= GPIO_BSRR_BR12 | GPIO_BSRR_BR14;
		else
			GPIOD->BSRR |= GPIO_BSRR_BS12 | GPIO_BSRR_BS14;

		TIM7->SR &= ~TIM_SR_UIF;
	}
}

void TIM2_IRQHandler()
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		scan(&button);
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

void EXTI0_IRQHandler()
{
	if(EXTI->PR & EXTI_PR_PR0 && button.isLow)
	{
		EXTI->PR |= EXTI_PR_PR0;
		GPIOD->BSRR |= GPIO_BSRR_BR12 | GPIO_BSRR_BR13 | GPIO_BSRR_BR14 | GPIO_BSRR_BR15;

		if(TIM6->CR1 & TIM_CR1_CEN)
		{
			TIM6->CR1 &= ~TIM_CR1_CEN;
			TIM7->CR1 |= TIM_CR1_CEN;
		}
		else
		{
			TIM7->CR1 &= ~TIM_CR1_CEN;
			TIM6->CR1 |= TIM_CR1_CEN;
		}
	}
}

int main()
{
	clockInit();
	LEDsInit();
	ButtonsInit();
	button.GPIO = GPIOA;
	button.GPIO_Pin = GPIO_IDR_ID0;
	button.filterTime = 50; /* 50 ms */
	button.filterCounter = 0;
	TIMInit();

	/* Start timers */
	TIM6->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 |= TIM_CR1_CEN;

	while(1)
	{
	}
}


