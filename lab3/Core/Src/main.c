#include <main.h>

void sysClockInit()
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

void ADCInit()
{
	/* GPIOA pin 1: analog input */
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= 0x3 << GPIO_MODER_MODE1_Pos;
	/* Enable ADC1 clocking */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	/* Power on ADC1 */
	ADC1->CR2 |= ADC_CR2_ADON;
	/* Set sequence of regular channels */
	ADC1->SQR3 |= 0x1 << ADC_SQR3_SQ1_Pos;
	/* Set sampling time ~1 microseconds for 10 kOm resistor */
	ADC1->SMPR2 |= 0x6 << ADC_SMPR2_SMP1_Pos;
}

void perfialInit()
{
	LEDsInit();
}

int main()
{
	sysClockInit();

	/* Enable GPIOA and GPIOD clocking */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	perfialInit();
	ADCInit();

	while(1)
	{
		ADC1->CR2 |= ADC_CR2_SWSTART;
		while(!(ADC1->SR & ADC_SR_EOC));

		/* Result from 0 to 4095 */
		uint32_t ADCResult = ADC1->DR;

		GPIOD->ODR &= ~(GPIO_ODR_OD12 | GPIO_ODR_OD13 | GPIO_ODR_OD14 | GPIO_ODR_OD15);

		GPIOD->ODR |= GPIO_ODR_OD12 * (ADCResult >= 0)
				    | GPIO_ODR_OD13 * (ADCResult >= 1024)
					| GPIO_ODR_OD14 * (ADCResult >= 2048)
					| GPIO_ODR_OD15 * (ADCResult >= 3072);
	}
}


