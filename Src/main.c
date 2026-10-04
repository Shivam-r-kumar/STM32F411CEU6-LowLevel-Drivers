#include "GPIO_DRIVER.h"

void delay(){
	for(volatile int i = 0; i < 500000; i++);
}

int main(void)
{
	GPIO_HANDLE_t gpioled;

	gpioled.pGPIOx = GPIOC;
	gpioled.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	gpioled.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpioled.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_TYPE_PP;
	gpioled.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	gpioled.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUSH_PULL;

	GPIO_PeriClockControl(GPIOC, ENABLE);
	GPIO_Init(&gpioled);
	while(1){
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		delay();
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		delay();

	}
}
