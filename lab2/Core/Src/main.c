#include <main.h>

void clockInit()
{
	/* Power on HSE */
	if(!(RCC->CR & RCC_CR_HSEON))
	{
		RCC->CR |= RCC_CR_HSEON;
		while(!(RCC->CR & RCC_CR_HSERDY));
	}
	/* Power on HSI if power off */
	if(!(RCC->CR & RCC_CR_HSION))
	{
		RCC->CR |= RCC_CR_HSION;
		while(!(RCC->CR & RCC_CR_HSIRDY));
	}
}

void LEDsInit()
{
	/* GPIOD pins 12-15: output, low speed, no pull-up no pull-down */
	GPIOD->MODER &= ~(GPIO_MODER_MODE12
				    | GPIO_MODER_MODE13
				    | GPIO_MODER_MODE14
				    | GPIO_MODER_MODE15);

	GPIOD->MODER |= 0x1 << GPIO_MODER_MODE12_Pos
				  | 0x1 << GPIO_MODER_MODE13_Pos
				  | 0x1 << GPIO_MODER_MODE14_Pos
				  | 0x1 << GPIO_MODER_MODE15_Pos;

	GPIOD->OTYPER &= ~(GPIO_OTYPER_OT12
				     | GPIO_OTYPER_OT13
				     | GPIO_OTYPER_OT14
				     | GPIO_OTYPER_OT15);

	GPIOD->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED12
				      | GPIO_OSPEEDR_OSPEED13
				      | GPIO_OSPEEDR_OSPEED14
				      | GPIO_OSPEEDR_OSPEED15);

	/* Reset configured pins */
	GPIOD->BSRR |= GPIO_BSRR_BR12 | GPIO_BSRR_BR13 | GPIO_BSRR_BR14 | GPIO_BSRR_BR15;

}

void ButtonsInit()
{
	/* GPIOA pin 0: input, pull-down */
	GPIOA->MODER &= ~(GPIO_MODER_MODE0);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0);
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD0_Pos;
}

void perfialInit()
{
	LEDsInit();
	ButtonsInit();
}

void delay(uint32_t delay)
{
	for(uint32_t i = 0; i < delay; i++);
}

int main()
{
	clockInit();

	/* Enable GPIOA and GPIOA clocking */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	perfialInit();

	_Bool selector = 0;

	while(1)
	{
		if(GPIOA->IDR & GPIO_IDR_ID0)
		{
			selector = !selector;
			RCC->CFGR &= ~RCC_CFGR_SW;
			if(selector)
			{
				RCC->CFGR |= RCC_CFGR_SW_HSE;
				while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
			}
			else
			{
				RCC->CFGR |= RCC_CFGR_SW_HSI;
				while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
			}

			/* Blink for indicate that clock source switched */
			GPIOD->BSRR |= GPIO_BSRR_BS12;
			delay(20000);
			GPIOD->BSRR |= GPIO_BSRR_BR12;
			delay(20000);
		}

		GPIOD->BSRR |= GPIO_BSRR_BS13 | GPIO_BSRR_BS15;
		delay(2000000);
		GPIOD->BSRR |= GPIO_BSRR_BR13 | GPIO_BSRR_BR15;
		delay(2000000);
	}
}


