#ifndef INC_STM32F411CEUX_H_
#define INC_STM32F411CEUX_H_

#include  <stdint.h>

// SOME GENERIC MACROS

#define ENABLE 								1
#define DISABLE 							0
#define SET 								ENABLE
#define RESET 								DISABLE
#define GPIO_PIN_SET   						SET
#define GPIO_PIN_RESET 						RESET

#define FLASH_BASEADDR                		0x08000000U
#define SRAM1_BASEADDR 						0x20000000U
#define SRAM    							SRAM1_BASEADDR
#define SYSTEM_MEMORY_BASEADDR				0x1FFF0000U

#define APB1_BASEADDR						0x40000000U
#define APB2_BASEADDR 						0x40010000U
#define AHB1_BASEADDR 						0x40020000U
#define AHB2_BASEADDR    					0x50000000U

// AHB1 Peripheral -------

#define GPIOA_BASEADDR   					(AHB1_BASEADDR + 0x0000)
#define GPIOB_BASEADDR   					(AHB1_BASEADDR + 0x0400)
#define GPIOC_BASEADDR   					(AHB1_BASEADDR + 0x0800)

#define RCC_BASEADDR 						(AHB1_BASEADDR + 0x3800)

// APB1 PERIPHERAL ---------

#define TIM2_BASEADDR    					(APB1_BASEADDR + 0x0000)
#define TIM3_BASEADDR    					(APB1_BASEADDR + 0x0400)
#define TIM4_BASEADDR    					(APB1_BASEADDR + 0x0800)
#define TIM5_BASEADDR    					(APB1_BASEADDR + 0x0C00)

#define I2C1_BASEADDR    					(APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR    					(APB1_BASEADDR + 0x5800)
#define I2C3_BASEADDR    					(APB1_BASEADDR + 0x5C00)

#define SPI2_BASEADDR 						(APB1_BASEADDR + 0x3800)
#define SPI3_BASEADDR 						(APB1_BASEADDR + 0x3C00)

#define USART2_BASEADDR 					(APB1_BASEADDR + 0x4400)

// APB2 PERIPHERAL -----------

#define ADC1_BASEADDR    					(APB2_BASEADDR + 0x2000)

#define TIM1_BASEADDR    					(APB2_BASEADDR + 0x0000)
#define TIM9_BASEADDR    					(APB2_BASEADDR + 0x4000)
#define TIM10_BASEADDR    					(APB2_BASEADDR + 0x4400)
#define TIM11_BASEADDR    					(APB2_BASEADDR + 0x4800)

#define USART1_BASEADDR    					(APB2_BASEADDR + 0x1000)
#define USART6_BASEADDR    					(APB2_BASEADDR + 0x1400)
#define SPI1_BASEADDR    					(APB2_BASEADDR + 0x3000)
#define SPI4_BASEADDR    					(APB2_BASEADDR + 0x3400)
#define SPI5_BASEADDR    					(APB2_BASEADDR + 0x5000)

#define EXTI_BASEADDR    					(APB2_BASEADDR + 0x3C00)

#define SYSCFG_BASEADDR 					(APB2_BASEADDR + 0x3800)

/* PERIPERAL REGISTER DEFINITION STRUCTURES */

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];

} GPIO_RegDef_t;

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	uint32_t RESERVED0[2];
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	uint32_t RESERVED2[2];
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	uint32_t RESERVED4[2];
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t DCKCFGR;

} RCC_RegDef_t;

// EXTI REGISTER DEFINITION STRUCTURES

typedef struct {
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;

} EXTI_RegDef_t;

// SYSCFG REGISTER DEFINITION STRUCTURE

typedef struct {
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];

	volatile uint32_t RESERVED[2];

	volatile uint32_t CMPCR;

} SYSCFG_RegDef_t;

// PERIPHERAL DEFINITIONS ( PERIPERAL BASE ADDRESSES TYPECASTED TO GPIO_REGDEF_T )

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR )
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASEADDR )
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASEADDR )

#define RCC ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI ((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

// Clock enable macro for Gpio peripheral

#define GPIOA_CLOCK_ENABLE() ( RCC->AHB1ENR |= (1 << 0) )
#define GPIOB_CLOCK_ENABLE() ( RCC->AHB1ENR |= (1 << 1) )
#define GPIOC_CLOCK_ENABLE() ( RCC->AHB1ENR |= (1 << 2) )

// Clock enable macro for I2C peripheral

#define I2C1_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 21))
#define I2C2_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 22))
#define I2C3_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 23))

// Clock enable macro for SPI peripheral

#define SPI1_CLOCK_ENABLE()  (RCC->AHB2ENR |= (1 << 12))
#define SPI2_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 14))
#define SPI3_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 15))

// Clock enable macro for USART peripheral

#define USART1_CLOCK_ENABLE()  (RCC->AHB2ENR |= (1 << 4))
#define USART2_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1 << 17))
#define USART6_CLOCK_ENABLE()  (RCC->AHB2ENR |= (1 << 5))

// Clock enable macro for SYSCFG peripheral
#define SYSCFG_CLOCK_ENABLE()  (RCC->APB2ENR |= (1 << 14))

// Clock enable macro for Gpio peripheral

#define GPIOA_CLOCK_DISABLE() ( RCC->AHB1ENR &= ~(1 << 0) )
#define GPIOB_CLOCK_DISABLE() ( RCC->AHB1ENR &= ~(1 << 1) )
#define GPIOC_CLOCK_DISABLE() ( RCC->AHB1ENR &= ~(1 << 2) )

// Clock enable macro for I2C peripheral

#define I2C1_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 21))
#define I2C2_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 22))
#define I2C3_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 23))

// Clock enable macro for SPI peripheral

#define SPI1_CLOCK_DISABLE()  (RCC->AHB2ENR &= ~(1 << 12))
#define SPI2_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 14))
#define SPI3_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 15))

// Clock enable macro for USART peripheral

#define USART1_CLOCK_DISABLE()  (RCC->AHB2ENR &= ~(1 << 4))
#define USART2_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1 << 17))
#define USART6_CLOCK_DISABLE()  (RCC->AHB2ENR &= ~(1 << 5))

// Macros to Reset GPIOx Peripheral

#define GPIOA_REG_RESET()       do { (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()       do { (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()       do { (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); } while(0)

// Macros for PORT SELECT ON SYSCFG REGISTER

#define GPIO_BASEADDR_TO_CODE(x) 	((x == GPIOA) ? 0:\
									 (x == GPIOB) ? 1:\
									 (x == GPIOC) ? 2:0)

// IRQ NUMBER MACROS

#define IRQ_NO_EXTI0				6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2				8
#define IRQ_NO_EXTI3				9
#define IRQ_NO_EXTI4				10
#define IRQ_NO_EXTI5_9				23
#define IRQ_NO_EXTI10_15			40



#endif
