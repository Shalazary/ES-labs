#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include "stm32f407xx.h"

typedef struct debouncePin
{
	GPIO_TypeDef *GPIO;
	uint32_t GPIO_Pin;
	uint32_t filterTime;
	int32_t filterCounter;
	uint8_t isLow;
	uint8_t isHight;
} debouncePin;

void scan(debouncePin *pin);

#endif /* DEBOUNCE_H_ */
