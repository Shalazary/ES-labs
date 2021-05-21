#include <main.h>

void clockInit()
{
	FLASH->ACR |= 0x6 << FLASH_ACR_LATENCY_Pos;

	/* Power on HSE */
	if(!(RCC->CR & RCC_CR_HSEON))
	{
		RCC->CR |= RCC_CR_HSEON;
		while(!(RCC->CR & RCC_CR_HSERDY));
	}

	/* Configure PLL */
	RCC->CR &= ~RCC_CR_PLLON;

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM
				  | RCC_PLLCFGR_PLLN
			      | RCC_PLLCFGR_PLLP
				  | RCC_PLLCFGR_PLLQ);

	RCC->PLLCFGR |= 8 << RCC_PLLCFGR_PLLM_Pos
				  | 336 << RCC_PLLCFGR_PLLN_Pos
				  | 2 << RCC_PLLCFGR_PLLP_Pos
				  | 7 << RCC_PLLCFGR_PLLQ_Pos;

	RCC->CR |= RCC_CR_PLLON;

	/* Configure ABP prescalers */
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= 0x5 << RCC_CFGR_PPRE1_Pos;

	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR |= 0x4 << RCC_CFGR_PPRE2_Pos;


	/* Set PLL as SYSCLK */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	/* Power off HSI */
	RCC->CR &= ~RCC_CR_HSION;
}

void perfialInit()
{
	LEDsInit();
	ButtonsInit();
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
		}

		if(selector)
		{
			GPIOD->BSRR |= GPIO_BSRR_BS12 | GPIO_BSRR_BS14;
			delay(2000000);
			GPIOD->BSRR |= GPIO_BSRR_BR12 | GPIO_BSRR_BR14;
			delay(2000000);
		}
		else
		{
			GPIOD->BSRR |= GPIO_BSRR_BS13 | GPIO_BSRR_BS15;
			delay(2000000);
			GPIOD->BSRR |= GPIO_BSRR_BR13 | GPIO_BSRR_BR15;
			delay(2000000);
		}
	}
}


