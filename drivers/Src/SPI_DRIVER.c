#include "SPI_DRIVER.h"

// supporting function
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName) {
	return (pSPIx->SR & flagName);
}

__attribute__ ((weak)) void SPI_ApplicationEventCallback(
		SPI_Handle_t *pSPIHandle, uint8_t event) {
}
static void spi_txe_interupt_handle(SPI_Handle_t *pSPIHandle) {
	while (pSPIHandle->TxLen > 0) {
		// wait unitl txe is set
		while (SPI_GetFlagStatus(pSPIHandle->pSPIx, SPI_TXE_FLAG) == FLAG_RESET)
			;

		// CHEKS DFF MODE AND IMPLEMENT DEPENDS ON DATA FORMAT
		if (pSPIHandle->pSPIx->CR1 & (1 << 11)) {
			// FOR 16-BIT FORMAT MODE
			pSPIHandle->pSPIx->DR = *((uint16_t*) pSPIHandle->pTxBuffer);
			pSPIHandle->TxLen = pSPIHandle->TxLen - 2;
			(uint16_t*) pSPIHandle->pTxBuffer++; // INCREMENT THE POINTER
		} else {
			// FOR 8-Bit FORMAT MODE
			pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);
			pSPIHandle->TxLen = pSPIHandle->TxLen - 1;
			pSPIHandle->pTxBuffer++; // INCREMENT THE POINTER
		}

		// closing the spi communication
		if (!pSPIHandle->TxLen) {
			pSPIHandle->pSPIx->CR2 &= ~(1 << 7);
			pSPIHandle->pTxBuffer = NULL;
			pSPIHandle->TxLen = 0;
			pSPIHandle->TxState = SPI_READY;
			SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_COMPLETE);
		}

	}
}
static void spi_rxne_interupt_handle(SPI_Handle_t *pSPIHandle) {
	while (pSPIHandle->TxLen > 0) {
		// wait unitl txe is set
		while (SPI_GetFlagStatus(pSPIHandle->pSPIx, SPI_RXNE_FLAG) == FLAG_RESET)
			;

		// CHEKS DFF MODE AND IMPLEMENT DEPENDS ON DATA FORMAT
		if (pSPIHandle->pSPIx->CR1 & (1 << 11)) {
			// FOR 16-BIT FORMAT MODE
			pSPIHandle->pSPIx->DR = *((uint16_t*) pSPIHandle->pRxBuffer);
			pSPIHandle->RxLen = pSPIHandle->RxLen - 2;
			(uint16_t*) pSPIHandle->pRxBuffer++; // INCREMENT THE POINTER
		} else {
			// FOR 8-Bit FORMAT MODE
			pSPIHandle->pSPIx->DR = *(pSPIHandle->pRxBuffer);
			pSPIHandle->RxLen = pSPIHandle->RxLen - 1;
			pSPIHandle->pRxBuffer++; // INCREMENT THE POINTER
		}

		// closing the spi communication
		if (!pSPIHandle->RxLen) {
			pSPIHandle->pSPIx->CR2 &= ~(1 << 7);
			pSPIHandle->pRxBuffer = NULL;
			pSPIHandle->RxLen = 0;
			pSPIHandle->RxState = SPI_READY;
			SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_COMPLETE);
		}

	}
}

static void spi_ovr_interupt_handle(SPI_Handle_t *pSPIHandle) {
	if (pSPIHandle->TxState != SPI_BUSY_IN_TX) {
		(void) pSPIHandle->pSPIx->DR;
		(void) pSPIHandle->pSPIx->SR;
	}

	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERROR);
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << 7);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &= ~(1 << 7);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

// SPI CLOCK CONTROL
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_CLOCK_ENABLE();
		} else if (pSPIx == SPI2) {
			SPI2_CLOCK_ENABLE();
		} else if (pSPIx == SPI3) {
			SPI3_CLOCK_ENABLE();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_CLOCK_DISABLE();
		} else if (pSPIx == SPI2) {
			SPI2_CLOCK_DISABLE();
		} else if (pSPIx == SPI3) {
			SPI3_CLOCK_DISABLE();
		}
	}
}

// INIT AND DE-INIT
void SPI_Init(SPI_Handle_t *pSPIHandle) {
	//ENABLING THE CLOCK
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// first configure CR1 Register
	uint32_t tempReg = 0;

	// configure the device mode
	tempReg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << 2);

	// configure the bus configuration
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BusConfig_FD) {
		// BIDI mode should be cleared
		tempReg &= ~(1 << 15);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BusConfig_HD) {
		// BIDI mode should be set
		tempReg |= (1 << 15);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig
			== SPI_BusConfig_SIMPLEX_RXONLY) {
		// BIDI mode should be cleared
		tempReg &= ~(1 << 15);
		// RXONLY Bit must be set
		tempReg |= (1 << 10);
	}

	// Configure the clock speed
	tempReg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << 3);

	// configure the data format
	tempReg |= (pSPIHandle->SPIConfig.SPI_DFF << 11);

	// configure the CPOL - clock polarity
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPOL << 1);

	//configure the CPHA - clock phase
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPHA << 0);

	pSPIHandle->pSPIx->CR1 = tempReg;

}
void SPI_DeInit(SPI_RegDef_t *pSPIx) {
	if (pSPIx == SPI1)
		SPI2_REG_RESET();
	else if (pSPIx == SPI2)
		SPI2_REG_RESET();
	else if (pSPIx == SPI3)
		SPI3_REG_RESET();
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi)
		pSPIx->CR1 |= (1 << 8);
	else
		pSPIx->CR1 &= ~(1 << 8);
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi)
		pSPIx->CR2 |= (1 << 2);
	else
		pSPIx->CR2 &= ~(1 << 2);
}

// DATA SEND AND RECEIVE
void SPI_DataSend(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len) {
	while (Len > 0) {
		// wait unitl txe is set
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET)
			;

		// CHEKS DFF MODE AND IMPLEMENT DEPENDS ON DATA FORMAT
		if (pSPIx->CR1 & (1 << 11)) {
			// FOR 16-BIT FORMAT MODE
			pSPIx->DR = *((uint16_t*) pTxBuffer);
			Len = Len - 2;
			(uint16_t*) pTxBuffer++; // INCREMENT THE POINTER
		} else {
			// FOR 8-Bit FORMAT MODE
			pSPIx->DR = *(pTxBuffer);
			Len = Len - 1;
			pTxBuffer++; // INCREMENT THE POINTER
		}

	}
}

void SPI_DataRecieve(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len) {
	while (Len > 0) {
		// wait unitl Rxe is set
		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET)
			;

		// CHEKS DFF MODE AND IMPLEMENT DEPENDS ON DATA FORMAT
		if (pSPIx->CR1 & (1 << 11)) {
			// FOR 16-BIT FORMAT MODE
			*((uint16_t*) pRxBuffer) = pSPIx->DR;
			Len = Len - 2;
			(uint16_t*) pRxBuffer++; // INCREMENT THE POINTER
		} else {
			// FOR 8-Bit FORMAT MODE
			*(pRxBuffer) = pSPIx->DR;
			Len = Len - 1;
			pRxBuffer++; // INCREMENT THE POINTER
		}

	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer,
		uint32_t Len) {
	if (pSPIHandle->RxState != SPI_BUSY_IN_TX) {
		// Save the Tx buffer address and leng info in global variable
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;
		//mark the spi state busy in transmission state so that no other code take over spi
		pSPIHandle->TxState = SPI_BUSY_IN_TX;
		// enable the TXEIE control bit to get interupt wheter txe flag is set in sr
		pSPIHandle->pSPIx->CR2 = (1 << 7);

		// Data transmission will be handled in isr code

	}
	return pSPIHandle->TxState;
}

uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer,
		uint32_t Len) {
	if (pSPIHandle->RxState != SPI_BUSY_IN_RX) {
		// Save the Tx buffer address and leng info in global variable
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		//mark the spi state busy in recieving state so that no other code take over spi
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		// enable the RXEIE control bit to get interupt wheter txe flag is set in sr
		pSPIHandle->pSPIx->CR2 = (1 << 6);

		// Data transmission will be handled in isr code

	}
	return pSPIHandle->TxState;
}

// IRQ CONFIGURATION AND ISR HANDLING
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t Priority, uint8_t EnorDi) {
	uint8_t reg = IRQNumber / 32;
	uint8_t bit = IRQNumber % 32;

	if (EnorDi == 1) {
		NVIC->ISER[reg] |= (1U << bit);
	} else if (EnorDi == 0) {
		NVIC->ICER[reg] |= (1U << bit);
	}

	// COFIGURING PRIORITY REGISTER IN NVIC
	uint8_t prx = IRQNumber / 4;
	uint8_t prxSection = IRQNumber % 4;

	NVIC->IPR[prx] |= (Priority << (8 * prxSection + 4));
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle) {
	int temp1, temp2 = 0;
	// for txe
	temp1 = pSPIHandle->pSPIx->SR & (1 << 1);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << 7);

	if (temp1 && temp2) {
		spi_txe_interupt_handle(pSPIHandle);
	}

	// for rxne
	temp1 = pSPIHandle->pSPIx->SR & (1 << 0);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << 6);

	if (temp1 && temp2) {
		spi_rxne_interupt_handle(pSPIHandle);
	}

	// for over run
	temp1 = pSPIHandle->pSPIx->SR & (1 << 6);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << 5);

	if (temp1 && temp2) {
		spi_ovr_interupt_handle(pSPIHandle);
	}
}

// Other API's

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE)
		pSPIx->CR1 |= (1 << 6);
	else if (EnOrDi == DISABLE)
		pSPIx->CR1 &= ~(1 << 6);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
	(void) pSPIx->DR;
	(void) pSPIx->SR;
}
