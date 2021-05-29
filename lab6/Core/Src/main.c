#include "main.h"

#define PWM_SourceFreq 500000
#define PWM_Freq 300
#define PWM_MaxCounter (PWM_SourceFreq / PWM_Freq)

debouncePin button;

uint8_t brightness = 0;
uint64_t PWM_Counter = 0;

void TIM2_IRQHandler()
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		PWM_Counter = (PWM_Counter + 1) % PWM_MaxCounter;
		if(PWM_Counter < (PWM_MaxCounter * brightness / 100))
		{
			GPIOD->ODR |= GPIO_ODR_OD12;
		}
		else
		{
			GPIOD->ODR &= ~GPIO_ODR_OD12;
		}
		scan(&button);
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

void EXTI0_IRQHandler()
{
	if(EXTI->PR & EXTI_PR_PR0 && button.isLow)
	{
		EXTI->PR |= EXTI_PR_PR0;
		brightness = (brightness + 20) % 120;
	}
}

int main()
{
	clockInit();
	LEDsInit();
	ButtonsInit();
	button.GPIO = GPIOA;
	button.GPIO_Pin = GPIO_IDR_ID0;
	button.filterTime = 5000; /* 50 ms */
	button.filterCounter = 0;
	TIMInit();

	/* Start timers */
	TIM2->CR1 |= TIM_CR1_CEN;

	while(1)
	{
	}
}


