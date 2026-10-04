#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include "STM32F411CEUx.h"

// GPIO PIN SELECTION MACROS
#define GPIO_PIN_0                  0U
#define GPIO_PIN_1                  1U
#define GPIO_PIN_2                  2U
#define GPIO_PIN_3                  3U
#define GPIO_PIN_4                  4U
#define GPIO_PIN_5                  5U
#define GPIO_PIN_6                  6U
#define GPIO_PIN_7                  7U
#define GPIO_PIN_8                  8U
#define GPIO_PIN_9                  9U
#define GPIO_PIN_10                 10U
#define GPIO_PIN_11                 11U
#define GPIO_PIN_12                 12U
#define GPIO_PIN_13                 13U
#define GPIO_PIN_14                 14U
#define GPIO_PIN_15                 15U


// GPIO MODES MACROS
#define GPIO_MODE_INPUT             0U
#define GPIO_MODE_OUTPUT   		    1U
#define GPIO_MODE_ALTFN             2U
#define GPIO_MODE_ANALOG  		    3U
#define GPIO_MODE_IT_FT             4U
#define GPIO_MODE_IT_RT 			5U
#define GPIO_MODE_IT_RFT			6U

// GPIO OUTPUT TYPE MACROS
#define GPIO_OUTPUT_TYPE_PP		    0U
#define GPIO_OUTPUT_TYPE_OD 		1U

// GPIO PIN SPEED MACROS
#define GPIO_SPEED_LOW              0U
#define GPIO_SPEED_MEDIUM 			1U
#define GPIO_SPEED_FAST 			2U
#define GPIO_SPEED_HIGH				3U

// GPIO PULL UP /DOWN MACROS
#define GPIO_NO_PUSH_PULL 			0U
#define GPIO_PULL_UP 				1U
#define GPIO_PULL_DOWN              2U

// GPIO ALT FUNCTIONS MACROS
#define GPIO_AF0        0U      // AF0  : System / MCO
#define GPIO_AF1        1U      // AF1  : TIM1, TIM2
#define GPIO_AF2        2U      // AF2  : TIM3, TIM4, TIM5
#define GPIO_AF3        3U      // AF3  : TIM9, TIM10, TIM11
#define GPIO_AF4        4U      // AF4  : I2C1, I2C2, I2C3
#define GPIO_AF5        5U      // AF5  : SPI1, SPI2, I2S2
#define GPIO_AF6        6U      // AF6  : SPI2, SPI3, I2S2, I2S3, I2S4, I2S5
#define GPIO_AF7        7U      // AF7  : SPI3, USART1, USART2
#define GPIO_AF8        8U      // AF8  : USART6
#define GPIO_AF9        9U      // AF9  : I2C2, I2C3
#define GPIO_AF10       10U     // AF10 : OTG_FS (USB)
#define GPIO_AF11       11U     // AF11 : Reserved / Not used
#define GPIO_AF12       12U     // AF12 : SDIO
#define GPIO_AF13       13U     // AF13 : Reserved / Not used
#define GPIO_AF14       14U     // AF14 : Reserved / Not used
#define GPIO_AF15       15U     // AF15 : EVENTOUT


// this is a pin configuration structure for a gpio pin
typedef struct {
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;

} GPIO_PinConfig_t;


// this is a handle structure for a gpio pin
typedef struct {
	GPIO_RegDef_t* pGPIOx;  // this holds the base address of the gpio port to which the pin belong
	GPIO_PinConfig_t GPIO_PinConfig;

} GPIO_HANDLE_t;

// PERIPHERAL CLOCK SETUP
void GPIO_PeriClockControl(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi);

// INIT AND DE-INIT
void GPIO_Init(GPIO_HANDLE_t* pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t* pGPIOx);

// DATA READ AND WRITE
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t* pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t* pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);

// IRQ CONFIGURATION AND ISR HANDLING
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);




#endif
