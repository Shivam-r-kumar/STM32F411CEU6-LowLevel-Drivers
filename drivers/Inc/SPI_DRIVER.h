#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include "STM32F411CEUx.h"

#define SPI_READY 			0
#define SPI_BUSY_IN_RX		1
#define SPI_BUSY_IN_TX		2

// SPI FLAGS

#define SPI_RXNE_FLAG   			(1U << 0U)
#define SPI_TXE_FLAG   				(1U << 1U)
#define SPI_CRCERP_FLAG   			(1U << 4U)
#define SPI_MODF_FLAG   			(1U << 5U)
#define SPI_OVR_FLAG   				(1U << 6U)
#define SPI_BSY_FLAG				(1U << 7U)
#define SPI_FRE_FLAG   				(1U << 8U)

// SPI COMPLETION EVENT
#define SPI_EVENT_TX_COMPLETE   	1
#define SPI_EVENT_RX_COMPLETE 		2
#define SPI_EVENT_OVR_ERROR			3
#define SPI_EVENT_CRC_ERROR			4

// COMMUNICATION PROTOCOL STRUCTURES
typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
} SPI_Config_t;

typedef struct {
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxState;
	uint8_t RxState;
} SPI_Handle_t;

// @SPI_DeviceMode
#define SPI_DeviceMode_MASTER    						1
#define SPI_DeviceMode_SLAVE							0

// @SPI_BusConfig
#define SPI_BusConfig_FD								1
#define SPI_BusConfig_HD								2
#define SPI_BusConfig_SIMPLEX_RXONLY					3

// @SPI_SclkSpeed
#define SPI_SclkSpeed_DIV2        						0
#define SPI_SclkSpeed_DIV4        						1
#define SPI_SclkSpeed_DIV8        						2
#define SPI_SclkSpeed_DIV16        						3
#define SPI_SclkSpeed_DIV32       						4
#define SPI_SclkSpeed_DIV64       						5
#define SPI_SclkSpeed_DIV128      						6
#define SPI_SclkSpeed_DIV256       						7

// @SPI_DFF
#define SPI_DFF_8Bit 									0
#define SPI_DFF_16Bit									1

// @SPI_CPOL
#define SPI_CPOL_HIGH 									1
#define SPI_CPOL_LOW									0

// @SPI_CPHA
#define SPI_CPHA_HIGH 									1
#define SPI_CPHA_LOW									0

// SPI CLOCK ENABLE OR DISABLE
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

// INIT AND DE-INIT
void SPI_Init(SPI_Handle_t *pSPIx);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// DATA SEND AND RECEIVE
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

// IRQ CONFIGURATION AND ISR HANDLING
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t Priority, uint8_t EnorDi);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

// OTHER APIS --------------------------------------------------------


// SSI ENALE PIN ( 1 - FOR SINGLE MASTER , 0 - FOR MULTI MASTER )
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
// SSOE Enabling API
/* MAKING SSOE 1 DOES NSS OUTPUT ENABLE
 * THE NSS PIN IS AUTOMATICALLY MANAGED BY THE HARDWARE
 * WHEN SPE = 1, NSS WILL BE PULLED TO LOW
 * AND NSS PIN WILL BE HIGH WHEN SPE = 0
*/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

#endif /* INC_SPI_DRIVER_H_ */
