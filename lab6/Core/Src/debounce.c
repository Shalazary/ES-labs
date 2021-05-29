#include "debounce.h"

void scan(debouncePin *pin)
{
	if(pin->GPIO->IDR & pin->GPIO_Pin)
	{
		pin->filterCounter++;
		if(pin->filterCounter >= pin->filterTime)
		{
			pin->filterCounter = pin->filterTime;
			pin->isHight = 1;
			pin->isLow = 0;
		}

	}
	else
	{
		pin->filterCounter--;
		if(pin->filterCounter < 0)
		{
			pin->filterCounter = 0;
			pin->isHight = 0;
			pin->isLow = 1;
		}
	}
}
