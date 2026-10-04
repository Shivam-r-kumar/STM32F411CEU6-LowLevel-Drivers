#include "GPIO_DRIVER.h"

// PERIPHERAL CLOCK SETUP
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pGPIOx == GPIOA)
			GPIOA_CLOCK_ENABLE();
		else if (pGPIOx == GPIOB)
			GPIOB_CLOCK_ENABLE();
		else if (pGPIOx == GPIOC)
			GPIOC_CLOCK_ENABLE();
	} else {
		if (pGPIOx == GPIOA)
			GPIOA_CLOCK_DISABLE();
		else if (pGPIOx == GPIOB)
			GPIOB_CLOCK_DISABLE();
		else if (pGPIOx == GPIOC)
			GPIOC_CLOCK_DISABLE();
	}
}

// INIT AND DE-INIT
void GPIO_Init(GPIO_HANDLE_t *pGPIOHandle) {
	uint32_t temp = 0;
	// Configure the mode of gpio pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= 3) {
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode
				<< (2 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
		// Clearing the bit
		pGPIOHandle->pGPIOx->MODER &= ~(0x3U
				<< (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		// Setting the bit
		pGPIOHandle->pGPIOx->MODER |= temp;
	} else {
		// Interupt case implement later

		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
			// handle falling

			// 1.configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clearing the RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
			// handle rising

			// 1.configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clearing the FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			// handle falling and rising

			// 1.configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			// 1.configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

		// 2. Configure the Gpio port selection in SYSCNF_EXTICR

		uint8_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4);
		uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4);
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG->EXTICR[temp1] = (portcode << (temp2 * 4));

		// 3. enable the EXTI Interupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

	// Configure the output type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType
			<< (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	// Clearing the bit
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1U
			<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	// Setting the bit
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	// Configure the output speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed
			<< (2 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
	// Clearing the bit
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3U
			<< (2 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
	// Setting the bit
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	// Configure the pull up / down
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl
			<< (2 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
	// Clearing the bit
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3U
			<< (2 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
	// Setting the bit
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	// Configure the alternate function
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber > 7) {
			temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode
					<< (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
			// Clearing the bit
			pGPIOHandle->pGPIOx->AFR[1] &= ~(0xFU
					<< (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
			// Setting the bit
			pGPIOHandle->pGPIOx->AFR[1] |= temp;
		} else {
			temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode
					<< (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
			// Clearing the bit
			pGPIOHandle->pGPIOx->AFR[0] &= ~(0xFU
					<< (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
			// Setting the bit
			pGPIOHandle->pGPIOx->AFR[0] |= temp;

		}
	}
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {
	if (pGPIOx == GPIOA)
		GPIOA_REG_RESET();
	else if (pGPIOx == GPIOB)
		GPIOB_REG_RESET();
	else if (pGPIOx == GPIOC)
		GPIOC_REG_RESET();
}

// DATA READ AND WRITE
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	return (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001);
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {
	return (uint16_t) pGPIOx->IDR;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber,
		uint8_t Value) {
	if (Value == GPIO_PIN_SET) {
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	pGPIOx->ODR ^= (1 << PinNumber);
}

// IRQ CONFIGURATION AND ISR HANDLING
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

