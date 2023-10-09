/*												                      	@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx.h						            					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    CMSIS STM32F407xx Device Peripheral 										@@@@@@@@@@@@@@@   @@@@@@@         @@@@@@@@@@@@@@
  		   Access Layer Header File.			       						        @@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							              					@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									               			 	@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&         @@@@@
												                        @@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file contains:								              				@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
- Data structures and the address mapping for 										@@@@@@@@@@@@@@@@@             @@@             @@
  all peripherals								                			@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
- peripherals registers declarations and bits   									@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
  definition									                  			@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
											                        	@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
												                        @@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
												                        @@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
												                        @@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
												                        @@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
												                        @@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
												                        @@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
												                        @@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
												                        @@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
												                        @@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
												                        @@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
												                        @@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/
#ifndef __STM32F407xx_H
#define __STM32F407xx_H


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stddef.h>

#define __weak			__attribute__((weak))

/**
* @brief Configuration of the Cortex-M4 Processor and Core Peripherals
*/
#define __CM4_REV                 0x0001U  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1U       /*!< STM32F4XX provides an MPU                     */
#define __NVIC_PRIO_BITS          4U       /*!< STM32F4XX uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /*!< FPU present                                   */


/**
* @brief STM32F4XX base addresses of flash and SRAM memories
*/
#define FLASH_BASE            0x08000000UL /*!< FLASH(up to 1 MB) base address in the alias region                       */
#define CCMDATARAM_BASE       0x10000000UL /*!< CCM(core coupled memory) data RAM(64 KB) base address in the alias region*/
#define SRAM1_BASE            0x20000000UL /*!< SRAM1(112 KB) base address in the alias region                           */
#define SRAM2_BASE            0x2001C000UL /*!< SRAM2(16 KB) base address in the alias region                            */
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                              */
#define BKPSRAM_BASE          0x40024000UL /*!< Backup SRAM(4 KB) base address in the alias region                       */
#define FSMC_R_BASE           0xA0000000UL /*!< FSMC registers base address                                              */
#define SRAM1_BB_BASE         0x22000000UL /*!< SRAM1(112 KB) base address in the bit-band region                        */
#define SRAM2_BB_BASE         0x22380000UL /*!< SRAM2(16 KB) base address in the bit-band region                         */
#define PERIPH_BB_BASE        0x42000000UL /*!< Peripheral base address in the bit-band region                           */
#define BKPSRAM_BB_BASE       0x42480000UL /*!< Backup SRAM(4 KB) base address in the bit-band region                    */
#define FLASH_END             0x080FFFFFUL /*!< FLASH end address                                                        */
#define FLASH_OTP_BASE        0x1FFF7800UL /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area              */
#define FLASH_OTP_END         0x1FFF7A0FUL /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area               */
#define CCMDATARAM_END        0x1000FFFFUL /*!< CCM data RAM end address                                                 */

/**
* @brief STM32F4XX AHBx and APBx Bus Peripheral base addresses
*/
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                                */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000UL)

/**
* @brief STM32F4XX base addresses of peripherals which are hanging on AHB1 bus
*/
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400UL)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800UL)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00UL)
#define GPIOI_BASE            (AHB1PERIPH_BASE + 0x2000UL)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00UL)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000UL)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010UL)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028UL)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040UL)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058UL)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070UL)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088UL)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0UL)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400UL)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)
#define ETH_BASE              (AHB1PERIPH_BASE + 0x8000UL)
#define ETH_MAC_BASE          (ETH_BASE)
#define ETH_MMC_BASE          (ETH_BASE + 0x0100UL)
#define ETH_PTP_BASE          (ETH_BASE + 0x0700UL)
#define ETH_DMA_BASE          (ETH_BASE + 0x1000UL)

/**
* @brief STM32F4XX base addresses of peripherals which are hanging on AHB1 bus
*/
#define DCMI_BASE             (AHB2PERIPH_BASE + 0x50000UL)
#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800UL)

/**
* @brief STM32F4XX base addresses of peripherals which are hanging on APB1 bus
*/
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000UL)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00UL)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000UL)
#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00UL)
#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800UL)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00UL)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00UL)
#define CAN1_BASE             (APB1PERIPH_BASE + 0x6400UL)
#define CAN2_BASE             (APB1PERIPH_BASE + 0x6800UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400UL)

/**
* @brief STM32F4XX base addresses of peripherals which are hanging on APB2 bus
*/
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400UL)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000UL)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2100UL)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x2200UL)
#define ADC123_COMMON_BASE    (APB2PERIPH_BASE + 0x2300UL)
#define ADC_BASE               ADC123_COMMON_BASE
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)

/**
* @brief FSMC Bankx registers base address
*/
#define FSMC_Bank1_R_BASE     (FSMC_R_BASE + 0x0000UL)
#define FSMC_Bank1E_R_BASE    (FSMC_R_BASE + 0x0104UL)
#define FSMC_Bank2_3_R_BASE   (FSMC_R_BASE + 0x0060UL)
#define FSMC_Bank4_R_BASE     (FSMC_R_BASE + 0x00A0UL)

/**
* @brief Debug MCU registers base address
*/
#define DBGMCU_BASE           0xE0042000UL

/**
* @brief USB registers base address
*/
#define USB_OTG_HS_PERIPH_BASE               0x40040000UL
#define USB_OTG_FS_PERIPH_BASE               0x50000000UL

#define USB_OTG_GLOBAL_BASE                  0x000UL
#define USB_OTG_DEVICE_BASE                  0x800UL
#define USB_OTG_IN_ENDPOINT_BASE             0x900UL
#define USB_OTG_OUT_ENDPOINT_BASE            0xB00UL
#define USB_OTG_EP_REG_SIZE                  0x20UL
#define USB_OTG_HOST_BASE                    0x400UL
#define USB_OTG_HOST_PORT_BASE               0x440UL
#define USB_OTG_HOST_CHANNEL_BASE            0x500UL
#define USB_OTG_HOST_CHANNEL_SIZE            0x20UL
#define USB_OTG_PCGCCTL_BASE                 0xE00UL
#define USB_OTG_FIFO_BASE                    0x1000UL
#define USB_OTG_FIFO_SIZE                    0x1000UL

#define UID_BASE                     0x1FFF7A10UL           /*!< Unique device ID register base address */
#define FLASHSIZE_BASE               0x1FFF7A22UL           /*!< FLASH Size register base address       */
#define PACKAGE_BASE                 0x1FFF7BF0UL           /*!< Package size register base address     */

/**
* @brief Memory mapping of Core Hardware
*/
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define ITM_BASE            (0xE0000000UL)                            /*!< ITM Base Address */
#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address */
#define TPI_BASE            (0xE0040000UL)                            /*!< TPI Base Address */
#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */

/**
* @brief STM32F4XX peripheral declaration
*/
#define TIM2                ((TIM_2_5_Reg_Def_t *) TIM2_BASE)
#define TIM3                ((TIM_2_5_Reg_Def_t *) TIM3_BASE)
#define TIM4                ((TIM_2_5_Reg_Def_t *) TIM4_BASE)
#define TIM5                ((TIM_2_5_Reg_Def_t *) TIM5_BASE)
#define TIM6                ((TIM_6_7_Reg_Def_t *) TIM6_BASE)
#define TIM7                ((TIM_6_7_Reg_Def_t *) TIM7_BASE)
#define TIM12               ((TIM_9_12_Reg_Def_t *) TIM12_BASE)
#define TIM13               ((TIM_10_14_Reg_Def_t *) TIM13_BASE)
#define TIM14               ((TIM_10_14_Reg_Def_t *) TIM14_BASE)
#define RTC                 ((RTC_RegDef_t *) RTC_BASE)
#define WWDG                ((WWDG_RegDef_t *) WWDG_BASE)
#define IWDG                ((IWDG_RegDef_t *) IWDG_BASE)
#define I2S2ext             ((SPI_RegDef_t *) I2S2ext_BASE)
#define SPI1                ((SPI_RegDef_t *) SPI1_BASE)
#define SPI2                ((SPI_RegDef_t *) SPI2_BASE)
#define SPI3                ((SPI_RegDef_t *) SPI3_BASE)
#define SPI4                ((SPI_RegDef_t *) SPI4_BASE)
#define I2S3ext             ((SPI_RegDef_t *) I2S3ext_BASE)
#define USART2              ((USART_RegDef_t *) USART2_BASE)
#define USART3              ((USART_RegDef_t *) USART3_BASE)
#define UART4               ((USART_RegDef_t *) UART4_BASE)
#define UART5               ((USART_RegDef_t *) UART5_BASE)
#define I2C1                ((I2C_RegDef_t *) I2C1_BASE)
#define I2C2                ((I2C_RegDef_t *) I2C2_BASE)
#define I2C3                ((I2C_RegDef_t *) I2C3_BASE)
#define CAN1                ((CAN_RegDef_t *) CAN1_BASE)
#define CAN2                ((CAN_RegDef_t *) CAN2_BASE)
#define PWR                 ((PWR_RegDef_t *) PWR_BASE)
#define DAC1                ((DAC_RegDef_t *) DAC_BASE)
#define DAC                 ((DAC_RegDef_t *) DAC_BASE) /* Kept for legacy purpose */
#define TIM1                ((TIM_1_8_Reg_Def_t *) TIM1_BASE)
#define TIM8                ((TIM_1_8_Reg_Def_t *) TIM8_BASE)
#define USART1              ((USART_RegDef_t *) USART1_BASE)
#define USART6              ((USART_RegDef_t *) USART6_BASE)
#define ADC1                ((ADC_RegDef_t *) ADC1_BASE)
#define ADC2                ((ADC_RegDef_t *) ADC2_BASE)
#define ADC3                ((ADC_RegDef_t *) ADC3_BASE)
#define ADC123_COMMON       ((ADC_Common_RegDef_t *) ADC123_COMMON_BASE)
#define SDIO                ((SDIO_RegDef_t *) SDIO_BASE)
#define SYSCFG              ((SYSCFG_RegDef_t *) SYSCFG_BASE)
#define EXTI                ((EXTI_RegDef_t *) EXTI_BASE)
#define TIM9                ((TIM_9_12_Reg_Def_t *) TIM9_BASE)
#define TIM10               ((TIM_10_14_Reg_Def_t *) TIM10_BASE)
#define TIM11               ((TIM_11_Reg_Def_t *) TIM11_BASE)
#define GPIOA               ((GPIO_RegDef_t *) GPIOA_BASE)
#define GPIOB               ((GPIO_RegDef_t *) GPIOB_BASE)
#define GPIOC               ((GPIO_RegDef_t *) GPIOC_BASE)
#define GPIOD               ((GPIO_RegDef_t *) GPIOD_BASE)
#define GPIOE               ((GPIO_RegDef_t *) GPIOE_BASE)
#define GPIOF               ((GPIO_RegDef_t *) GPIOF_BASE)
#define GPIOG               ((GPIO_RegDef_t *) GPIOG_BASE)
#define GPIOH               ((GPIO_RegDef_t *) GPIOH_BASE)
#define GPIOI               ((GPIO_RegDef_t *) GPIOI_BASE)
#define CRC                 ((CRC_RegDef_t *) CRC_BASE)
#define RCC                 ((RCC_RegDef_t *) RCC_BASE)
#define FLASH               ((FLASH_RegDef_t *) FLASH_R_BASE)
#define DMA1                ((DMA_RegDef_t *) DMA1_BASE)
#define DMA1_Stream0        ((DMA_Stream_RegDef_t *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_RegDef_t *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_RegDef_t *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_RegDef_t *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_RegDef_t *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_RegDef_t *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_RegDef_t *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_RegDef_t *) DMA1_Stream7_BASE)
#define DMA2                ((DMA_RegDef_t *) DMA2_BASE)
#define DMA2_Stream0        ((DMA_Stream_RegDef_t *) DMA2_Stream0_BASE)
#define DMA2_Stream1        ((DMA_Stream_RegDef_t *) DMA2_Stream1_BASE)
#define DMA2_Stream2        ((DMA_Stream_RegDef_t *) DMA2_Stream2_BASE)
#define DMA2_Stream3        ((DMA_Stream_RegDef_t *) DMA2_Stream3_BASE)
#define DMA2_Stream4        ((DMA_Stream_RegDef_t *) DMA2_Stream4_BASE)
#define DMA2_Stream5        ((DMA_Stream_RegDef_t *) DMA2_Stream5_BASE)
#define DMA2_Stream6        ((DMA_Stream_RegDef_t *) DMA2_Stream6_BASE)
#define DMA2_Stream7        ((DMA_Stream_RegDef_t *) DMA2_Stream7_BASE)
#define ETH                 ((ETH_RegDef_t *) ETH_BASE)
#define DCMI                ((DCMI_RegDef_t *) DCMI_BASE)
#define RNG                 ((RNG_RegDef_t *) RNG_BASE)
#define FSMC_Bank1          ((FSMC_Bank1_RegDef_t *) FSMC_Bank1_R_BASE)
#define FSMC_Bank1E         ((FSMC_Bank1E_RegDef_t *) FSMC_Bank1E_R_BASE)
#define FSMC_Bank2_3        ((FSMC_Bank2_3_RegDef_t *) FSMC_Bank2_3_R_BASE)
#define FSMC_Bank4          ((FSMC_Bank4_RegDef_t *) FSMC_Bank4_R_BASE)
#define DBGMCU              ((DBGMCU_RegDef_t *) DBGMCU_BASE)
#define USB_OTG_FS          ((USB_OTG_FS_Reg_t *) USB_OTG_FS_PERIPH_BASE)
#define USB_OTG_HS          ((USB_OTG_HS_Reg_t *) USB_OTG_HS_PERIPH_BASE)

#define NVIC_IPR_BASE					 ((volatile uint32_t *)0xE000E400)

/**
* @brief STM32F4XX Interrupt Number Definition, according to the selected device
*        in @ref Library_configuration_section
*/
typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare global interrupt                             */
DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                             */
SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
RNG_IRQn                    = 80,     /*!< RNG global Interrupt                                              */
FPU_IRQn                    = 81      /*!< FPU global interrupt                                               */
}IRQn_Type;


typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus_t, ITStatus_t;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
}FunctionalState_t;

/****************************************** Peripheral register definition structures ******************************************/
/*
* Note: Registers of a peripheral are specific to MCU
*/

/**
  * @brief Analog to Digital Converter
  */

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t awd			: 1 ;	/*!< [Bit 0]     Analog watchdog flag         	    				*/
		volatile uint32_t eoc			: 1 ;	/*!< [Bit 1]     Regular channel end of conversion         	    	*/
		volatile uint32_t jeoc			: 1 ;	/*!< [Bit 2]     Injected channel end of conversion         	    	*/
		volatile uint32_t jstrt			: 1 ;	/*!< [Bit 3]     Injected channel start flag		         	    	*/
		volatile uint32_t strt			: 1 ;	/*!< [Bit 4]     Regular channel start flag		         	    	*/
		volatile uint32_t ovr			: 1 ;	/*!< [Bit 5]     Overrun		         	    						*/
				  uint32_t reserved		: 2 ;	/*!< [Bits 31:6] Reserved, must be kept at reset value          		*/
		  }bit;
}ADC_SR_Reg_t;

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t awdch 		: 5 ;	/*!< [Bits 4:0]   Analog watchdog channel select bits         	    */
		volatile uint32_t eocie 		: 1 ;	/*!< [Bit 5]      Interrupt enable for EOC			  				*/
		volatile uint32_t awdie 		: 1 ;	/*!< [Bit 6]      Analog watchdog interrupt enable			  		*/
		volatile uint32_t jeocie 		: 1 ;	/*!< [Bit 7]      Interrupt enable for injected channels  			*/
		volatile uint32_t scan 			: 1 ;	/*!< [Bit 8]      Scan mode  										*/
		volatile uint32_t awdsgl 		: 1 ;	/*!< [Bit 9]      Enable the watchdog on a single channel in scan mode*/
		volatile uint32_t jauto 		: 1 ;	/*!< [Bit 10]     Automatic injected group conversion 				*/
		volatile uint32_t discen 		: 1 ;	/*!< [Bit 11]     Discontinuous mode on regular channels         	*/
		volatile uint32_t jdiscen 		: 1 ;	/*!< [Bit 12]     Discontinuous mode on injected channels         	*/
		volatile uint32_t discnum 		: 3 ;	/*!< [Bits 15:13] Discontinuous mode channel count         	    	*/
				  uint32_t reserved_0 	: 6 ;	/*!< [Bits 21:16] Reserved, must be kept at reset value          	*/
		volatile uint32_t jawden 		: 1 ;	/*!< [Bit 22]     Analog watchdog enable on injected channels       */
		volatile uint32_t awden 		: 1 ;	/*!< [Bit 23]     Analog watchdog enable on regular channels        */
		volatile uint32_t res 			: 2 ;	/*!< [Bits 25:24] Resolution                              			*/
		volatile uint32_t ovrie 		: 1 ;	/*!< [Bit 26] 	  Overrun interrupt enable                          */
		 	 	  uint32_t reserved_1 	: 5 ;	/*!< [Bits 31:27] Reserved, must be kept at reset value             */
		  }bit;
}ADC_CR1_Reg_t;

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t adon 			: 1 ;	/*!< [Bit 0] 	  A/D Converter ON / OFF                                 */
		volatile uint32_t cont 			: 1 ;	/*!< [Bit 1] 	  Continuous conversion                                  */
				  uint32_t reserved_0 	: 6 ;	/*!< [Bits 7:2]   Reserved, must be kept at reset value                  */
		volatile uint32_t dma 			: 1 ;	/*!< [Bit 8] 	  Direct memory access mode (for single ADC mode)   	 */
		volatile uint32_t dds 			: 1 ;	/*!< [Bit 9] 	  DMA disable selection (for single ADC mode)   	 	 */
		volatile uint32_t eocs	 		: 1 ;	/*!< [Bit 10] 	  End of conversion selection 							 */
		volatile uint32_t align 		: 1 ;	/*!< [Bit 11] 	  Data alignment		 								 */
				  uint32_t reserved_1	: 4 ;	/*!< [Bits 15:12] Reserved, must be kept at reset value                	 */
		volatile uint32_t jextsel 		: 4 ;	/*!< [Bits 19:16] External event select for injected group               */
		volatile uint32_t jexten 		: 2 ;	/*!< [Bits 21:20] External trigger enable for injected channels          */
		volatile uint32_t jswstart 		: 1 ;	/*!< [Bit 22] 	  Start conversion of injected channels		 			 */
				  uint32_t reserved_2	: 1 ;	/*!< [Bit 23] 	  Reserved, must be kept at reset value                	 */
		volatile uint32_t extsel		: 4 ;	/*!< [Bits 27:24] External event select for regular group        		 */
		volatile uint32_t exten			: 2 ;	/*!< [Bits 29:28] External trigger enable for regular channels        	 */
		volatile uint32_t swstart 		: 1 ;	/*!< [Bit 30] 	  Start conversion of regular channels           	 	 */
                  uint32_t reserved_3 	: 1 ;	/*!< [Bit 31] 	  Reserved, must be kept at reset value                	 */
		  }bit;
}ADC_CR2_Reg_t;

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t smp_10 		: 3 ;	/*!< [Bits 26:0]  Channel x sampling time selection        	 			 */
		volatile uint32_t smp_11 		: 3 ;
		volatile uint32_t smp_12 		: 3 ;
		volatile uint32_t smp_13 		: 3 ;
		volatile uint32_t smp_14 		: 3 ;
		volatile uint32_t smp_15 		: 3 ;
		volatile uint32_t smp_16 		: 3 ;
		volatile uint32_t smp_17 		: 3 ;
		volatile uint32_t smp_18 		: 3 ;
				  uint32_t reserved		: 5 ;	/*!< [Bits 31:27] Reserved, must be kept at reset value                	 */
		  }bit;
}ADC_SMPR1_Reg_t;

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t smp_0 		: 3 ;	/*!< [Bits 29:0]  Channel x sampling time selection        	 			 */
		volatile uint32_t smp_1 		: 3 ;
		volatile uint32_t smp_2 		: 3 ;
		volatile uint32_t smp_3 		: 3 ;
		volatile uint32_t smp_4 		: 3 ;
		volatile uint32_t smp_5 		: 3 ;
		volatile uint32_t smp_6 		: 3 ;
		volatile uint32_t smp_7 		: 3 ;
		volatile uint32_t smp_8 		: 3 ;
		volatile uint32_t smp_9 		: 3 ;
				  uint32_t reserved		: 2 ;	/*!< [Bits 31:30] Reserved, must be kept at reset value                	 */
		  }bit;
}ADC_SMPR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t joffset 		: 12 ; 	/*!< [Bits 11:0]   Data offset for injected channel x 		 			*/
        	      uint32_t reserved     : 20 ;   /*!< [Bits 31:12] Reserved, must be kept at reset value 	 			*/
    	   }bit;
}ADC_JOFRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ht 			: 12 ;   /*!< [Bits 11:0]  Analog watchdog higher threshold 						*/
                  uint32_t reserved 	: 20 ;   /*!< [Bits 31:12] Reserved, must be kept at reset value 				*/
    	  }bit;
}ADC_HTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ltr 			: 12 ;   /*!< [Bits 11:0]  Analog watchdog lower threshold 						*/
                  uint32_t reserved 	: 20 ;   /*!< [Bits 31:12] Reserved, must be kept at reset value 				*/
    	   }bit;
}ADC_LTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sq13 			: 5 ;    /*!< [Bits 4:0]   13th conversion in regular sequence 					*/
        volatile uint32_t sq14 			: 5 ;    /*!< [Bits 9:5]   14th conversion in regular sequence 					*/
        volatile uint32_t sq15 			: 5 ;    /*!< [Bits 14:10] 15th conversion in regular sequence 					*/
        volatile uint32_t sq16 			: 5 ;    /*!< [Bits 19:15] 16th conversion in regular sequence 					*/
        volatile uint32_t l 			: 4 ;    /*!< [Bits 23:20] Regular channel sequence length 						*/
        		  uint32_t reserved 	: 8 ;    /*!< [Bits 31:24] Reserved, must be kept at reset value 				*/
    	  }bit;
}ADC_SQR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sq7 			: 5 ;     /*!< [Bits 4:0]   7th conversion in regular sequence 					*/
        volatile uint32_t sq8 			: 5 ;     /*!< [Bits 9:5] 	8th conversion in regular sequence 					*/
        volatile uint32_t sq9 			: 5 ;     /*!< [Bits 14:10] 9th conversion in regular sequence 					*/
        volatile uint32_t sq10 			: 5 ;     /*!< [Bits 19:15] 10th conversion in regular sequence 					*/
        volatile uint32_t sq11 			: 5 ;     /*!< [Bits 24:20] 11th conversion in regular sequence 					*/
        volatile uint32_t sq12 			: 5 ;     /*!< [Bits 29:25] 12th conversion in regular sequence 					*/
                  uint32_t reserved 	: 2 ;     /*!< [Bits 31:30] Reserved, must be kept at reset value 				*/
    	   }bit;
}ADC_SQR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sq1 			: 5	;     /*!< [Bits 4:0] 	1st conversion in regular sequence 					*/
        volatile uint32_t sq2 			: 5	;     /*!< [Bits 9:5] 	2nd conversion in regular sequence 					*/
        volatile uint32_t sq3 			: 5	;     /*!< [Bits 14:10] 3rd conversion in regular sequence 					*/
        volatile uint32_t sq4 			: 5	;     /*!< [Bits 19:15] 4th conversion in regular sequence 					*/
        volatile uint32_t sq5 			: 5	;     /*!< [Bits 24:20] 5th conversion in regular sequence 					*/
        volatile uint32_t sq6 			: 5	;     /*!< [Bits 29:25] 6th conversion in regular sequence 					*/
        		  uint32_t reserved 	: 2	;     /*!< [Bits 31:30] Reserved, must be kept at reset value 				*/
    	   }bit;
}ADC_SQR3_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t jsql 			:  5 ;    /*!< [Bits 4:0] 	1st conversion in injected sequence 				*/
        volatile uint32_t jsq2 			:  5 ;    /*!< [Bits 9:5] 	2nd conversion in injected sequence 				*/
        volatile uint32_t jsq3 			:  5 ;    /*!< [Bits 14:10] 3rd conversion in injected sequence 					*/
        volatile uint32_t jsq4 			:  5 ;    /*!< [Bits 19:15] 4th conversion in injected sequence 					*/
        		  uint32_t reserved 	: 12 ;    /*!< [Bits 31:20] Reserved, must be kept at reset value 				*/
    	  }bit;
}ADC_JSQR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t jdata 		: 16 ;    /*!< [Bits 15:0]  Injected data 										*/
        		  uint32_t reserved 	: 16 ;    /*!< [Bits 31:16] Reserved, must be kept at reset value 				*/
    }bit;
}ADC_JDRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t  data 		: 16 ;    /*!< [Bits 15:0]  Regular data 										*/
        		  uint32_t reserved 	: 16 ;    /*!< [Bits 31:16] Reserved, must be kept at reset value 				*/
    }bit;
}ADC_DR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t awd1 			:  1 ;    /*!< [Bit 0] 		Analog watchdog flag of ADC1 						*/
    	volatile uint32_t eoc1 			:  1 ;    /*!< [Bit 1] 		End of conversion of ADC1 							*/
    	volatile uint32_t jeoc1 		:  1 ;    /*!< [Bit 2] 		Injected channel end of conversion of ADC1 			*/
    	volatile uint32_t jstrt1 		:  1 ;    /*!< [Bit 3] 		Injected channel Start flag of ADC1 				*/
    	volatile uint32_t strt1 		:  1 ;    /*!< [Bit 4] 		Regular channel Start flag of ADC1 					*/
    	volatile uint32_t ovr1 			:  1 ;    /*!< [Bit 5] 		Overrun flag of ADC1 								*/
    			  uint32_t reserved_0 	:  2 ;    /*!< [Bits 7:6] 	Reserved, must be kept at reset value 				*/

    	volatile uint32_t awd2 			:  1 ;    /*!< [Bit 8] 		Analog watchdog flag of ADC2 						*/
    	volatile uint32_t eoc2 			:  1 ;    /*!< [Bit 9] 		End of conversion of ADC2 							*/
    	volatile uint32_t jeoc2 		:  1 ;    /*!< [Bit 10] 	Injected channel end of conversion of ADC2 				*/
    	volatile uint32_t jstrt2 		:  1 ;    /*!< [Bit 11] 	Injected channel Start flag of ADC2 					*/
    	volatile uint32_t strt2 		:  1 ;    /*!< [Bit 12] 	Regular channel Start flag of ADC2 						*/
    	volatile uint32_t ovr2 			:  1 ;    /*!< [Bit 13] 	Overrun flag of ADC2 									*/
    			  uint32_t reserved_1 	:  2 ;    /*!< [Bits 15:14] Reserved, must be kept at reset value 				*/

    	volatile uint32_t awd3 			:  1 ;    /*!< [Bit 16] 	Analog watchdog flag of ADC3 							*/
    	volatile uint32_t eoc3 			:  1 ;    /*!< [Bit 17] 	End of conversion of ADC3 								*/
    	volatile uint32_t jeoc3 		:  1 ;    /*!< [Bit 18] 	Injected channel end of conversion of ADC3 				*/
    	volatile uint32_t jstrt3 		:  1 ;    /*!< [Bit 19] 	Injected channel Start flag of ADC3 					*/
    	volatile uint32_t strt3 		:  1 ;    /*!< [Bit 20] 	Regular channel Start flag of ADC3 						*/
    	volatile uint32_t ovr3 			:  1 ;    /*!< [Bit 21] 	Overrun flag of ADC3 									*/
    			  uint32_t reserved_2 	: 10 ;    /*!< [Bits 31:22] Reserved, must be kept at reset value 				*/
    	   }bit;
}ADC_CSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t multi 		: 5	;     /*!< [Bits 4:0] 	Multi ADC mode selection 							*/
    			  uint32_t reserved_0 	: 3	; 	  /*!< [Bits 7:5] 	Reserved, must be kept at reset value 				*/
    	volatile uint32_t delay 		: 4	;     /*!< [Bits 11:8] 	Delay between 2 sampling phases 					*/
    	volatile uint32_t dds 			: 1	;     /*!< [Bit 13] 	DMA disable selection (for multi-ADC mode) 				*/
    	volatile uint32_t dma 			: 2	;     /*!< [Bits 15:14] Direct memory access mode for multi ADC mode 		*/
    	volatile uint32_t adcpres 		: 2	;     /*!< [Bits 17:16] ADC prescaler 										*/
    	volatile uint32_t reserved_1 	: 2	; 	  /*!< [Bits 19:18] Reserved, must be kept at reset value 				*/
    	volatile uint32_t vbate 		: 1	;     /*!< [Bit 22] 	VBAT enable 											*/
    	volatile uint32_t tsvrefe 		: 1	;     /*!< [Bit 23] 	Temperature sensor and VREFINT enable 					*/
    			  uint32_t reserved_2 	: 8	; 	  /*!< [Bits 31:24] Reserved, must be kept at reset value 				*/
    	   }bit;
}ADC_CCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t data1 		: 16 ;    /*!< [Bits 15:0] 	1st data item of a pair of regular conversions 		*/
    	volatile uint32_t data2 		: 16 ;    /*!< [Bits 31:16] 2nd data item of a pair of regular conversions 		*/
    }bit;
}ADC_CDR_Reg_t;

typedef struct
{
ADC_SR_Reg_t     SR;     /*!< ADC status register,                         Address offset: 0x00 */
ADC_CR1_Reg_t    CR1;    /*!< ADC control register 1,                      Address offset: 0x04 */
ADC_CR2_Reg_t 	 CR2;    /*!< ADC control register 2,                      Address offset: 0x08 */
ADC_SMPR1_Reg_t  SMPR1;  /*!< ADC sample time register 1,                  Address offset: 0x0C */
ADC_SMPR2_Reg_t  SMPR2;  /*!< ADC sample time register 2,                  Address offset: 0x10 */
ADC_JOFRx_Reg_t  JOFR1;  /*!< ADC injected channel data offset register 1, Address offset: 0x14 */
ADC_JOFRx_Reg_t  JOFR2;  /*!< ADC injected channel data offset register 2, Address offset: 0x18 */
ADC_JOFRx_Reg_t  JOFR3;  /*!< ADC injected channel data offset register 3, Address offset: 0x1C */
ADC_JOFRx_Reg_t  JOFR4;  /*!< ADC injected channel data offset register 4, Address offset: 0x20 */
ADC_HTR_Reg_t 	 HTR;    /*!< ADC watchdog higher threshold register,      Address offset: 0x24 */
ADC_LTR_Reg_t    LTR;    /*!< ADC watchdog lower threshold register,       Address offset: 0x28 */
ADC_SQR1_Reg_t   SQR1;   /*!< ADC regular sequence register 1,             Address offset: 0x2C */
ADC_SQR2_Reg_t   SQR2;   /*!< ADC regular sequence register 2,             Address offset: 0x30 */
ADC_SQR3_Reg_t   SQR3;   /*!< ADC regular sequence register 3,             Address offset: 0x34 */
ADC_JSQR_Reg_t   JSQR;   /*!< ADC injected sequence register,              Address offset: 0x38*/
ADC_JDRx_Reg_t 	 JDR1;   /*!< ADC injected data register 1,                Address offset: 0x3C */
ADC_JDRx_Reg_t 	 JDR2;   /*!< ADC injected data register 2,                Address offset: 0x40 */
ADC_JDRx_Reg_t 	 JDR3;   /*!< ADC injected data register 3,                Address offset: 0x44 */
ADC_JDRx_Reg_t 	 JDR4;   /*!< ADC injected data register 4,                Address offset: 0x48 */
ADC_DR_Reg_t 	 DR;     /*!< ADC regular data register,                   Address offset: 0x4C */
}ADC_RegDef_t;

typedef struct
{
ADC_CSR_Reg_t    CSR;    /*!< ADC Common status register,                  Address offset: ADC1 base address + 0x300 */
ADC_CCR_Reg_t 	 CCR;    /*!< ADC common control register,                 Address offset: ADC1 base address + 0x304 */
ADC_CDR_Reg_t    CDR;    /*!< ADC common regular data register for dual
                             AND triple modes,                            Address offset: ADC1 base address + 0x308 */
}ADC_Common_RegDef_t;


/**
  * @brief Controller Area Network
  */

typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t inak       	:  1 ;    /*!< [Bit 0] 		Initialization acknowledge 							*/
    	volatile uint32_t slak       	:  1 ;    /*!< [Bit 1] 		Sleep acknowledge 									*/
    	volatile uint32_t erri       	:  1 ;    /*!< [Bit 2] 		Error interrupt 									*/
    	volatile uint32_t wkui       	:  1 ;    /*!< [Bit 3] 		Wakeup interrupt 									*/
    	volatile uint32_t slaki      	:  1 ;    /*!< [Bit 4] 		Sleep acknowledge interrupt 						*/
    			  uint32_t reserved_0  	:  3 ;    /*!< [Bits 7:5] 	Reserved, must be kept at reset value 				*/
    	volatile uint32_t txm        	:  1 ;    /*!< [Bit 8] 		Transmit mode 										*/
    	volatile uint32_t rxm        	:  1 ;    /*!< [Bit 9] 		Receive mode 										*/
    	volatile uint32_t samp       	:  1 ;    /*!< [Bit 10] 	Last sample point 										*/
    	volatile uint32_t rx         	:  1 ;    /*!< [Bit 11] 	CAN Rx signal 											*/
    			  uint32_t reserved_1   : 20 ;    /*!< [Bits 31:12] Reserved, must be kept at reset value 				*/
    	  }bit;
}CAN_MCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t inak     		:  1 ;    /*!< [Bit 0] 		Initialization acknowledge 							*/
        volatile uint32_t slak     		:  1 ;    /*!< [Bit 1] 		Sleep acknowledge 									*/
        volatile uint32_t erri     		:  1 ;    /*!< [Bit 2] 		Error interrupt 									*/
        volatile uint32_t wkui     		:  1 ;    /*!< [Bit 3] 		Wakeup interrupt 									*/
        volatile uint32_t slaki    		:  1 ;    /*!< [Bit 4] 		Sleep acknowledge interrupt 						*/
        		  uint32_t reserved_0 	:  3 ;    /*!< [Bits 7:5] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t txm      		:  1 ;    /*!< [Bit 8] 		Transmit mode 										*/
        volatile uint32_t rxm      		:  1 ;    /*!< [Bit 9] 		Receive mode 										*/
        volatile uint32_t samp     		:  1 ;    /*!< [Bit 10] 	Last sample point 										*/
        volatile uint32_t rx       		:  1 ;    /*!< [Bit 11] 	CAN Rx signal 											*/
        		  uint32_t reserved_1 	: 20 ; 	  /*!< [Bits 31:12] Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_MSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t rqcp0    		: 1 ;   /*!< [Bit 0] 		Request completed mailbox0 							*/
        volatile uint32_t txok0    		: 1 ;   /*!< [Bit 1] 		Transmission OK of mailbox0 						*/
        volatile uint32_t alst0    		: 1 ;   /*!< [Bit 2] 		Arbitration lost for mailbox0 						*/
        volatile uint32_t terr0    		: 1 ;   /*!< [Bit 3] 		Transmission error of mailbox0 						*/
        		  uint32_t reserved_0 	: 3 ;   /*!< [Bits 6:4] 	Reserved, must be kept at reset value 					*/
        volatile uint32_t abrq0    		: 1 ;  	/*!< [Bit 7] 		Abort request for mailbox0 							*/
        volatile uint32_t rqcp1    		: 1 ;   /*!< [Bit 8] 		Request completed mailbox1 							*/
        volatile uint32_t txok1    		: 1 ;   /*!< [Bit 9] 		Transmission OK of mailbox1 						*/
        volatile uint32_t alst1    		: 1 ;   /*!< [Bit 10] 		Arbitration lost for mailbox1 						*/
        volatile uint32_t terr1    		: 1 ;   /*!< [Bit 11] 		Transmission error of mailbox1 						*/
        volatile uint32_t abrq1    		: 1 ;   /*!< [Bit 15] 		Abort request for mailbox1 							*/
        volatile uint32_t rqcp2    		: 1 ;   /*!< [Bit 16] 		Request completed mailbox2 							*/
        volatile uint32_t txok2    		: 1 ;   /*!< [Bit 17] 		Transmission OK of mailbox2 						*/
        volatile uint32_t alst2    		: 1 ;   /*!< [Bit 18] 		Arbitration lost for mailbox2 						*/
        volatile uint32_t terr2    		: 1 ;   /*!< [Bit 19] 		Transmission error of mailbox2 						*/
        		  uint32_t reserved_1 	: 3 ; 	/*!< [Bits 22:20] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t code     		: 2 ;   /*!< [Bits 25:24] 	Mailbox code 										*/
        volatile uint32_t tme0     		: 1 ;   /*!< [Bit 26] 		Transmit mailbox 0 empty 							*/
        volatile uint32_t tme1     		: 1 ;   /*!< [Bit 27] 		Transmit mailbox 1 empty 							*/
        volatile uint32_t tme2     		: 1 ;   /*!< [Bit 28] 		Transmit mailbox 2 empty 							*/
        volatile uint32_t low0     		: 1 ;   /*!< [Bit 29] 		Lowest priority flag for mailbox 0 					*/
        volatile uint32_t low1     		: 1 ;   /*!< [Bit 30] 		Lowest priority flag for mailbox 1 					*/
        volatile uint32_t low2     		: 1 ;   /*!< [Bit 31] 		Lowest priority flag for mailbox 2 					*/
    	   }bit;
}CAN_TSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fmp0     		:  2 ;  /*!< [Bits 1:0] 	FIFO 0 message pending 									*/
        	  	  uint32_t reserved_0 	:  1 ; 	/*!< [Bit 2] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t full0    		:  1 ;  /*!< [Bit 3] 		FIFO 0 full 										*/
        volatile uint32_t fovr0    		:  1 ;  /*!< [Bit 4] 		FIFO 0 overrun 										*/
        volatile uint32_t rfom0    		:  1 ;  /*!< [Bit 5] 		Release FIFO 0 output mailbox 						*/
        		  uint32_t reserved_1 	: 26 ; 	/*!< [Bits 31:6] 	Reserved, must be kept at reset value 				*/
    	  }bit;
}CAN_RF0R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fmp1     		:  2 ;  /*!< [Bits 1:0] 	FIFO 1 message pending 									*/
        		  uint32_t reserved_0 	:  1 ; 	/*!< [Bit 2] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t full1    		:  1 ;  /*!< [Bit 3] 		FIFO 1 full 										*/
        volatile uint32_t fovr1   		:  1 ;  /*!< [Bit 4] 		FIFO 1 overrun 										*/
        volatile uint32_t rfom1    		:  1 ;  /*!< [Bit 5] 		Release FIFO 1 output mailbox 						*/
        		  uint32_t reserved_1 	: 26 ; 	/*!< [Bits 31:6] 	Reserved, must be kept at reset value 				*/
    	  }bit;
}CAN_RF1R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tmeie    		:  1 ;  /*!< [Bit 0] 		Transmit mailbox empty interrupt enable 			*/
        volatile uint32_t fmpie0   		:  1 ;  /*!< [Bit 1] 		FIFO message pending interrupt enable for FIFO0 	*/
        volatile uint32_t ffie0    		:  1 ;  /*!< [Bit 2] 		FIFO full interrupt enable for FIFO0 				*/
        volatile uint32_t fovie0   		:  1 ;  /*!< [Bit 3] 		FIFO overrun interrupt enable for FIFO0 			*/
        volatile uint32_t fmpie1   		:  1 ;  /*!< [Bit 4] 		FIFO message pending interrupt enable for FIFO1 	*/
        volatile uint32_t ffie1    		:  1 ;  /*!< [Bit 5] 		FIFO full interrupt enable for FIFO1 				*/
        volatile uint32_t fovie1   		:  1 ;  /*!< [Bit 6] 		FIFO overrun interrupt enable for FIFO1 			*/
        		  uint32_t reserved_0 	:  1 ; 	/*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t ewgie    		:  1 ;  /*!< [Bit 8] 		Error warning interrupt enable 						*/
        volatile uint32_t epvie    		:  1 ;  /*!< [Bit 9] 		Error passive interrupt enable 						*/
        volatile uint32_t bofie    		:  1 ;  /*!< [Bit 10] 		Bus-off interrupt enable 							*/
        volatile uint32_t lecie    		:  1 ;  /*!< [Bit 11] 		Last error code interrupt enable 					*/
        		  uint32_t reserved_1 	:  3 ; 	/*!< [Bits 14:12] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t errie    		:  1 ;  /*!< [Bit 15] 		Error interrupt enable 								*/
        volatile uint32_t wkuie    		:  1 ;  /*!< [Bit 16] 		Wakeup interrupt enable 							*/
        volatile uint32_t slkie    		:  1 ;  /*!< [Bit 17] 		Sleep interrupt enable 								*/
        	      uint32_t reserved_2 	: 14 ; 	/*!< [Bits 31:18] 	Reserved, must be kept at reset value 				*/
    	  }bit;
}CAN_IER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ewgf     		: 1 ;  	/*!< [Bit 0] 		Error warning flag 									*/
        volatile uint32_t epvf     		: 1 ;  	/*!< [Bit 1] 		Error passive flag 									*/
        volatile uint32_t boff     		: 1 ;  	/*!< [Bit 2] 		Bus-off flag 										*/
         	 	  uint32_t reserved_0 	: 1 ; 	/*!< [Bit 3] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t lec      		: 3 ;  	/*!< [Bits 6:4] 	Last error code 										*/
        		  uint32_t reserved_1 	: 1 ; 	/*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t tec      		: 8 ;  	/*!< [Bits 15:8] 	Least significant byte of the 9-bit transmit error counter */
        volatile uint32_t rec      		: 8 ;  	/*!< [Bits 23:16] 	Receive error counter 								*/
        		  uint32_t reserved_2 	: 8 ; 	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 				*/
    	 }bit;
}CAN_ESR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t brp     		: 10 ; 	/*!< [Bits 9:0] 	Baud rate prescaler 									*/
        		  uint32_t reserved_0 	:  6 ; 	/*!< [Bits 15:10] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t ts1     		:  4 ;  /*!< [Bits 19:16] 	Time segment 1 										*/
        volatile uint32_t ts2     		:  3 ;  /*!< [Bits 22:20] 	Time segment 2 										*/
        		  uint32_t reserved_1 	:  1 ; 	/*!< [Bit 23] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t sjw     		:  2 ;  /*!< [Bits 25:24] 	Resynchronization jump width 						*/
        		  uint32_t reserved_2 	:  4 ; 	/*!< [Bits 29:26] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t lbkm    		:  1 ;  /*!< [Bit 30] 		Loop back mode (debug) 								*/
        volatile uint32_t silm    		:  1 ;  /*!< [Bit 31] 		Silent mode (debug) 								*/
    	   }bit;
}CAN_BTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t txrq    		:  1 ;  /*!< [Bit 31] 		Transmit mailbox request 							*/
    	volatile uint32_t rtr     		:  1 ;  /*!< [Bit 30] 		Remote Transmission Request 						*/
    	volatile uint32_t ide     		:  1 ;  /*!< [Bit 29] 		Identifier Extension 								*/
    	volatile uint32_t exid    		: 18 ; 	/*!< [Bits 28:11] 	Extended Identifier 								*/
        volatile uint32_t stid    		: 11 ; 	/*!< [Bits 10:0] 	Standard Identifier 								*/
    }bit;
}CAN_TIxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dlc     		:  4 ;  /*!< [Bits 3:0] 	Data Length Code 										*/
        		  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 7:4] 	Reserved, must be kept at reset value 					*/
        volatile uint32_t tgt 			:  1 ; 	/*!< [Bit 8] 		Transmit global time 								*/
        		  uint32_t reserved_1 	:  7 ; 	/*!< [Bits 15:9] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t time 			: 16 ;	/*!< [Bits 31:16] 	Message time stamp 									*/
    	   }bit;
}CAN_TDTxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t data0   		: 8 ;  	/*!< [Bits 7:0] 	Data Byte 0 											*/
        volatile uint32_t data1   		: 8 ;  	/*!< [Bits 15:8] 	Data Byte 1 										*/
        volatile uint32_t data2   		: 8 ;  	/*!< [Bits 23:16] 	Data Byte 2 										*/
        volatile uint32_t data3   		: 8 ;  	/*!< [Bits 31:24] 	Data Byte 3 										*/
    	   }bit;
}CAN_TDLxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t data4   		: 8 ;  	/*!< [Bits 7:0] 	Data Byte 4 											*/
        volatile uint32_t data5   		: 8 ;  	/*!< [Bits 15:8] 	Data Byte 5 										*/
        volatile uint32_t data6   		: 8 ;  	/*!< [Bits 23:16] 	Data Byte 6 										*/
        volatile uint32_t data7   		: 8 ;  	/*!< [Bits 31:24] 	Data Byte 7 										*/
    	   }bit;
}CAN_TDHxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    			  uint32_t reserved 	:  1 ; 	/*!< [Bit 0] 		Reserved, must be kept at reset value 				*/
    	volatile uint32_t rtr     		:  1 ;  /*!< [Bit 1] 		Remote Transmission Request 						*/
    	volatile uint32_t ide     		:  1 ;  /*!< [Bit 2] 		Identifier Extension 								*/
    	volatile uint32_t exid    		: 18 ; 	/*!< [Bits 20:3] 	Extended Identifier 								*/
        volatile uint32_t stid	    	: 11 ; 	/*!< [Bits 31:21] 	Standard Identifier or Extended Identifier			*/
    	   }bit;
}CAN_RIxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dlc     		:  4 ;  /*!< [Bits 3:0] 	Data Length Code 										*/
        		  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 7:4] 	Reserved, must be kept at reset value 					*/
        volatile uint32_t fmi     		:  8 ;  /*!< [Bits 15:8] 	Filter Match Index 									*/
        		  uint32_t reserved_1 	: 16 ; 	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_RDTxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t data0   		: 8 ;  	/*!< [Bits 7:0] 	Data Byte 0 											*/
        volatile uint32_t data1   		: 8 ;  	/*!< [Bits 15:8] 	Data Byte 1 										*/
        volatile uint32_t data2   		: 8 ;  	/*!< [Bits 23:16] 	Data Byte 2 										*/
        volatile uint32_t data3   		: 8 ;  	/*!< [Bits 31:24] 	Data Byte 3 										*/
    	   }bit;
}CAN_RDLxR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t data4   		: 8 ;  	/*!< [Bits 7:0] 	Data Byte 4 											*/
        volatile uint32_t data5   		: 8 ;  	/*!< [Bits 15:8] 	Data Byte 5 										*/
        volatile uint32_t data6   		: 8 ;  	/*!< [Bits 23:16] 	Data Byte 6 										*/
        volatile uint32_t data7   		: 8 ;  	/*!< [Bits 31:24] 	Data Byte 7 										*/
    	   }bit;
}CAN_RDHxR_Reg_t;

typedef struct{
    CAN_TIxR_Reg_t 	TIxR; 				/*!< CAN Transmit Mailbox Identifier Register 									*/
    CAN_TDTxR_Reg_t TDTxR; 				/*!< CAN Transmit Mailbox Data Length Control Register 							*/
    CAN_TDLxR_Reg_t TDLxR; 				/*!< CAN Transmit Mailbox Data Low Register 										*/
    CAN_TDHxR_Reg_t TDHxR;				/*!< CAN Transmit Mailbox Data High Register 									*/
}CAN_TxMailBox_Regs_t;

typedef struct{
    CAN_RIxR_Reg_t 	RIxR; 				/*!< CAN Receive FIFO Mailbox Identifier Register 								*/
    CAN_RDTxR_Reg_t RDTxR; 				/*!< CAN Receive FIFO Mailbox Data Length Control Register 						*/
    CAN_RDLxR_Reg_t RDLxR; 				/*!< CAN Receive FIFO Mailbox Data Low Register 									*/
    CAN_RDHxR_Reg_t RDHxR; 				/*!< CAN Receive FIFO Mailbox Data High Register 								*/
}CAN_RxMailBox_Regs_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t finit        	:  1 ;   /*!< [Bit 0]  		Filter init mode  									*/
        		  uint32_t reserved_1   :  7 ;   /*!< [Bits 7:1] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t can2sb       	:  6 ;   /*!< [Bits 13:8] 	CAN2 start bank 									*/
        		  uint32_t reserved_2   : 18 ; 	 /*!< [Bits 31:14] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_FMR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fbmx        	: 28 ;   /*!< [Bits 27:0] 	Filter mode 										*/
        		  uint32_t reserved    	:  4 ;   /*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_FM1R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fscx        	: 28 ;  /*!< [Bits 27:0] 	Filter scale configuration 							*/
        		  uint32_t reserved    	:  4 ;  /*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_FS1R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ffax        	: 28 ;  /*!< [Bits 27:0] 	Filter FIFO assignment for filter x 				*/
        		  uint32_t reserved    	:  4 ;  /*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_FFA1R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t factx       	: 28 ;  /*!< [Bits 27:0] 	Filter active 										*/
        		 uint32_t reserved     	:  4 ;  /*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CAN_FA1R_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fb0 			: 1 ;   /*!< [Bit 0] 		Filter bit 0 										*/
        volatile uint32_t fb1 			: 1 ;   /*!< [Bit 1] 		Filter bit 1 										*/
        volatile uint32_t fb2 			: 1 ;   /*!< [Bit 2] 		Filter bit 2 										*/
        volatile uint32_t fb3 			: 1 ;   /*!< [Bit 3] 		Filter bit 3 										*/
        volatile uint32_t fb4 			: 1 ;   /*!< [Bit 4] 		Filter bit 4 										*/
        volatile uint32_t fb5 			: 1 ;   /*!< [Bit 5] 		Filter bit 5 										*/
        volatile uint32_t fb6 			: 1 ;   /*!< [Bit 6] 		Filter bit 6 										*/
        volatile uint32_t fb7 			: 1 ;   /*!< [Bit 7] 		Filter bit 7 										*/
        volatile uint32_t fb8 			: 1 ;   /*!< [Bit 8] 		Filter bit 8 										*/
        volatile uint32_t fb9 			: 1 ;   /*!< [Bit 9] 		Filter bit 9 										*/
        volatile uint32_t fb10 			: 1 ;  	/*!< [Bit 10] 		Filter bit 10 										*/
        volatile uint32_t fb11 			: 1 ;  	/*!< [Bit 11] 		Filter bit 11 										*/
        volatile uint32_t fb12 			: 1 ;  	/*!< [Bit 12] 		Filter bit 12 										*/
        volatile uint32_t fb13 			: 1 ;  	/*!< [Bit 13] 		Filter bit 13 										*/
        volatile uint32_t fb14 			: 1 ;  	/*!< [Bit 14] 		Filter bit 14 										*/
        volatile uint32_t fb15 			: 1 ;  	/*!< [Bit 15] 		Filter bit 15 										*/
        volatile uint32_t fb16 			: 1 ;  	/*!< [Bit 16] 		Filter bit 16 										*/
        volatile uint32_t fb17 			: 1 ;  	/*!< [Bit 17] 		Filter bit 17 										*/
        volatile uint32_t fb18 			: 1 ;  	/*!< [Bit 18] 		Filter bit 18 										*/
        volatile uint32_t fb19 			: 1 ;  	/*!< [Bit 19] 		Filter bit 19 										*/
        volatile uint32_t fb20 			: 1 ;  	/*!< [Bit 20] 		Filter bit 20 										*/
        volatile uint32_t fb21 			: 1 ;  	/*!< [Bit 21] 		Filter bit 21 										*/
        volatile uint32_t fb22 			: 1 ;  	/*!< [Bit 22] 		Filter bit 22 										*/
        volatile uint32_t fb23 			: 1 ;  	/*!< [Bit 23] 		Filter bit 23 										*/
        volatile uint32_t fb24 			: 1 ;  	/*!< [Bit 24] 		Filter bit 24 										*/
        volatile uint32_t fb25 			: 1 ;  	/*!< [Bit 25] 		Filter bit 25 										*/
        volatile uint32_t fb26 			: 1 ;  	/*!< [Bit 26] 		Filter bit 26 										*/
        volatile uint32_t fb27 			: 1 ;  	/*!< [Bit 27] 		Filter bit 27 										*/
        volatile uint32_t fb28 			: 1 ;  	/*!< [Bit 28] 		Filter bit 28 										*/
        volatile uint32_t fb29 			: 1 ;  	/*!< [Bit 29] 		Filter bit 29 										*/
        volatile uint32_t fb30 			: 1 ;  	/*!< [Bit 30] 		Filter bit 30 										*/
        volatile uint32_t fb31 			: 1 ;  	/*!< [Bit 31] 		Filter bit 31 										*/
    	   }bit;
}CAN_FilterRegister_t;

typedef struct
{
	CAN_FilterRegister_t FR1; 					/*!< CAN Filter bank register 1 											*/
	CAN_FilterRegister_t FR2; 					/*!< CAN Filter bank register 1 											*/
}CAN_FiRx_Reg_t;

typedef struct
{
CAN_MCR_Reg_t              MCR;                 /*!< CAN master control register,         Address offset: 0x00          */
CAN_MSR_Reg_t              MSR;                 /*!< CAN master status register,          Address offset: 0x04          */
CAN_TSR_Reg_t              TSR;                 /*!< CAN transmit status register,        Address offset: 0x08          */
CAN_RF0R_Reg_t             RF0R;                /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
CAN_RF1R_Reg_t             RF1R;                /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
CAN_IER_Reg_t              IER;                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
CAN_ESR_Reg_t              ESR;                 /*!< CAN error status register,           Address offset: 0x18          */
CAN_BTR_Reg_t              BTR;                 /*!< CAN bit timing register,             Address offset: 0x1C          */
  uint32_t                 RESERVED0[88];       /*!< Reserved, 0x020 - 0x17F                                            */
CAN_TxMailBox_Regs_t       sTxMailBox_0;        /*!< CAN Tx MailBox,                      Address offset: 0x180		 	*/
CAN_TxMailBox_Regs_t       sTxMailBox_1;        /*!< CAN Tx MailBox,                      Address offset: 0x190			*/
CAN_TxMailBox_Regs_t       sTxMailBox_2;        /*!< CAN Tx MailBox,                      Address offset: 0x1A0			*/
CAN_RxMailBox_Regs_t       sFIFOMailBox_0;     	/*!< CAN FIFO MailBox,                    Address offset: 0x1B0 	 		*/
CAN_RxMailBox_Regs_t       sFIFOMailBox_1;     	/*!< CAN FIFO MailBox,                    Address offset: 0x1C0 			*/
  uint32_t                 RESERVED1[12];       /*!< Reserved, 0x1D0 - 0x1FF                                            */
CAN_FMR_Reg_t              FMR;                 /*!< CAN filter master register,          Address offset: 0x200         */
CAN_FM1R_Reg_t             FM1R;                /*!< CAN filter mode register,            Address offset: 0x204         */
  uint32_t                 RESERVED2;           /*!< Reserved, 0x208                                                    */
CAN_FS1R_Reg_t             FS1R;                /*!< CAN filter scale register,           Address offset: 0x20C         */
  uint32_t                 RESERVED3;           /*!< Reserved, 0x210                                                    */
CAN_FFA1R_Reg_t            FFA1R;               /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
  uint32_t                 RESERVED4;           /*!< Reserved, 0x218                                                    */
CAN_FA1R_Reg_t             FA1R;                /*!< CAN filter activation register,      Address offset: 0x21C         */
  uint32_t                 RESERVED5[8];        /*!< Reserved, 0x220-0x23F                                              */
CAN_FiRx_Reg_t       	   sF0x; 				/*!< CAN Filter Registers,                Address offset: 0x240-0x31C   */
CAN_FiRx_Reg_t       	   sF1x;
CAN_FiRx_Reg_t       	   sF2x;
CAN_FiRx_Reg_t       	   sF3x;
CAN_FiRx_Reg_t       	   sF4x;
CAN_FiRx_Reg_t       	   sF5x;
CAN_FiRx_Reg_t       	   sF6x;
CAN_FiRx_Reg_t       	   sF7x;
CAN_FiRx_Reg_t       	   sF8x;
CAN_FiRx_Reg_t       	   sF9x;
CAN_FiRx_Reg_t       	   sF10x;
CAN_FiRx_Reg_t       	   sF11x;
CAN_FiRx_Reg_t       	   sF12x;
CAN_FiRx_Reg_t       	   sF13x;
CAN_FiRx_Reg_t       	   sF14x;
CAN_FiRx_Reg_t       	   sF15x;
CAN_FiRx_Reg_t       	   sF16x;
CAN_FiRx_Reg_t       	   sF17x;
CAN_FiRx_Reg_t       	   sF18x;
CAN_FiRx_Reg_t       	   sF19x;
CAN_FiRx_Reg_t       	   sF20x;
CAN_FiRx_Reg_t       	   sF21x;
CAN_FiRx_Reg_t       	   sF22x;
CAN_FiRx_Reg_t       	   sF23x;
CAN_FiRx_Reg_t       	   sF24x;
CAN_FiRx_Reg_t       	   sF25x;
CAN_FiRx_Reg_t       	   sF26x;
CAN_FiRx_Reg_t       	   sF27x;
}CAN_RegDef_t;

/**
  * @brief CRC calculation unit
  */
typedef union{
    uint32_t reg;
    struct{
    	volatile uint32_t data 			: 32 ;  /*!< [Bits 31:0] 	Data register bits 									*/
    	   }bit;
}CRC_DR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t data 			:  8 ;  /*!< [Bits 7:0] 		General-purpose 8-bit data register bits 			*/
        		  uint32_t reserved 	: 24 ; 	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CRC_IDR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t reset 		:  1 ;  /*!< [Bit 0] 		RESET bit 											*/
        		  uint32_t reserved 	: 31 ; 	/*!< [Bits 31:1] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}CRC_CR_Reg_t;

typedef struct
{
CRC_DR_Reg_t  	DR;         /*!< CRC Data register,             Address offset: 0x00 */
CRC_IDR_Reg_t  	IDR;        /*!< CRC Independent data register, Address offset: 0x04 */
  uint8_t       RESERVED0;  /*!< Reserved, 0x05                                      */
  uint16_t      RESERVED1;  /*!< Reserved, 0x06                                      */
CRC_CR_Reg_t 	CR;         /*!< CRC Control register,          Address offset: 0x08 */
}CRC_RegDef_t;

/**
  * @brief Digital to Analog Converter
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t en1 			: 1 ;   /*!< [Bit 0] 		DAC channel1 enable 								*/
        volatile uint32_t boff1 		: 1 ;   /*!< [Bit 1] 		DAC channel1 output buffer disable 					*/
        volatile uint32_t ten1 			: 1 ;   /*!< [Bit 2] 		DAC channel1 trigger enable 						*/
        volatile uint32_t tsel1 		: 3 ;   /*!< [Bits 5:3] 		DAC channel1 trigger selection 						*/
        volatile uint32_t wave1 		: 2 ;   /*!< [Bits 7:6] 		DAC channel1 noise/triangle wave generation enable 	*/
        volatile uint32_t mamp1 		: 4 ;   /*!< [Bits 11:8] 	DAC channel1 mask/amplitude selector 				*/
        volatile uint32_t dmaen1 		: 1 ;   /*!< [Bit 12] 		DAC channel1 DMA enable 							*/
        volatile uint32_t dmaudrie1 	: 1 ; 	/*!< [Bit 13] 		DAC channel1 DMA Underrun Interrupt enable 			*/
        		  uint32_t reserved_0 	: 2 ; 	/*!< [Bits 15:14] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t en2 			: 1 ;   /*!< [Bit 16] 		DAC channel2 enable 								*/
        volatile uint32_t boff2 		: 1 ;   /*!< [Bit 17] 		DAC channel2 output buffer disable 					*/
        volatile uint32_t ten2 			: 1 ;   /*!< [Bit 18] 		DAC channel2 trigger enable 						*/
        volatile uint32_t tsel2 		: 3 ;   /*!< [Bits 21:19] 	DAC channel2 trigger selection 						*/
        volatile uint32_t wave2 		: 2 ;   /*!< [Bits 23:22] 	DAC channel2 noise/triangle wave generation enable 	*/
        volatile uint32_t mamp2 		: 4 ;   /*!< [Bits 27:24] 	DAC channel2 mask/amplitude selector 				*/
        volatile uint32_t dmaen2 		: 1 ;   /*!< [Bit 28] 		DAC channel2 DMA enable 							*/
        volatile uint32_t dmaudrie2 	: 1 ; 	/*!< [Bit 29] 		DAC channel2 DMA underrun interrupt enable 			*/
        		  uint32_t reserved_1 	: 2 ; 	/*!< [Bits 31:30] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t swtrig1 		:  1 ;  /*!< [Bit 0]       	DAC channel1 software trigger 						*/
        volatile uint32_t swtrig2 		:  1 ;  /*!< [Bit 1]       	DAC channel2 software trigger 						*/
        		  uint32_t reserved  	: 30 ;  /*!< [Bits 31:2]   	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_SWTRIGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc1dhr 		: 12 ; 	/*!< [Bits 11:0]    DAC channel1 12-bit right-aligned data 				*/
        		  uint32_t reserved 	: 20 ;  /*!< [Bits 31:12]   Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR12R1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	:  4 ;  /*!< [Bits 3:0] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc1dhr 		: 12 ; 	/*!< [Bits 15:4] 	DAC channel1 12-bit left-aligned data 				*/
        		  uint32_t reserved_1 	: 16 ;  /*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR12L1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc1dhr 		:  8 ; 	/*!< [Bits 7:0] 		DAC channel1 8-bit right-aligned data 				*/
        	      uint32_t reserved 	: 24 ; 	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR8R1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 3:0] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc2dhr 		: 12 ; 	/*!< [Bits 15:4] 	DAC channel2 12-bit right-aligned data 				*/
        		  uint32_t reserved_1 	: 16 ; 	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR12R2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 3:0] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc2dhr 		: 12 ; 	/*!< [Bits 15:4] 	DAC channel2 12-bit left-aligned data 				*/
        		  uint32_t reserved_1 	: 16 ; 	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR12L2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc2dhr 		:  8 ; 	/*!< [Bits 7:0] 		DAC channel2 8-bit right-aligned data 				*/
        		  uint32_t reserved_0 	: 24 ;  /*!< [Bits 31:8] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR8R2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc1dhr 		: 12 ; 	/*!< [Bits 11:0] 	DAC channel1 12-bit right-aligned data 				*/
        		  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 15:12] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc2dhr 		: 12 ; 	/*!< [Bits 27:16] 	DAC channel2 12-bit right-aligned data 				*/
        		  uint32_t reserved_1 	:  4 ; 	/*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR12RD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    			  uint32_t reserved_0 	:  4 ; 	/*!< [Bits 3:0] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc1dhr 		: 12 ; 	/*!< [Bits 15:4] 	DAC channel1 12-bit left-aligned data 				*/
        		  uint32_t reserved_1 	:  4 ; 	/*!< [Bits 19:16] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t dacc2dhr 		: 12 ; 	/*!< [Bits 31:20] 	DAC channel2 12-bit left-aligned data 				*/
    	   }bit;
}DAC_DHR12LD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc1dhr 		:  8 ; 	/*!< [Bits 7:0] 		DAC channel1 8-bit right-aligned data 				*/
        volatile uint32_t dacc2dhr 		:  8 ; 	/*!< [Bits 15:8] 	DAC channel2 8-bit right-aligned data 				*/
        		  uint32_t reserved 	: 16 ; 	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_DHR8RD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc1dor 		: 12 ; 	/*!< [Bits 11:0] 	DAC channel1 data output 							*/
        		  uint32_t reserved 	: 20 ; 	/*!< [Bits 31:12] 	Reserved, must be kept at reset value 				*/
    }bit;
}DAC_DOR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dacc2dor 		: 12 ; 	/*!< [Bits 11:0] 	DAC channel1 data output 							*/
        		  uint32_t reserved 	: 20 ; 	/*!< [Bits 31:12] 	Reserved, must be kept at reset value 				*/
    }bit;
}DAC_DOR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0  	: 13 ;	/*!< [Bits 12:0] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t dmaudr1 		:  1 ;	/*!< [Bit 13] 		DAC channel1 DMA underrun flag 						*/
        		  uint32_t reserved_1  	: 15 ;	/*!< [Bits 28:14] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t dmaudr2 		:  1 ;	/*!< [Bit 29] 		DAC channel2 DMA underrun flag 						*/
        		  uint32_t reserved_2  	:  2 ;	/*!< [Bits 31:30] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DAC_SR_Reg_t;

typedef struct
{
DAC_CR_Reg_t 	  CR;       /*!< DAC control register,                                    Address offset: 0x00 */
DAC_SWTRIGR_Reg_t SWTRIGR;  /*!< DAC software trigger register,                           Address offset: 0x04 */
DAC_DHR12R1_Reg_t DHR12R1;  /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
DAC_DHR12L1_Reg_t DHR12L1;  /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
DAC_DHR8R1_Reg_t  DHR8R1;   /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
DAC_DHR12R2_Reg_t DHR12R2;  /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
DAC_DHR12L2_Reg_t DHR12L2;  /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
DAC_DHR8R2_Reg_t  DHR8R2;   /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
DAC_DHR12RD_Reg_t DHR12RD;  /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
DAC_DHR12LD_Reg_t DHR12LD;  /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
DAC_DHR8RD_Reg_t  DHR8RD;   /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
DAC_DOR1_Reg_t 	  DOR1;     /*!< DAC channel1 data output register,                       Address offset: 0x2C */
DAC_DOR2_Reg_t    DOR2;     /*!< DAC channel2 data output register,                       Address offset: 0x30 */
DAC_SR_Reg_t 	  SR;       /*!< DAC status register,                                     Address offset: 0x34 */
}DAC_TypeDef;

/**
  * @brief Debug MCU
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dev_id 		: 13 ;	/*!< [Bits 12:0] 		Device identificator							*/
        		  uint32_t reserved		:  3 ;	/*!< [Bits 13:15]		Reserved, must be kept at reset value			*/
        volatile uint32_t rev_id 		: 16 ;	/*!< [Bit 32:16] 		DAC channel2 DMA underrun flag 					*/
    	   }bit;
}DBGMCU_IDCODE_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dbg_sleep   	:  1 ; 	/*!< [Bit 0] 			Debug Sleep mode */
        volatile uint32_t dbg_stop    	:  1 ; 	/*!< [Bit 1] 			Debug Stop mode */
        volatile uint32_t dbg_standby 	:  1 ; 	/*!< [Bit 2] 			Debug Standby mode */
        		  uint32_t reserved_0   :  2 ; 	/*!< [Bits 4:3] 			Reserved, must be kept at reset value */
        volatile uint32_t trace_ioen 	:  1 ; 	/*!< [Bit 5] 			TRACE pin assignment control - Trace pin assignment
         	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 enable */
        volatile uint32_t trace_mode 	:  2 ; 	/*!< [Bits 7:6] 			TRACE pin assignment control - Trace mode */
        		  uint32_t reserved_1   : 24 ; 	/*!< [Bits 31:8] 		Reserved, must be kept at reset value */
    	   }bit;
}DBGMCU_CR_Reg_t;

typedef union {
    uint32_t reg;
    struct{
        volatile uint32_t tim2_stop    			: 1 ;  /*!< [Bit 0]  TIM2 counter stopped when core is halted		*/
        volatile uint32_t tim3_stop    			: 1 ;  /*!< [Bit 1] 	 TIM3 counter stopped when core is halted		*/
        volatile uint32_t tim4_stop 			: 1 ;  /*!< [Bit 2]  TIM4 counter stopped when core is halted		*/
        volatile uint32_t tim5_stop 			: 1 ;  /*!< [Bit 3]  TIM5 counter stopped when core is halted		*/
        volatile uint32_t tim6_stop  			: 1 ;  /*!< [Bit 4]  TIM6 counter stopped when core is halted		*/
        volatile uint32_t tim7_stop  			: 1 ;  /*!< [Bit 5]  TIM7 counter stopped when core is halted		*/
        volatile uint32_t tim12_stop  			: 1 ;  /*!< [Bit 6]  TIM12 counter stopped when core is halted		*/
        volatile uint32_t tim13_stop  			: 1 ;  /*!< [Bit 7]  TIM13 counter stopped when core is halted		*/
        volatile uint32_t tim14_stop 			: 1 ;  /*!< [Bit 8] 	 TIM14 counter stopped when core is halted		*/
        		  uint32_t reserved_0 			: 1 ;  /*!< [Bit 9] 	 Reserved, must be kept at reset value 			*/
        volatile uint32_t rtc_stop   			: 1 ;  /*!< [Bit 10] RTC stopped when Core is halted 				*/
        volatile uint32_t wwdg_stop  			: 1 ;  /*!< [Bit 11] Debug Window Watchdog stopped when Core is halted */
        volatile uint32_t iwdg_stop  			: 1 ;  /*!< [Bit 12] Debug independent watchdog stopped when core is halted */
        		  uint32_t reserved_1 			: 8 ;  /*!< [Bits 20:13] Reserved, must be kept at reset value 		*/
        volatile uint32_t i2c1_smbus_timeout	: 1 ;  /*!< [Bit 21] SMBUS timeout mode stopped when Core is halted */
        volatile uint32_t i2c2_smbus_timeout 	: 1 ;  /*!< [Bit 22] SMBUS timeout mode stopped when Core is halted */
        volatile uint32_t i2c3_smbus_timeout	: 1 ;  /*!< [Bit 23] SMBUS timeout mode stopped when Core is halted */
        		  uint32_t reserved_2        	: 1 ;  /*!< [Bit 24] Reserved, must be kept at reset value 			*/
        volatile uint32_t can1_stop      		: 1 ;  /*!< [Bit 25] Debug CAN1 stopped when Core is halted 			*/
        volatile uint32_t can2_stop      		: 1 ;  /*!< [Bit 26] Debug CAN2 stopped when Core is halted 			*/
        		  uint32_t reserved_3        	: 5 ;  /*!< [Bits 31:27] Reserved, must be kept at reset value 		*/
    	   }bit;
}DBGMCU_APB1FZ_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim1_stop 			:  1 ;  /*!< [Bit 0] TIM1 counter stopped when core is halted 		*/
        volatile uint32_t tim8_stop 			:  1 ;  /*!< [Bit 1] TIM8 counter stopped when core is halted 		*/
        		  uint32_t reserved_0    		: 13 ;  /*!< [Bit 15:2] Reserved, must be kept at reset value 		*/
        volatile uint32_t tim9_stop 			:  1 ;  /*!< [Bit 16] TIM9 counter stopped when core is halted		*/
        volatile uint32_t tim10_stop 			:  1 ;  /*!< [Bit 17] TIM10 counter stopped when core is halted		*/
        volatile uint32_t tim11_stop 			:  1 ;  /*!< [Bit 18] TIM11 counter stopped when core is halted		*/
        		  uint32_t reserved_1          	: 13 ;  /*!< [Bits 31:19] Reserved, must be kept at reset value 		*/
    	   }bit;
}DBGMCU_APB2FZ_Reg_t;

typedef struct
{
DBGMCU_IDCODE_Reg_t IDCODE;  /*!< MCU device ID code,               Address offset: 0x00 */
DBGMCU_CR_Reg_t 	CR;      /*!< Debug MCU configuration register, Address offset: 0x04 */
DBGMCU_APB1FZ_Reg_t APB1FZ;  /*!< Debug MCU APB1 freeze register,   Address offset: 0x08 */
DBGMCU_APB2FZ_Reg_t APB2FZ;  /*!< Debug MCU APB2 freeze register,   Address offset: 0x0C */
}DBGMCU_RegDef_t;

/**
  * @brief DCMI
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t capture 		:  1 ;   /*!< [Bit 0] 		Capture enable                                		*/
        volatile uint32_t cm     		:  1 ;   /*!< [Bit 1] 		Capture mode                                  		*/
        volatile uint32_t crop      	:  1 ;   /*!< [Bit 2] 		Crop feature                                  		*/
        volatile uint32_t jpeg      	:  1 ;   /*!< [Bit 3] 		JPEG format                                   		*/
        volatile uint32_t ess     		:  1 ;   /*!< [Bit 4] 		Embedded synchronization select              		*/
        volatile uint32_t pckpol  		:  1 ;   /*!< [Bit 5] 		Pixel clock polarity                         		*/
        volatile uint32_t hspol  		:  1 ;   /*!< [Bit 6] 		Horizontal synchronization polarity          		*/
        volatile uint32_t vspol 		:  1 ;   /*!< [Bit 7] 		Vertical synchronization polarity            		*/
        volatile uint32_t fcrc    		:  2 ;   /*!< [Bits 9:8] 	Frame capture rate control                			*/
        volatile uint32_t edm    		:  2 ;   /*!< [Bits 11:10] 	Extended data mode                      			*/
        		  uint32_t reserved_0	:  2 ;   /*!< [Bits 13:12] 	Reserved, must be kept at reset value   			*/
        volatile uint32_t enable   		:  1 ;   /*!< [Bit 14] 		DCMI enable                                 		*/
        		  uint32_t reserved_1 	: 16 ;   /*!< [Bits 31:15] 	Reserved, must be kept at reset value   			*/
    	   }bit;
}DCMI_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t hsync			:  1 ;   /*!< [Bit 0] 		HSYNC                                       		*/
        volatile uint32_t vsync			:  1 ;   /*!< [Bit 1] 		VSYNC                                       		*/
        volatile uint32_t fne  			:  1 ;   /*!< [Bit 2] 		FIFO not empty                               		*/
        		  uint32_t reserved	  	: 29 ;   /*!< [Bits 31:3] 	Reserved, must be kept at reset value   			*/
    	   }bit;
}DCMI_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t frame_ris   	:  1 ;  /*!< [Bit 0]      	Capture complete raw interrupt status              	*/
        volatile uint32_t ovr_ris     	:  1 ;  /*!< [Bit 1]       	Overrun raw interrupt status                        */
        volatile uint32_t err_ris     	:  1 ;  /*!< [Bit 2]       	Synchronization error raw interrupt status          */
        volatile uint32_t vsync_ris   	:  1 ;  /*!< [Bit 3]       	VSYNC raw interrupt status                          */
        volatile uint32_t line_ris    	:  1 ;  /*!< [Bit 4]       	Line raw interrupt status                           */
                  uint32_t reserved	 	: 27 ;  /*!< [Bits 31:5]   	Reserved, must be kept at reset value               */
    }bit;
}DCMI_RIS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t frame_ie   	:  1 ;  /*!< [Bit 0]       	Capture complete interrupt enable              		*/
        volatile uint32_t ovr_ie     	:  1 ;  /*!< [Bit 1]       	Overrun interrupt enable                        	*/
        volatile uint32_t err_ie     	:  1 ;  /*!< [Bit 2]       	Synchronization error interrupt enable          	*/
        volatile uint32_t vsync_ie   	:  1 ;  /*!< [Bit 3]       	VSYNC interrupt enable                           	*/
        volatile uint32_t line_ie    	:  1 ;  /*!< [Bit 4]       	Line interrupt enable                            	*/
                  uint32_t reserved		: 27 ;  /*!< [Bits 31:5]   	Reserved, must be kept at reset value            	*/
    	   }bit;
}DCMI_IER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t frame_mis  	:  1 ;  /*!< [Bit 0]       	Capture complete masked interrupt status         	*/
        volatile uint32_t ovr_mis    	:  1 ;  /*!< [Bit 1]       	Overrun masked interrupt status                   	*/
        volatile uint32_t err_mis    	:  1 ;  /*!< [Bit 2]       	Synchronization error masked interrupt status     	*/
        volatile uint32_t vsync_mis  	:  1 ;  /*!< [Bit 3]       	VSYNC masked interrupt status                     	*/
        volatile uint32_t line_mis   	:  1 ;  /*!< [Bit 4]       	Line masked interrupt status                      	*/
                  uint32_t reserved		: 27 ;  /*!< [Bits 31:5]   	Reserved, must be kept at reset value             	*/
    }bit;
}DCMI_MIS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t frame_isc  	:  1 ;  /*!< [Bit 0]       	Capture complete interrupt status clear              */
        volatile uint32_t ovr_isc    	:  1 ;  /*!< [Bit 1]       	Overrun interrupt status clear                       */
        volatile uint32_t err_isc    	:  1 ;  /*!< [Bit 2]       	Synchronization error interrupt status clear         */
        volatile uint32_t vsync_isc  	:  1 ;  /*!< [Bit 3]       	Vertical synch interrupt status clear                */
        volatile uint32_t line_isc   	:  1 ;  /*!< [Bit 4]       	Line interrupt status clear                          */
                  uint32_t reserved		: 27 ;  /*!< [Bits 31:5]   	Reserved, must be kept at reset value                */
    	   }bit;
}DCMI_ICR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fsc   		: 8 ;   /*!< [Bits 7:0]     	Frame start delimiter code                  		*/
        volatile uint32_t lsc   		: 8 ;   /*!< [Bits 15:8]    	Line start delimiter code                   		*/
        volatile uint32_t lec   		: 8 ;   /*!< [Bits 23:16]   	Line end delimiter code                     		*/
        volatile uint32_t fec   		: 8 ;   /*!< [Bits 31:24]   	Frame end delimiter code                    		*/
    	   }bit;
}DCMI_ESCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fsu   		: 8 ;   /*!< [Bits 7:0] 		Frame start delimiter unmask                  		*/
        volatile uint32_t lsu   		: 8 ;   /*!< [Bits 15:8] 	Line start delimiter unmask                   		*/
        volatile uint32_t leu   		: 8 ;   /*!< [Bits 23:16]  	Line end delimiter unmask                     		*/
        volatile uint32_t feu   		: 8 ;   /*!< [Bits 31:24] 	Frame end delimiter unmask                    		*/
    	   }bit;
}DCMI_ESUR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t hoffcnt 		: 14 ; 	/*!< [Bit 13:0]   	Horizontal offset count                     		*/
        		  uint32_t reserved_0 	:  2 ; 	/*!< [Bits 15:14] 	Reserved, must be kept at reset value       		*/
        volatile uint32_t vst 			: 13 ;  /*!< [Bits 28:16] 	Vertical start line count                     		*/
        		  uint32_t reserved_1 	:  3 ;  /*!< [Bits 31:29]  	Reserved, must be kept at reset value       		*/
    	   }bit;
}DCMI_CWSTRT_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t capcnt 		: 14 ; 	/*!< [Bit 13:0]   	Capture count                               		*/
        		  uint32_t reserved_0 	:  2 ; 	/*!< [Bits 15:14] 	Reserved, must be kept at reset value       		*/
        volatile uint32_t vline 		: 14 ; 	/*!< [Bits 29:16]   Vertical line count                         			*/
        		  uint32_t reserved_1 	:  2 ; 	/*!< [Bits 31:30]   Reserved, must be kept at reset value       			*/
    	   }bit;
}DCMI_CWSIZE_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t byte_0 		: 8 ;  /*!< [Bits 7:0]   	Data byte 0                          				*/
        volatile uint32_t byte_1 		: 8 ;  /*!< [Bits 15:8]   	Data byte 1                          				*/
        volatile uint32_t byte_2 		: 8 ;  /*!< [Bits 23:16]  	Data byte 2                          				*/
        volatile uint32_t byte_3 		: 8 ;  /*!< [Bits 31:24] 	Data byte 3                          				*/
    	   }bit;
}DCMI_DR_Reg_t;

typedef struct
{
DCMI_CR_Reg_t 		CR;       /*!< DCMI control register 1,                       Address offset: 0x00 */
DCMI_SR_Reg_t 		SR;       /*!< DCMI status register,                          Address offset: 0x04 */
DCMI_RIS_Reg_t 		RIS;      /*!< DCMI raw interrupt status register,            Address offset: 0x08 */
DCMI_IER_Reg_t 		IER;      /*!< DCMI interrupt enable register,                Address offset: 0x0C */
DCMI_MIS_Reg_t 		MIS;      /*!< DCMI masked interrupt status register,         Address offset: 0x10 */
DCMI_ICR_Reg_t 		ICR;      /*!< DCMI interrupt clear register,                 Address offset: 0x14 */
DCMI_ESCR_Reg_t 	ESCR;     /*!< DCMI embedded synchronization code register,   Address offset: 0x18 */
DCMI_ESUR_Reg_t 	ESUR;     /*!< DCMI embedded synchronization unmask register, Address offset: 0x1C */
DCMI_CWSTRT_Reg_t 	CWSTRT;   /*!< DCMI crop window start,                        Address offset: 0x20 */
DCMI_CWSIZE_Reg_t 	CWSIZE;   /*!< DCMI crop window size,                         Address offset: 0x24 */
DCMI_DR_Reg_t 		DR;       /*!< DCMI data register,                            Address offset: 0x28 */
}DCMI_RegDef_t;

/**
  * @brief DMA Controller
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t feif0 		: 1 ;  	/*!< [Bit 0] 		Stream 0 FIFO error interrupt flag 					*/
        		  uint32_t reserved_0 	: 1 ;   /*!< [Bit 1] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif0 		: 1 ; 	/*!< [Bit 2] 		Stream 0 direct mode error interrupt flag 			*/
        volatile uint32_t teif0 		: 1 ;  	/*!< [Bit 3] 		Stream 0 transfer error interrupt flag 				*/
        volatile uint32_t htif0 		: 1 ;  	/*!< [Bit 4] 		Stream 0 half transfer interrupt flag 				*/
        volatile uint32_t tcif0 		: 1 ;  	/*!< [Bit 5] 		Stream 0 transfer complete interrupt flag 			*/
        volatile uint32_t feif1 		: 1 ;  	/*!< [Bit 6] 		Stream 1 FIFO error interrupt flag 					*/
        		  uint32_t reserved_1 	: 1 ;   /*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif1 		: 1 ; 	/*!< [Bit 8] 		Stream 1 direct mode error interrupt flag 			*/
        volatile uint32_t teif1 		: 1 ;  	/*!< [Bit 9] 		Stream 1 transfer error interrupt flag 				*/
        volatile uint32_t htif1 		: 1 ;  	/*!< [Bit 10] 		Stream 1 half transfer interrupt flag 				*/
        volatile uint32_t tcif1 		: 1 ;  	/*!< [Bit 11] 		Stream 1 transfer complete interrupt flag 			*/
        		  uint32_t reserved_2 	: 4 ;   /*!< [Bits 12:15] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t feif2 		: 1 ;  	/*!< [Bit 16] 		Stream 2 FIFO error interrupt flag 					*/
        		  uint32_t reserved_3 	: 1 ;   /*!< [Bit 17] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif2 		: 1 ; 	/*!< [Bit 18] 		Stream 2 direct mode error interrupt flag 			*/
        volatile uint32_t teif2 		: 1 ;  	/*!< [Bit 19] 		Stream 2 transfer error interrupt flag 				*/
        volatile uint32_t htif2 		: 1 ;  	/*!< [Bit 20] 		Stream 2 half transfer interrupt flag 				*/
        volatile uint32_t tcif2 		: 1 ;  	/*!< [Bit 21] 		Stream 2 transfer complete interrupt flag 			*/
        volatile uint32_t feif3 		: 1 ;  	/*!< [Bit 22] 		Stream 3 FIFO error interrupt flag 					*/
        		  uint32_t reserved_4 	: 1 ;   /*!< [Bit 23] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif3 		: 1 ; 	/*!< [Bit 24] 		Stream 3 direct mode error interrupt flag 			*/
        volatile uint32_t teif3 		: 1 ;  	/*!< [Bit 25] 		Stream 3 transfer error interrupt flag 				*/
        volatile uint32_t htif3 		: 1 ;  	/*!< [Bit 26] 		Stream 3 half transfer interrupt flag 				*/
        volatile uint32_t tcif3 		: 1 ;  	/*!< [Bit 27] 		Stream 3 transfer complete interrupt flag 			*/
        uint32_t reserved_5 			: 4 ;   /*!< [Bits 28:31] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_LISR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t feif4 		: 1 ;  	/*!< [Bit 0] 		Stream 4 FIFO error interrupt flag 					*/
        		  uint32_t reserved_0 	: 1 ;   /*!< [Bit 1] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif4 		: 1 ; 	/*!< [Bit 2] 		Stream 4 direct mode error interrupt flag 			*/
        volatile uint32_t teif4 		: 1 ;  	/*!< [Bit 3] 		Stream 4 transfer error interrupt flag 				*/
        volatile uint32_t htif4 		: 1 ;  	/*!< [Bit 4] 		Stream 4 half transfer interrupt flag 				*/
        volatile uint32_t tcif4 		: 1 ;  	/*!< [Bit 5] 		Stream 4 transfer complete interrupt flag 			*/
        volatile uint32_t feif5 		: 1 ;  	/*!< [Bit 6] 		Stream 5 FIFO error interrupt flag 					*/
        		  uint32_t reserved_1 	: 1 ;   /*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif5 		: 1 ; 	/*!< [Bit 8] 		Stream 5 direct mode error interrupt flag 			*/
        volatile uint32_t teif5 		: 1 ;  	/*!< [Bit 9] 		Stream 5 transfer error interrupt flag 				*/
        volatile uint32_t htif5 		: 1 ;  	/*!< [Bit 10] 		Stream 5 half transfer interrupt flag 				*/
        volatile uint32_t tcif5 		: 1 ;  	/*!< [Bit 11] 		Stream 5 transfer complete interrupt flag 			*/
        		  uint32_t reserved_2 	: 4 ;   /*!< [Bits 12:15] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t feif6 		: 1 ;  	/*!< [Bit 16] 		Stream 6 FIFO error interrupt flag 					*/
        		  uint32_t reserved_3 	: 1 ;   /*!< [Bit 17] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif6 		: 1 ; 	/*!< [Bit 18] 		Stream 6 direct mode error interrupt flag 			*/
        volatile uint32_t teif6 		: 1 ;  	/*!< [Bit 19] 		Stream 6 transfer error interrupt flag 				*/
        volatile uint32_t htif6 		: 1 ;  	/*!< [Bit 20] 		Stream 6 half transfer interrupt flag 				*/
        volatile uint32_t tcif6 		: 1 ;  	/*!< [Bit 21] 		Stream 6 transfer complete interrupt flag 			*/
        volatile uint32_t feif7 		: 1 ;  	/*!< [Bit 22] 		Stream 7 FIFO error interrupt flag 					*/
        		  uint32_t reserved_4 	: 1 ;   /*!< [Bit 23] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t dmeif7 		: 1 ; 	/*!< [Bit 24] 		Stream 7 direct mode error interrupt flag 			*/
        volatile uint32_t teif7 		: 1 ;  	/*!< [Bit 25] 		Stream 7 transfer error interrupt flag 				*/
        volatile uint32_t htif7 		: 1 ;  	/*!< [Bit 26] 		Stream 7 half transfer interrupt flag 				*/
        volatile uint32_t tcif7 		: 1 ;  	/*!< [Bit 27] 		Stream 7 transfer complete interrupt flag 			*/
        		  uint32_t reserved_5 	: 4 ;   /*!< [Bits 28:31] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_HISR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cfeif0 		: 1 ; 	/*!< [Bit 0] 		Stream 0 clear FIFO error interrupt flag */
        		  uint32_t reserved_0 	: 1 ;   /*!< [Bit 1] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif0 		: 1 ; 	/*!< [Bit 2] 		Stream 0 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif0 		: 1 ;  	/*!< [Bit 3] 		Stream 0 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif0 		: 1 ;  	/*!< [Bit 4] 		Stream 0 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif0 		: 1 ;  	/*!< [Bit 5] 		Stream 0 clear transfer complete interrupt flag 	*/
        volatile uint32_t cfeif1 		: 1 ;  	/*!< [Bit 6] 		Stream 1 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_1 	: 1 ;   /*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif1 		: 1 ; 	/*!< [Bit 8] 		Stream 1 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif1 		: 1 ;  	/*!< [Bit 9] 		Stream 1 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif1 		: 1 ;  	/*!< [Bit 10] 		Stream 1 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif1 		: 1 ;  	/*!< [Bit 11] 		Stream 1 clear transfer complete interrupt flag 	*/
        		  uint32_t reserved_2 	: 4 ;   /*!< [Bits 12:15] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t cfeif2 		: 1 ;  	/*!< [Bit 16] 		Stream 2 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_3 	: 1 ;   /*!< [Bit 17] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif2 		: 1 ; 	/*!< [Bit 18] 		Stream 2 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif2 		: 1 ;  	/*!< [Bit 19] 		Stream 2 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif2 		: 1 ;  	/*!< [Bit 20] 		Stream 2 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif2 		: 1 ;  	/*!< [Bit 21] 		Stream 2 clear transfer complete interrupt flag 	*/
        volatile uint32_t cfeif3 		: 1 ;  	/*!< [Bit 22] 		Stream 3 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_4 	: 1 ;   /*!< [Bit 23] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif3 		: 1 ; 	/*!< [Bit 24] 		Stream 3 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif3 		: 1 ;  	/*!< [Bit 25] 		Stream 3 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif3 		: 1 ;  	/*!< [Bit 26] 		Stream 3 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif3 		: 1 ;  	/*!< [Bit 27] 		Stream 3 clear transfer complete interrupt flag 	*/
        		  uint32_t reserved_5 	: 4 ;   /*!< [Bits 28:31] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_LIFCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cfeif4     	: 1 ;  	/*!< [Bit 0] 		Stream 4 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_0   : 1 ;  	/*!< [Bit 1] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif4    	: 1 ;  	/*!< [Bit 2] 		Stream 4 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif4     	: 1 ;  	/*!< [Bit 3] 		Stream 4 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif4     	: 1 ;  	/*!< [Bit 4] 		Stream 4 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif4     	: 1 ;  	/*!< [Bit 5] 		Stream 4 clear transfer complete interrupt flag 	*/
        volatile uint32_t cfeif5     	: 1 ;  	/*!< [Bit 6] 		Stream 5 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_1   : 1 ;  	/*!< [Bit 7] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif5    	: 1 ;  	/*!< [Bit 8] 		Stream 5 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif5     	: 1 ;  	/*!< [Bit 9] 		Stream 5 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif5     	: 1 ;  	/*!< [Bit 10] 		Stream 5 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif5     	: 1 ;  	/*!< [Bit 11] 		Stream 5 clear transfer complete interrupt flag 	*/
        		  uint32_t reserved_2   : 4 ;  	/*!< [Bits 12:15] 	Reserved, must be kept at reset value 				*/
        volatile uint32_t cfeif6    	: 1 ;  	/*!< [Bit 16] 		Stream 6 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_3   : 1 ;  	/*!< [Bit 17] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif6    	: 1 ;  	/*!< [Bit 18] 		Stream 6 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif6     	: 1 ;  	/*!< [Bit 19] 		Stream 6 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif6     	: 1 ;  	/*!< [Bit 20] 		Stream 6 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif6     	: 1 ;  	/*!< [Bit 21] 		Stream 6 clear transfer complete interrupt flag 	*/
        volatile uint32_t cfeif7     	: 1 ;  	/*!< [Bit 22] 		Stream 7 clear FIFO error interrupt flag 			*/
        		  uint32_t reserved_4   : 1 ;  	/*!< [Bit 23] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t cdmeif7    	: 1 ;  	/*!< [Bit 24]	 	Stream 7 clear direct mode error interrupt flag 	*/
        volatile uint32_t cteif7     	: 1 ;  	/*!< [Bit 25] 		Stream 7 clear transfer error interrupt flag 		*/
        volatile uint32_t chtif7     	: 1 ;  	/*!< [Bit 26] 		Stream 7 clear half transfer interrupt flag 		*/
        volatile uint32_t ctcif7     	: 1 ;  	/*!< [Bit 27] 		Stream 7 clear transfer complete interrupt flag 	*/
        		  uint32_t reserved_5   : 4 ;  	/*!< [Bits 28:31] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_HIFCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t en        	: 1 ;  	/*!< [Bit 0] 		Stream enable / flag stream ready when read low 	*/
        volatile uint32_t dmeie     	: 1 ;  	/*!< [Bit 1] 		Direct mode error interrupt enable 					*/
        volatile uint32_t teie      	: 1 ;  	/*!< [Bit 2] 		Transfer error interrupt enable 					*/
        volatile uint32_t htie      	: 1 ;  	/*!< [Bit 3] 		Half transfer interrupt enable 						*/
        volatile uint32_t tcie      	: 1 ;  	/*!< [Bit 4] 		Transfer complete interrupt enable 					*/
        volatile uint32_t pfctrl      	: 1 ;  	/*!< [Bit 5] 		Peripheral flow controller 							*/
        volatile uint32_t dir      		: 2 ;  	/*!< [Bits 7:6] 		Data transfer direction 							*/
        volatile uint32_t circ     		: 1 ;  	/*!< [Bit 8] 		Circular mode 										*/
        volatile uint32_t pinc     		: 1 ;  	/*!< [Bit 9] 		Peripheral increment mode 							*/
        volatile uint32_t minc     		: 1 ;  	/*!< [Bit 10] 		Memory increment mode 								*/
        volatile uint32_t psize    		: 2 ;  	/*!< [Bits 12:11] 	Peripheral data size 								*/
        volatile uint32_t msize    		: 2 ;  	/*!< [Bits 14:13] 	Memory data size 									*/
        volatile uint32_t pincos  	 	: 1 ;  	/*!< [Bit 15] 		Peripheral increment offset size 					*/
        volatile uint32_t pl			: 2 ;  	/*!< [Bits 17:16] 	Priority level 										*/
        volatile uint32_t dbm      		: 1 ;  	/*!< [Bit 18] 		Double buffer mode 									*/
        volatile uint32_t ct       		: 1 ;  	/*!< [Bit 19] 		Current target (only in double buffer mode) 		*/
        		  uint32_t reserved_0 	: 1 ;  	/*!< [Bit 20] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t pburst   		: 2 ;  	/*!< [Bits 22:21] 	Peripheral burst transfer configuration 			*/
        volatile uint32_t mburst   		: 2 ;  	/*!< [Bits 24:23] 	Memory burst transfer configuration 				*/
        volatile uint32_t chsel    		: 3 ;  	/*!< [Bits 27:25] 	Channel selection 									*/
        		  uint32_t reserved_1 	: 4 ;  	/*!< [Bits 31:28] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_SxCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ndt        	: 16 ; 	/*!< [Bits 15:0] 	Number of data items to transfer 					*/
        		  uint32_t reserved 	: 16 ;  /*!< [Bits 31:16] 	Reserved, must be kept at reset value 				*/
    	   }bit;
}DMA_SxNDTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fth     		:  2 ; 	/*!< [Bits 1:0] 		FIFO threshold selection 							*/
        volatile uint32_t dmdis   		:  1 ; 	/*!< [Bit 2] 		Direct mode disable 								*/
        volatile uint32_t fs      		:  3 ;	/*!< [Bits 5:3] 		FIFO status 										*/
        		  uint32_t reserved_0   :  1 ; 	/*!< [Bit 6] 		Reserved, must be kept at reset value 				*/
        volatile uint32_t feie    		:  1 ;	/*!< [Bit 7] 		FIFO error interrupt enable 						*/
        		  int32_t reserved_1 	: 24 ; 	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 				*/
    	  }bit;
}DMA_SxFCR_Reg_t;

typedef struct
{
DMA_LISR_Reg_t   LISR;   /*!< DMA low interrupt status register,       	Address offset: 0x00 */
DMA_HISR_Reg_t   HISR;   /*!< DMA high interrupt status register,      	Address offset: 0x04 */
DMA_LIFCR_Reg_t  LIFCR;  /*!< DMA low interrupt flag clear register,   	Address offset: 0x08 */
DMA_HIFCR_Reg_t  HIFCR;  /*!< DMA high interrupt flag clear register,  	Address offset: 0x0C */
DMA_SxCR_Reg_t	 S0CR;	 /*!< DMA stream 0 configuration register, 	   	Address offset: 0x10 */
DMA_SxNDTR_Reg_t S0NDTR; /*!< DMA stream 0 number of data register,    	Address offset: 0x14 */
uint32_t	 	 S0PAR;	 /*!< DMA stream 0 peripheral address register, Address offset: 0x18 */
uint32_t 		 S0M0AR; /*!< DMA stream 0 memory 0 address register,   Address offset: 0x1C */
uint32_t 		 S0M1AR; /*!< DMA stream 0 memory 1 address register,   Address offset: 0x20 */
DMA_SxFCR_Reg_t	 S0FCR;  /*!< DMA stream 0 FIFO control register,  	    Address offset: 0x24 */
DMA_SxCR_Reg_t	 S1CR;	 /*!< DMA stream 1 configuration register, 	  	Address offset: 0x28 */
DMA_SxNDTR_Reg_t S1NDTR; /*!< DMA stream 1 number of data register,   	Address offset: 0x2C */
uint32_t	 	 S1PAR;	 /*!< DMA stream 1 peripheral address register, Address offset: 0x30 */
uint32_t 		 S1M0AR; /*!< DMA stream 1 memory 0 address register,   Address offset: 0x34 */
uint32_t 		 S1M1AR; /*!< DMA stream 1 memory 1 address register,   Address offset: 0x38 */
DMA_SxFCR_Reg_t	 S1FCR;  /*!< DMA stream 1 FIFO control register,  	    Address offset: 0x3C */
DMA_SxCR_Reg_t	 S2CR;	 /*!< DMA stream 2 configuration register, 	  	Address offset: 0x40 */
DMA_SxNDTR_Reg_t S2NDTR; /*!< DMA stream 2 number of data register,   	Address offset: 0x44 */
uint32_t	 	 S2PAR;	 /*!< DMA stream 2 peripheral address register, Address offset: 0x48 */
uint32_t 		 S2M0AR; /*!< DMA stream 2 memory 0 address register,   Address offset: 0x4C */
uint32_t 		 S2M1AR; /*!< DMA stream 2 memory 1 address register,   Address offset: 0x50 */
DMA_SxFCR_Reg_t	 S2FCR;  /*!< DMA stream 2 FIFO control register,  	    Address offset: 0x54 */
DMA_SxCR_Reg_t	 S3CR;	 /*!< DMA stream 3 configuration register, 	  	Address offset: 0x58 */
DMA_SxNDTR_Reg_t S3NDTR; /*!< DMA stream 3 number of data register,   	Address offset: 0x5C */
uint32_t	 	 S3PAR;	 /*!< DMA stream 3 peripheral address register, Address offset: 0x60 */
uint32_t 		 S3M0AR; /*!< DMA stream 3 memory 0 address register,   Address offset: 0x64 */
uint32_t 		 S3M1AR; /*!< DMA stream 3 memory 1 address register,   Address offset: 0x68 */
DMA_SxFCR_Reg_t	 S3FCR;  /*!< DMA stream 3 FIFO control register,  	    Address offset: 0x6C */
DMA_SxCR_Reg_t	 S4CR;	 /*!< DMA stream 4 configuration register, 	  	Address offset: 0x70 */
DMA_SxNDTR_Reg_t S4NDTR; /*!< DMA stream 4 number of data register,   	Address offset: 0x74 */
uint32_t	 	 S4PAR;	 /*!< DMA stream 4 peripheral address register, Address offset: 0x78 */
uint32_t 		 S4M0AR; /*!< DMA stream 4 memory 0 address register,   Address offset: 0x7C */
uint32_t 		 S4M1AR; /*!< DMA stream 4 memory 1 address register,   Address offset: 0x80 */
DMA_SxFCR_Reg_t	 S4FCR;  /*!< DMA stream 4 FIFO control register,  	    Address offset: 0x84 */
DMA_SxCR_Reg_t	 S5CR;	 /*!< DMA stream 5 configuration register, 	  	Address offset: 0x88 */
DMA_SxNDTR_Reg_t S5NDTR; /*!< DMA stream 5 number of data register,   	Address offset: 0x8C */
uint32_t	 	 S5PAR;	 /*!< DMA stream 5 peripheral address register, Address offset: 0x90 */
uint32_t 		 S5M0AR; /*!< DMA stream 5 memory 0 address register,   Address offset: 0x94 */
uint32_t 		 S5M1AR; /*!< DMA stream 5 memory 1 address register,   Address offset: 0x98 */
DMA_SxFCR_Reg_t	 S5FCR;  /*!< DMA stream 5 FIFO control register,  	    Address offset: 0x9C */
DMA_SxCR_Reg_t	 S6CR;	 /*!< DMA stream 6 configuration register,	   	Address offset: 0xA0 */
DMA_SxNDTR_Reg_t S6NDTR; /*!< DMA stream 6 number of data register,    	Address offset: 0xA4 */
uint32_t	 	 S6PAR;	 /*!< DMA stream 6 peripheral address register, Address offset: 0xA8 */
uint32_t 		 S6M0AR; /*!< DMA stream 6 memory 0 address register,   Address offset: 0xAC */
uint32_t 		 S6M1AR; /*!< DMA stream 6 memory 1 address register,   Address offset: 0xB0 */
DMA_SxFCR_Reg_t	 S6FCR;  /*!< DMA stream 6 FIFO control register,  	    Address offset: 0xB4 */
DMA_SxCR_Reg_t	 S7CR;	 /*!< DMA stream 7 configuration register, 	  	Address offset: 0xB8 */
DMA_SxNDTR_Reg_t S7NDTR; /*!< DMA stream 7 number of data register,   	Address offset: 0xBC */
uint32_t	 	 S7PAR;	 /*!< DMA stream 7 peripheral address register, Address offset: 0xC0 */
uint32_t 		 S7M0AR; /*!< DMA stream 7 memory 0 address register,   Address offset: 0xC4 */
uint32_t 		 S7M1AR; /*!< DMA stream 7 memory 1 address register,   Address offset: 0xC8 */
DMA_SxFCR_Reg_t	 S7FCR;  /*!< DMA stream 7 FIFO control register,  	    Address offset: 0xCC */
}DMA_RegDef_t;

/**
  * @brief Ethernet MAC
  */

typedef union{
    uint32_t reg;
    struct{
    			  uint32_t reserved_1 	: 2 ;   /*!< [Bits 1:0] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t re           	: 1 ;   /*!< [Bit 2] 		Receiver enable 									*/
        volatile uint32_t te           	: 1 ;   /*!< [Bit 3] 		Transmitter enable 									*/
        volatile uint32_t dc          	: 1 ;   /*!< [Bit 4] 		Deferral check 										*/
        volatile uint32_t bl           	: 2 ;   /*!< [Bits 6:5] 		Back-off limit 										*/
        volatile uint32_t apcs         	: 1 ;   /*!< [Bit 7] 		Automatic pad/CRC stripping 						*/
        		  uint32_t reserved_2   : 1 ;   /*!< [Bit 8] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t rd           	: 1 ;   /*!< [Bit 9] 		Retry disable 										*/
        volatile uint32_t ipco         	: 1 ;   /*!< [Bit 10] 		IPv4 checksum offload 								*/
        volatile uint32_t dm           	: 1 ;   /*!< [Bit 11] 		Duplex mode 										*/
        volatile uint32_t lm           	: 1 ;   /*!< [Bit 12] 		Loopback mode 										*/
        volatile uint32_t rod          	: 1 ;   /*!< [Bit 13] 		Receive own disable 								*/
        volatile uint32_t fes          	: 1 ;   /*!< [Bit 14] 		Fast Ethernet speed 								*/
        		  uint32_t reserved_3   : 1 ;   /*!< [Bit 15] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t csd          	: 1 ;   /*!< [Bit 16] 		Carrier sense disable 								*/
        volatile uint32_t ifg          	: 3 ;   /*!< [Bits 19:17] 	Interframe gap 										*/
        		  uint32_t reserved_4   : 2 ;   /*!< [Bits 21:20] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t jd           	: 1 ;   /*!< [Bit 22] 		Jabber disable 										*/
        volatile uint32_t wd           	: 1 ;   /*!< [Bit 23] 		Watchdog disable 									*/
        		  uint32_t reserved_5   : 1 ;   /*!< [Bit 24] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t cstf         	: 1 ;   /*!< [Bit 25] 		CSTF 												*/
        		  uint32_t reserved_6   : 6 ;   /*!< [Bits 31:26] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pm     		:  1 ;	/*!< [Bit 0] 		Promiscuous mode 									*/
        volatile uint32_t hu     		:  1 ; 	/*!< [Bit 1] 		Hash unicast 										*/
        volatile uint32_t hm     		:  1 ;	/*!< [Bit 2] 		Hash multicast 										*/
        volatile uint32_t daif   		:  1 ;	/*!< [Bit 3] 		Destination address inverse filtering 				*/
        volatile uint32_t pam    		:  1 ;	/*!< [Bit 4] 		Pass all multicast 									*/
        volatile uint32_t bfd    		:  1 ;	/*!< [Bit 5] 		Broadcast frames disable 							*/
        volatile uint32_t pcf    		:  2 ;	/*!< [Bits 7:6] 		Pass control frames 								*/
        volatile uint32_t saif   		:  1 ;	/*!< [Bit 8] 		Source address inverse filtering 					*/
        volatile uint32_t saf    		:  1 ;	/*!< [Bit 9] 		Source address filter 								*/
        volatile uint32_t hpf    		:  1 ;	/*!< [Bit 10] 		Hash or perfect filter 								*/
        		  uint32_t reserved	 	: 20 ;	/*!< [Bits 30:11] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t ra          	:  1 ;	/*!< [Bit 31] 		Receive all 										*/
    	   }bit;
}ETH_MACFFR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t mb         	:  1 ;	/*!< [Bit 0] 		MII busy 											*/
        volatile uint32_t mw         	:  1 ;	/*!< [Bit 1] 		MII write 											*/
        volatile uint32_t cr         	:  3 ;	/*!< [Bits 4:2] 		Clock range 										*/
        		  uint32_t reserved_0	:  1 ;	/*!< [Bit 5] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t mr         	:  5 ;	/*!< [Bits 10:6] 	MII register 										*/
        volatile uint32_t pa         	:  5 ;	/*!< [Bits 15:11] 	PHY address 										*/
        		  uint32_t reserved_1	: 16 ;	/*!< [Bits 31:16] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACMIIAR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t md         	: 16 ; 	/*!< [Bits 15:0] 	MII data 											*/
        		  uint32_t reserved		: 16 ;  /*!< [Bits 31:16] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACMIIDR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fcb_bpa     	:  1 ;	/*!< [Bit 0] 		Flow control busy/back pressure activate 			*/
        volatile uint32_t tfce       	:  1 ;	/*!< [Bit 1] 		Transmit flow control enable 						*/
        volatile uint32_t rfce       	:  1 ;	/*!< [Bit 2] 		Receive flow control enable 						*/
        volatile uint32_t upfd       	:  1 ;	/*!< [Bit 3] 		Unicast pause frame detect 							*/
        		  uint32_t reserved_0	:  2 ;	/*!< [Bits 5:4] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t plt        	:  2 ;	/*!< [Bits 5:4] 		Pause low threshold 								*/
        		  uint32_t reserved_1	:  1 ;	/*!< [Bit 6] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t zqpd       	:  1 ;	/*!< [Bit 7] 		Zero-quanta pause disable 							*/
        		  uint32_t reserved_2	:  8 ;	/*!< [Bits 15:8] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t pt         	: 16 ;	/*!< [Bits 31:16] 	Pause time 											*/
    	   }bit;
}ETH_MACFCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t vlanti     	: 16 ;  /*!< [Bits 15:0] 	VLAN tag identifier (for receive frames) 			*/
        volatile uint32_t vlantc     	:  1 ;  /*!< [Bit 16] 		12-bit VLAN tag comparison 							*/
        		  uint32_t reserved		: 15 ;  /*!< [Bits 31:17] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACVLANTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pd        	:  1 ;	/*!< [Bit 0] 		Power down 											*/
        volatile uint32_t mpe       	:  1 ;  /*!< [Bit 1] 		Magic Packet enable 								*/
        volatile uint32_t wfe       	:  1 ;  /*!< [Bit 2] 		Wakeup frame enable 								*/
        		  uint32_t reserved_0	:  2 ;  /*!< [Bits 4:3] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t mpr       	:  1 ;  /*!< [Bit 5] 		Magic packet received 								*/
        volatile uint32_t wfr       	:  1 ;  /*!< [Bit 6] 		Wakeup frame received 								*/
        		  uint32_t reserved_1	:  2 ;  /*!< [Bits 8:7] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t gu        	:  1 ;  /*!< [Bit 9] 		Global unicast 										*/
        		  uint32_t reserved_2	: 21 ;  /*!< [Bits 30:10] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t wffrpr    	:  1 ;  /*!< [Bit 31] 		Wakeup frame filter register pointer reset 			*/
    	   }bit;
}ETH_MACPMTCSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t mmrpea    	: 1 ;   /*!< [Bit 0] 		MAC MII receive protocol engine active 				*/
        volatile uint32_t msfrwcs		: 2 ;   /*!< [Bits 2:1] 		MAC small FIFO read / write controllers status 		*/
        		  uint32_t reserved_0	: 1 ;   /*!< [Bit 3] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t rfwra    		: 1 ;   /*!< [Bit 4] 		Rx FIFO write controller active 					*/
        volatile uint32_t rfrcs    		: 2 ;   /*!< [Bits 6:5] 		Rx FIFO read controller status 						*/
        		  uint32_t reserved_1	: 1 ;   /*!< [Bit 7] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t rffl     		: 2 ;   /*!< [Bits 9:8] 		Rx FIFO fill level 									*/
        		  uint32_t reserved_2	: 6 ;   /*!< [Bits 15:10] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t mmtea    		: 1 ;   /*!< [Bit 16] 		MAC MII transmit engine active 						*/
        volatile uint32_t mtfcs    		: 2 ;   /*!< [Bits 18:17] 	MAC transmit frame controller status 				*/
        volatile uint32_t mtp      		: 1 ;   /*!< [Bit 19] 		MAC transmitter in pause 							*/
        volatile uint32_t tfrs     		: 2 ;   /*!< [Bits 21:20] 	Tx FIFO read status 								*/
        volatile uint32_t tfwa     		: 1 ;   /*!< [Bit 22] 		Tx FIFO write active 								*/
        		  uint32_t reserved_3	: 1 ;   /*!< [Bit 23] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t tfne     		: 1 ;   /*!< [Bit 24] 		Tx FIFO not empty 									*/
        volatile uint32_t tffrpr   		: 1 ;   /*!< [Bit 25] 		Tx FIFO full 										*/
        		  uint32_t reserved_4	: 6 ;   /*!< [Bits 31:26] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACDBGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
		  	  	  uint32_t reserved_0	: 2 ;   /*!< [Bits 2:0] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t pmts      	: 1 ;   /*!< [Bit 3] 		PMT status 											*/
        volatile uint32_t mmcs     		: 1 ;   /*!< [Bit 4] 		MMC status 											*/
        volatile uint32_t mmcrs    		: 1 ;   /*!< [Bit 5] 		MMC receive status 									*/
        volatile uint32_t mmcts    		: 1 ;   /*!< [Bit 6] 		MMC transmit status 								*/
        		  uint32_t reserved_1	: 2 ;   /*!< [Bits 8:7] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t tsts     		: 1 ;   /*!< [Bit 9] 		Time stamp trigger status 							*/
        		  uint32_t reserved_2	: 6 ;   /*!< [Bits 15:10] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	: 2 ;   /*!< [Bits 2:0] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t pmtim 		: 1 ;   /*!< [Bit 3] 		PMT interrupt mask 									*/
        		  uint32_t reserved_1	: 5 ;   /*!< [Bits 8:4] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t tstim 		: 1 ;   /*!< [Bit 9] 		Time stamp trigger interrupt mask 					*/
        		  uint32_t reserved_2	: 6 ;   /*!< [Bits 15:10] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MACIMR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t maca0h  		: 16 ;  /*!< [Bits 15:0] 	MAC address0 high [47:32] 							*/
        		  uint32_t reserved		: 15 ;  /*!< [Bits 30:16] 	Reserved, must be kept at reset value. 				*/
        		  uint32_t mo			:  1 ;  /*!< [Bit 31] 		Always 1. 											*/
    	   }bit;
}ETH_MACA0HR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t maca_x_h     	: 16 ;	/*!< [Bits 15:0] 	MAC address_X high [47:32] 							*/
        		  uint32_t reserved		:  8 ;  /*!< [Bits 23:16] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t mbc         	:  6 ;  /*!< [Bits 29:24] 	Mask byte control 									*/
        volatile uint32_t sa          	:  1 ;  /*!< [Bit 30] 		Source address 										*/
        volatile uint32_t ae          	:  1 ;  /*!< [Bit 31] 		Address enable 										*/
    	   }bit;
}ETH_MACAxHR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cr        	:  1 ;   /*!< [Bit 0]  		Counter reset 										*/
        volatile uint32_t csr       	:  1 ;   /*!< [Bit 1]  		Counter stop rollover 								*/
        volatile uint32_t ror        	:  1 ;   /*!< [Bit 2]  		Reset on read 										*/
        volatile uint32_t mcf       	:  1 ;   /*!< [Bit 3]  		MMC counter freeze 									*/
        volatile uint32_t mcp        	:  1 ;   /*!< [Bit 4]  		MMC counter preset 									*/
        volatile uint32_t mcfhp      	:  1 ;   /*!< [Bit 5]  		MMC counter Full-Half preset 						*/
        		  uint32_t reserved		: 26 ;   /*!< [Bits 31:6] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MMCCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	:  5 ;   /*!< [Bits 4:0] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t rfces       	:  1 ;   /*!< [Bit 5]  		Received frames CRC error status 					*/
        volatile uint32_t rfaes       	:  1 ;   /*!< [Bit 6]  		Received frames alignment error status 				*/
		  	  	  uint32_t reserved_1	: 10 ;   /*!< [Bits 16:7] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t rgufs        	:  1 ;   /*!< [Bit 17] 		Received Good Unicast Frames Status 				*/
        		  uint32_t reserved_2	: 10 ;   /*!< [Bits 31:18] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MMCRIR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	: 14 ;  /*!< [Bits 13:0] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t tgfscs       	:  1 ;  /*!< [Bit 14] 		Transmitted good frames single collision status 	*/
        volatile uint32_t tgfmscs     	:  1 ;  /*!< [Bit 15] 		Transmitted good frames more single collision status*/
        		  uint32_t reserved_1	:  5 ;  /*!< [Bits 20:16] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t tgfs        	:  1 ;  /*!< [Bit 21] 		Transmitted good frames status 						*/
        		  uint32_t reserved_2	: 10 ;  /*!< [Bits 31:22] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MMCTIR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	:  5 ;  /*!< [Bits 4:0] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t rfcem       	:  1 ;  /*!< [Bit 5] 		Received frame CRC error mask 						*/
        volatile uint32_t rfaem       	:  1 ;  /*!< [Bit 6] 		Received frames alignment error mask 				*/
        		  uint32_t reserved_1	: 10 ;  /*!< [Bits 16:7] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t rgufm       	:  1 ;  /*!< [Bit 17] 		Received good unicast frames mask 					*/
        		  uint32_t reserved_2	: 14 ;  /*!< [Bits 31:18] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_MMCRIMR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	: 14 ;  /*!< [Bits 13:0] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t tgfscm      	:  1 ;  /*!< [Bit 14] 		Transmitted good frames single collision mask 		*/
        volatile uint32_t tgfmscm    	:  1 ;  /*!< [Bit 15] 		Transmitted good frames more single collision mask 	*/
        		  uint32_t reserved_1	:  5 ;  /*!< [Bits 20:16] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t tgfm       	:  1 ;  /*!< [Bit 21] 		Transmitted good frames mask 						*/
        		  uint32_t reserved_2	: 10 ;  /*!< [Bits 31:22] 	Reserved, must be kept at reset value. 				*/
    }bit;
}ETH_MMCTIMR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tse      		:  1 ; 	/*!< [Bit 0] Time stamp enable 											*/
        volatile uint32_t tsfcu    		:  1 ;  /*!< [Bit 1] Time stamp fine or coarse update 							*/
        volatile uint32_t tssti       	:  1 ;  /*!< [Bit 2] Time stamp system time initialize 							*/
        volatile uint32_t tsstu        	:  1 ;  /*!< [Bit 3] Time stamp system time update 								*/
        volatile uint32_t tsite       	:  1 ;  /*!< [Bit 4] Time stamp interrupt trigger enable 						*/
        volatile uint32_t tsaru       	:  1 ;  /*!< [Bit 5] Time stamp addend register update 							*/
        		  uint32_t reserved_0	:  2 ;  /*!< [Bits 7:6] Reserved, must be kept at reset value. 					*/
        volatile uint32_t tsarfe       	:  1 ;  /*!< [Bit 8] Time stamp snapshot for all received frames enable 			*/
        volatile uint32_t tsssr       	:  1 ;  /*!< [Bit 9] Time stamp subsecond rollover: digital or binary rollover control */
        volatile uint32_t tsptppsv2e  	:  1 ;  /*!< [Bit 10] Time stamp PTP packet snooping for version2 format enable */
        volatile uint32_t tssptpoefe 	:  1 ;  /*!< [Bit 11] Time stamp snapshot for PTP over ethernet frames enable 	*/
        volatile uint32_t tssipv6fe   	:  1 ;  /*!< [Bit 12] Time stamp snapshot for IPv6 frames enable 				*/
        volatile uint32_t tssipv4fe 	:  1 ;  /*!< [Bit 13] Time stamp snapshot for IPv4 frames enable 				*/
        volatile uint32_t tsseme   		:  1 ;  /*!< [Bit 14] Time stamp snapshot for event message enable 				*/
        volatile uint32_t tssmrme   	:  1 ;  /*!< [Bit 15] Time stamp snapshot for message relevant to master enable */
        volatile uint32_t tscnt   		:  2 ;  /*!< [Bits 17:16] Time stamp clock node type 							*/
        volatile uint32_t tspffmae   	:  1 ;  /*!< [Bit 18] Time stamp PTP frame filtering MAC address enable 			*/
        		  uint32_t reserved_1	: 12 ;  /*!< [Bits 31:19] Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_PTPTSCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t stssi   		:  8 ;  /*!< [Bits 7:0] 		System time subsecond increment 					*/
        		  uint32_t reserved		: 24 ;  /*!< [Bits 31:8] 	Reserved, must be kept at reset value. 				*/
    }bit;
}ETH_PTPSSIR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t stss      	: 31 ;   /*!< [Bits 30:0] 	System time subseconds 								*/
        volatile uint32_t stpns     	:  1 ;   /*!< [Bit 31] 		System time positive or negative sign 				*/
    	   }bit;
}ETH_PTPTSLR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tsuss     	: 31 ;   /*!< [Bits 30:0] 	Time stamp update subseconds 						*/
        volatile uint32_t tsupns    	:  1 ;   /*!< [Bit 31] 		Time stamp update positive or negative sign 		*/
    	  }bit;
}ETH_PTPTSLUR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tsso      	:  1 ;   /*!< [Bit 0] 		Time stamp second overflow 							*/
        volatile uint32_t tsttr     	:  1 ;   /*!< [Bit 1] 		Time stamp target time reached 						*/
        		  uint32_t reserved   	: 30 ;   /*!< [Bits 31:2] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_PTPTSSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sr     		: 1 ;    /*!< [Bit 0] 		Software reset 										*/
        volatile uint32_t da     		: 1 ;    /*!< [Bit 1] 		DMA Arbitration 									*/
        volatile uint32_t dsl    		: 5 ;    /*!< [Bits 6:2] 	Descriptor skip length 								*/
        volatile uint32_t edfe   		: 1 ;    /*!< [Bit 7] 		Enhanced descriptor format enable 					*/
        volatile uint32_t pbl      		: 6 ;    /*!< [Bits 13:8] 	Programmable burst length 							*/
        volatile uint32_t pm        	: 2 ;    /*!< [Bits 15:14] 	Rx Tx priority ratio 								*/
        volatile uint32_t fb     		: 1 ;    /*!< [Bit 16] 		Fixed burst 										*/
        volatile uint32_t rdp    		: 6 ;    /*!< [Bits 22:17] 	Rx DMA PBL 											*/
        volatile uint32_t usp    		: 1 ;    /*!< [Bit 23] 		Use separate PBL 									*/
        volatile uint32_t fpm    		: 1 ;    /*!< [Bit 24] 		4xPBL mode 											*/
        volatile uint32_t aab    		: 1 ;    /*!< [Bit 25] 		Address-aligned beats 								*/
        volatile uint32_t mb     		: 1 ;    /*!< [Bit 26] 		Mixed burst 										*/
        		  uint32_t reserved 	: 5 ;    /*!< [Bits 31:27] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_DMABMR_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t ts      		: 1 ;    /*!< [Bit 0] 		Transmit status 									*/
        volatile uint32_t tpss    		: 1 ;    /*!< [Bit 1] 		Transmit process stopped status 					*/
        volatile uint32_t tbus    		: 1 ;    /*!< [Bit 2] 		Transmit buffer unavailable status 					*/
        volatile uint32_t tjts    		: 1 ;    /*!< [Bit 3] 		Transmit jabber timeout status 						*/
        volatile uint32_t ros     		: 1 ;    /*!< [Bit 4] 		Receive overflow status 							*/
        volatile uint32_t tus     		: 1 ;    /*!< [Bit 5] 		Transmit underflow status 							*/
        volatile uint32_t rs      		: 1 ;    /*!< [Bit 6] 		Receive status 										*/
        volatile uint32_t rbus    		: 1 ;    /*!< [Bit 7] 		Receive buffer unavailable status 					*/
        volatile uint32_t rpss    		: 1 ;    /*!< [Bit 8] 		Receive process stopped status 						*/
        volatile uint32_t rwts    		: 1 ;    /*!< [Bit 9] 		Receive watchdog timeout status 					*/
        volatile uint32_t ets     		: 1 ;    /*!< [Bit 10] 		Early transmit status 								*/
        		  uint32_t reserved_0 	: 2 ;    /*!< [Bits 12:11] 	Reserved, must be kept at reset value. 				*/
        volatile uint32_t fbes    		: 1 ;    /*!< [Bit 13] 		Fatal bus error status 								*/
        volatile uint32_t ers     		: 1 ;    /*!< [Bit 14] 		Early receive status 								*/
        volatile uint32_t ais     		: 1 ;    /*!< [Bit 15] 		Abnormal interrupt summary 							*/
        volatile uint32_t nis     		: 1 ;    /*!< [Bit 16] 		Normal interrupt summary 							*/
        volatile uint32_t rps     		: 3 ;    /*!< [Bits 19:17] 	Receive process state 								*/
        volatile uint32_t tps       	: 3 ;    /*!< [Bits 22:20] 	Transmit process state 								*/
        volatile uint32_t ebs        	: 3 ;    /*!< [Bits 25:23] 	Error bits status 									*/
        		  uint32_t reserved_1 	: 1 ;    /*!< [Bit 26] 		Reserved, must be kept at reset value. 				*/
        volatile uint32_t mmcs   		: 1 ;    /*!< [Bit 27] 		MMC status 											*/
        volatile uint32_t pmts   		: 1 ;    /*!< [Bit 28] 		PMT status 											*/
        volatile uint32_t tsts   		: 1 ;    /*!< [Bit 29] 		Time stamp trigger status 							*/
        		  uint32_t reserved_2 	: 2 ;    /*!< [Bits 31:30] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_DMASR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
				  uint32_t reserved_0  	: 1 ;    /*!< [Bit 0] 		Reserved, must be kept at reset value. 				*/
		volatile uint32_t sr          	: 1 ;    /*!< [Bit 1] 		Start/stop receive 									*/
		volatile uint32_t osf         	: 1 ;    /*!< [Bit 2] 		Operate on second frame 							*/
		volatile uint32_t rtc   		: 2 ;    /*!< [Bits 4:3] 		Receive threshold control 							*/
				  uint32_t reserved_1   : 1 ;    /*!< [Bit 5] 		Reserved, must be kept at reset value. 				*/
		volatile uint32_t fugf        	: 1 ;    /*!< [Bit 6] 		Forward undersized good frames 						*/
		volatile uint32_t fef         	: 1 ;    /*!< [Bit 7] 		Forward error frames 								*/
				  uint32_t reserved_2  	: 5 ;    /*!< [Bits 12:8] 	Reserved, must be kept at reset value. 				*/
		volatile uint32_t st         	: 1 ;    /*!< [Bit 13] 		Start/stop transmission 							*/
		volatile uint32_t ttc    		: 3 ;    /*!< [Bits 16:14] 	Transmit threshold control 							*/
				  uint32_t reserved_3   : 3 ;    /*!< [Bits 19:17] 	Reserved, must be kept at reset value. 				*/
		volatile uint32_t ftf        	: 1 ;    /*!< [Bit 20] 		Flush transmit FIFO 								*/
		volatile uint32_t tsf        	: 1 ;    /*!< [Bit 21] 		Transmit store and forward 							*/
				  uint32_t reserved_4  	: 2 ;    /*!< [Bits 23:22] 	Reserved, must be kept at reset value. 				*/
		volatile uint32_t dfrf       	: 1 ;    /*!< [Bit 24] 		Disable flushing of received frames 				*/
		volatile uint32_t rsf        	: 1 ;    /*!< [Bit 25] 		Receive store and forward 							*/
		volatile uint32_t dtcefd     	: 1 ;    /*!< [Bit 26] 		Dropping of TCP/IP checksum error frames disable 	*/
				  uint32_t reserved_5   : 5 ;    /*!< [Bits 31:27] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_DMAOMR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tie      		:  1 ;    /*!< [Bit 0] 		Transmit interrupt enable 							*/
        volatile uint32_t tpsie    		:  1 ;    /*!< [Bit 1] 		Transmit process stopped interrupt enable 			*/
        volatile uint32_t tbuie    		:  1 ;    /*!< [Bit 2] 		Transmit buffer unavailable interrupt enable 		*/
        volatile uint32_t tjtie    		:  1 ;    /*!< [Bit 3] 		Transmit jabber timeout interrupt enable 			*/
        volatile uint32_t roie     		:  1 ;    /*!< [Bit 4] 		Overflow interrupt enable 							*/
        volatile uint32_t tuie     		:  1 ;    /*!< [Bit 5] 		Underflow interrupt enable 							*/
        volatile uint32_t rie      		:  1 ;    /*!< [Bit 6] 		Receive interrupt enable 							*/
        volatile uint32_t rbuie    		:  1 ;    /*!< [Bit 7] 		Receive buffer unavailable interrupt enable 		*/
        volatile uint32_t rpsie    		:  1 ;    /*!< [Bit 8] 		Receive process stopped interrupt enable 			*/
        volatile uint32_t rwtie    		:  1 ;    /*!< [Bit 9] 		Receive watchdog timeout interrupt enable 			*/
        volatile uint32_t etie     		:  1 ;    /*!< [Bit 10] 		Early transmit interrupt enable 					*/
        		  uint32_t reserved_0   :  2 ;    /*!< [Bits 12:11] Reserved, must be kept at reset value. 				*/
        volatile uint32_t fbeie   		:  1 ;    /*!< [Bit 13] 		Fatal bus error interrupt enable 					*/
        volatile uint32_t erie    		:  1 ;    /*!< [Bit 14] 		Early receive interrupt enable 						*/
        volatile uint32_t aise    		:  1 ;    /*!< [Bit 15] 		Abnormal interrupt summary enable 					*/
        volatile uint32_t nise    		:  1 ;    /*!< [Bit 16] 		Normal interrupt summary enable 					*/
        		  uint32_t reserved_1 	: 15 ;    /*!< [Bits 31:17] Reserved, must be kept at reset value. 				*/
    	  }bit;
}ETH_DMAIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t mfc     		: 16 ;   /*!< [Bits 15:0] 	Missed frames by the controller 					*/
        volatile uint32_t omfc    		:  1 ;   /*!< [Bit 16] 		Overflow bit for missed frame counter 				*/
        volatile uint32_t mfa 			: 11 ;   /*!< [Bits 27:17] 	Missed frames by the application. 					*/
        volatile uint32_t ofoc    		:  1 ;   /*!< [Bit 28] 		Overflow bit for FIFO overflow counter 				*/
        		  uint32_t reserved		:  3 ;   /*!< [Bits 31:29] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_DMAMFBOCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t rswtc   		:  8 ;    /*!< [Bits 7:0] 	Receive status (RS) watchdog timer count 			*/
        		  uint32_t reserved    	: 24 ;    /*!< [Bits 31:8] 	Reserved, must be kept at reset value. 				*/
    	   }bit;
}ETH_DMARSWTR_Reg_t;

typedef struct
{
ETH_MACCR_Reg_t 	MACCR;
ETH_MACFFR_Reg_t 	MACFFR;
volatile uint32_t 	MACHTHR;
volatile uint32_t 	MACHTLR;
ETH_MACMIIAR_Reg_t 	MACMIIAR;
ETH_MACMIIDR_Reg_t 	MACMIIDR;
ETH_MACFCR_Reg_t 	MACFCR;
ETH_MACVLANTR_Reg_t MACVLANTR;             /*    8 */
  uint32_t      	RESERVED0[2];
volatile uint32_t 	MACRWUFFR;             /*   11 */
ETH_MACPMTCSR_Reg_t MACPMTCSR;
  uint32_t      	RESERVED1;
ETH_MACDBGR_Reg_t 	MACDBGR;
ETH_MACSR_Reg_t 	MACSR;                 /*   15 */
ETH_MACIMR_Reg_t 	MACIMR;
ETH_MACA0HR_Reg_t 	MACA0HR;
volatile uint32_t 	MACA0LR;
ETH_MACAxHR_Reg_t 	MACA1HR;
volatile uint32_t 	MACA1LR;
ETH_MACAxHR_Reg_t 	MACA2HR;
volatile uint32_t 	MACA2LR;
ETH_MACAxHR_Reg_t 	MACA3HR;
volatile uint32_t 	MACA3LR;               /*   24 */
  uint32_t      	RESERVED2[40];
ETH_MMCCR_Reg_t 	MMCCR;                 /*   65 */
ETH_MMCRIR_Reg_t 	MMCRIR;
ETH_MMCTIR_Reg_t 	MMCTIR;
ETH_MMCRIMR_Reg_t 	MMCRIMR;
ETH_MMCTIMR_Reg_t 	MMCTIMR;               /*   69 */
  uint32_t      	RESERVED3[14];
volatile uint32_t 	MMCTGFSCCR;            /*   84 */
volatile uint32_t 	MMCTGFMSCCR;
  uint32_t      	RESERVED4[5];
volatile uint32_t 	MMCTGFCR;
  uint32_t      	RESERVED5[10];
volatile uint32_t 	MMCRFCECR;
volatile uint32_t 	MMCRFAECR;
  uint32_t      	RESERVED6[10];
volatile uint32_t 	MMCRGUFCR;
  uint32_t      	RESERVED7[334];
ETH_PTPTSCR_Reg_t 	PTPTSCR;
ETH_PTPSSIR_Reg_t 	PTPSSIR;
volatile uint32_t 	PTPTSHR;
ETH_PTPTSLR_Reg_t 	PTPTSLR;
volatile uint32_t  	PTPTSHUR;
ETH_PTPTSLUR_Reg_t 	PTPTSLUR;
volatile uint32_t 	PTPTSAR;
volatile uint32_t 	PTPTTHR;
volatile uint32_t 	PTPTTLR;
volatile uint32_t 	RESERVED8;
ETH_PTPTSSR_Reg_t 	PTPTSSR;
  uint32_t      	RESERVED9[565];
ETH_DMABMR_Reg_t 	DMABMR;
volatile uint32_t 	DMATPDR;
volatile uint32_t 	DMARPDR;
volatile uint32_t 	DMARDLAR;
volatile uint32_t 	DMATDLAR;
ETH_DMASR_Reg_t 	DMASR;
ETH_DMAOMR_Reg_t 	DMAOMR;
ETH_DMAIER_Reg_t 	DMAIER;
ETH_DMAMFBOCR_Reg_t DMAMFBOCR;
ETH_DMARSWTR_Reg_t 	DMARSWTR;
  uint32_t      	RESERVED10[8];
volatile uint32_t 	DMACHTDR;
volatile uint32_t 	DMACHRDR;
volatile uint32_t 	DMACHTBAR;
volatile uint32_t 	DMACHRBAR;
}ETH_RegDef_t;

/**
  * @brief External Interrupt/Event Controller
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t mr0     		: 1 ;     /*!< [Bit 0] 		Interrupt or Event mask on line 0 					*/
        volatile uint32_t mr1     		: 1 ;     /*!< [Bit 1] 		Interrupt or Event mask on line 1 					*/
        volatile uint32_t mr2     		: 1 ;     /*!< [Bit 2] 		Interrupt or Event mask on line 2 					*/
        volatile uint32_t mr3     		: 1 ;     /*!< [Bit 3] 		Interrupt or Event mask on line 3 					*/
        volatile uint32_t mr4     		: 1 ;     /*!< [Bit 4] 		Interrupt or Event mask on line 4 					*/
        volatile uint32_t mr5     		: 1 ;     /*!< [Bit 5] 		Interrupt or Event mask on line 5 					*/
        volatile uint32_t mr6     		: 1 ;     /*!< [Bit 6] 		Interrupt or Event mask on line 6 					*/
        volatile uint32_t mr7     		: 1 ;     /*!< [Bit 7] 		Interrupt or Event mask on line 7 					*/
        volatile uint32_t mr8     		: 1 ;     /*!< [Bit 8] 		Interrupt or Event mask on line 8 					*/
        volatile uint32_t mr9     		: 1 ;     /*!< [Bit 9] 		Interrupt or Event mask on line 9 					*/
        volatile uint32_t mr10    		: 1 ;     /*!< [Bit 10] 		Interrupt or Event mask on line 10 					*/
        volatile uint32_t mr11    		: 1 ;     /*!< [Bit 11] 		Interrupt or Event mask on line 11 					*/
        volatile uint32_t mr12    		: 1 ;     /*!< [Bit 12] 		Interrupt or Event mask on line 12 					*/
        volatile uint32_t mr13    		: 1 ;     /*!< [Bit 13] 		Interrupt or Event mask on line 13 					*/
        volatile uint32_t mr14    		: 1 ;     /*!< [Bit 14] 		Interrupt or Event mask on line 14 					*/
        volatile uint32_t mr15    		: 1 ;     /*!< [Bit 15] 		Interrupt or Event mask on line 15 					*/
        volatile uint32_t mr16    		: 1 ;     /*!< [Bit 16] 		Interrupt or Event mask on line 16 					*/
        volatile uint32_t mr17    		: 1 ;     /*!< [Bit 17] 		Interrupt or Event mask on line 17 					*/
        volatile uint32_t mr18    		: 1 ;     /*!< [Bit 18] 		Interrupt or Event mask on line 18 					*/
        volatile uint32_t mr19    		: 1 ;     /*!< [Bit 19] 		Interrupt or Event mask on line 19 					*/
        volatile uint32_t mr20    		: 1 ;     /*!< [Bit 20] 		Interrupt or Event mask on line 20 					*/
        volatile uint32_t mr21    		: 1 ;     /*!< [Bit 21] 		Interrupt or Event mask on line 21 					*/
        volatile uint32_t mr22    		: 1 ;     /*!< [Bit 22] 		Interrupt or Event mask on line 22 					*/
        		  uint32_t reserved 	: 9 ;     /*!< [Bits 31:23] Reserved, must be kept at reset value. 				*/
    }bit;
}EXTI_Mask_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tr0     		: 1 ;     /*!< [Bit 0] Rising or Falling trigger event configuration bit of line 0 */
        volatile uint32_t tr1     		: 1 ;     /*!< [Bit 1] Rising or Falling trigger event configuration bit of line 1 */
        volatile uint32_t tr2     		: 1 ;     /*!< [Bit 2] Rising or Falling trigger event configuration bit of line 2 */
        volatile uint32_t tr3     		: 1 ;     /*!< [Bit 3] Rising or Falling trigger event configuration bit of line 3 */
        volatile uint32_t tr4     		: 1 ;     /*!< [Bit 4] Rising or Falling trigger event configuration bit of line 4 */
        volatile uint32_t tr5     		: 1 ;     /*!< [Bit 5] Rising or Falling trigger event configuration bit of line 5 */
        volatile uint32_t tr6     		: 1 ;     /*!< [Bit 6] Rising or Falling trigger event configuration bit of line 6 */
        volatile uint32_t tr7     		: 1 ;     /*!< [Bit 7] Rising or Falling trigger event configuration bit of line 7 */
        volatile uint32_t tr8     		: 1 ;     /*!< [Bit 8] Rising or Falling trigger event configuration bit of line 8 */
        volatile uint32_t tr9     		: 1 ;     /*!< [Bit 9] Rising or Falling trigger event configuration bit of line 9 */
        volatile uint32_t tr10    		: 1 ;     /*!< [Bit 10] Rising or Falling trigger event configuration bit of line 10 */
        volatile uint32_t tr11    		: 1 ;     /*!< [Bit 11] Rising or Falling trigger event configuration bit of line 11 */
        volatile uint32_t tr12    		: 1 ;     /*!< [Bit 12] Rising or Falling trigger event configuration bit of line 12 */
        volatile uint32_t tr13    		: 1 ;     /*!< [Bit 13] Rising or Falling trigger event configuration bit of line 13 */
        volatile uint32_t tr14    		: 1 ;     /*!< [Bit 14] Rising or Falling trigger event configuration bit of line 14 */
        volatile uint32_t tr15    		: 1 ;     /*!< [Bit 15] Rising or Falling trigger event configuration bit of line 15 */
        volatile uint32_t tr16    		: 1 ;     /*!< [Bit 16] Rising or Falling trigger event configuration bit of line 16 */
        volatile uint32_t tr17    		: 1 ;     /*!< [Bit 17] Rising or Falling trigger event configuration bit of line 17 */
        volatile uint32_t tr18    		: 1 ;     /*!< [Bit 18] Rising or Falling trigger event configuration bit of line 18 */
        volatile uint32_t tr19    		: 1 ;     /*!< [Bit 19] Rising or Falling trigger event configuration bit of line 19 */
        volatile uint32_t tr20    		: 1 ;     /*!< [Bit 20] Rising or Falling trigger event configuration bit of line 20 */
        volatile uint32_t tr21    		: 1 ;     /*!< [Bit 21] Rising or Falling trigger event configuration bit of line 21 */
        volatile uint32_t tr22    		: 1 ;     /*!< [Bit 22] Rising or Falling trigger event configuration bit of line 22 */
        		  uint32_t reserved     : 9 ;     /*!< [Bits 31:23] Reserved, must be kept at reset value. 				  */
    	   }bit;
}EXTI_TSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t swier0  		: 1 ;     /*!< [Bit 0] 		Software Interrupt on line 0 						*/
        volatile uint32_t swier1  		: 1 ;     /*!< [Bit 1] 		Software Interrupt on line 1 						*/
        volatile uint32_t swier2  		: 1 ;     /*!< [Bit 2] 		Software Interrupt on line 2 						*/
        volatile uint32_t swier3  		: 1 ;     /*!< [Bit 3] 		Software Interrupt on line 3 						*/
        volatile uint32_t swier4  		: 1 ;     /*!< [Bit 4] 		Software Interrupt on line 4 						*/
        volatile uint32_t swier5  		: 1 ;     /*!< [Bit 5] 		Software Interrupt on line 5 						*/
        volatile uint32_t swier6  		: 1 ;     /*!< [Bit 6] 		Software Interrupt on line 6 						*/
        volatile uint32_t swier7  		: 1 ;     /*!< [Bit 7] 		Software Interrupt on line 7 						*/
        volatile uint32_t swier8  		: 1 ;     /*!< [Bit 8] 		Software Interrupt on line 8 						*/
        volatile uint32_t swier9  		: 1 ;     /*!< [Bit 9] 		Software Interrupt on line 9 						*/
        volatile uint32_t swier10 		: 1 ;     /*!< [Bit 10] 		Software Interrupt on line 10 						*/
        volatile uint32_t swier11 		: 1 ;     /*!< [Bit 11] 		Software Interrupt on line 11 						*/
        volatile uint32_t swier12 		: 1 ;     /*!< [Bit 12] 		Software Interrupt on line 12 						*/
        volatile uint32_t swier13 		: 1 ;     /*!< [Bit 13] 		Software Interrupt on line 13 						*/
        volatile uint32_t swier14 		: 1 ;     /*!< [Bit 14] 		Software Interrupt on line 14 						*/
        volatile uint32_t swier15 		: 1 ;     /*!< [Bit 15] 		Software Interrupt on line 15 						*/
        volatile uint32_t swier16 		: 1 ;     /*!< [Bit 16] 		Software Interrupt on line 16 						*/
        volatile uint32_t swier17 		: 1 ;     /*!< [Bit 17] 		Software Interrupt on line 17 						*/
        volatile uint32_t swier18 		: 1 ;     /*!< [Bit 18] 		Software Interrupt on line 18 						*/
        volatile uint32_t swier19 		: 1 ;     /*!< [Bit 19] 		Software Interrupt on line 19 						*/
        volatile uint32_t swier20 		: 1 ;     /*!< [Bit 20] 		Software Interrupt on line 20 						*/
        volatile uint32_t swier21 		: 1 ;     /*!< [Bit 21] 		Software Interrupt on line 21 						*/
        volatile uint32_t swier22 		: 1 ;     /*!< [Bit 22] 		Software Interrupt on line 22 						*/
        		  uint32_t reserved  	: 9 ;     /*!< [Bits 31:23] Reserved, must be kept at reset value. 				*/
    	   }bit;
}EXTI_SWIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pr0  			: 1 ;     /*!< [Bit 0] 		Pending bit on line 0 								*/
        volatile uint32_t pr1  			: 1 ;     /*!< [Bit 1] 		Pending bit on line 1 								*/
        volatile uint32_t pr2  			: 1 ;     /*!< [Bit 2] 		Pending bit on line 2 								*/
        volatile uint32_t pr3  			: 1 ;     /*!< [Bit 3] 		Pending bit on line 3 								*/
        volatile uint32_t pr4  			: 1 ;     /*!< [Bit 4] 		Pending bit on line 4 								*/
        volatile uint32_t pr5  			: 1 ;     /*!< [Bit 5] 		Pending bit on line 5 								*/
        volatile uint32_t pr6  			: 1 ;     /*!< [Bit 6] 		Pending bit on line 6 								*/
        volatile uint32_t pr7  			: 1 ;     /*!< [Bit 7] 		Pending bit on line 7 								*/
        volatile uint32_t pr8  			: 1 ;     /*!< [Bit 8] 		Pending bit on line 8 								*/
        volatile uint32_t pr9  			: 1 ;     /*!< [Bit 9] 		Pending bit on line 9 								*/
        volatile uint32_t pr10 			: 1 ;     /*!< [Bit 10] 		Pending bit on line 10 								*/
        volatile uint32_t pr11 			: 1 ;     /*!< [Bit 11] 		Pending bit on line 11 								*/
        volatile uint32_t pr12 			: 1 ;     /*!< [Bit 12] 		Pending bit on line 12 								*/
        volatile uint32_t pr13 			: 1 ;     /*!< [Bit 13] 		Pending bit on line 13 								*/
        volatile uint32_t pr14 			: 1 ;     /*!< [Bit 14] 		Pending bit on line 14 								*/
        volatile uint32_t pr15 			: 1 ;     /*!< [Bit 15] 		Pending bit on line 15 								*/
        volatile uint32_t pr16 			: 1 ;     /*!< [Bit 16] 		Pending bit on line 16 								*/
        volatile uint32_t pr17 			: 1 ;     /*!< [Bit 17] 		Pending bit on line 17 								*/
        volatile uint32_t pr18 			: 1 ;     /*!< [Bit 18] 		Pending bit on line 18 								*/
        volatile uint32_t pr19 			: 1 ;     /*!< [Bit 19] 		Pending bit on line 19 								*/
        volatile uint32_t pr20 			: 1 ;     /*!< [Bit 20] 		Pending bit on line 20 								*/
        volatile uint32_t pr21 			: 1 ;     /*!< [Bit 21] 		Pending bit on line 21 								*/
        volatile uint32_t pr22 			: 1 ;     /*!< [Bit 22] 		Pending bit on line 22 								*/
        		  uint32_t reserved 	: 9 ;     /*!< [Bits 31:23] Reserved, must be kept at reset value. 				*/
    }bit;
}EXTI_PR_Reg_t;

typedef struct
{
EXTI_Mask_Reg_t  IMR;    /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
EXTI_Mask_Reg_t  EMR;    /*!< EXTI Event mask register,                Address offset: 0x04 */
EXTI_TSR_Reg_t   RTSR;   /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
EXTI_TSR_Reg_t 	 FTSR;   /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
EXTI_SWIER_Reg_t SWIER;  /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
EXTI_PR_Reg_t 	 PR;     /*!< EXTI Pending register,                   Address offset: 0x14 */
}EXTI_RegDef_t;

/**
  * @brief FLASH Registers
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t latency  		:  3 ; 	/*!< [Bits 2:0] 			Latency 										*/
        		  uint32_t reserved_0	:  5 ;	/*!< [Bits 7:3] 			Reserved, must be kept cleared. 				*/
        volatile uint32_t prften     	:  1 ; 	/*!< [Bit 8] 				Prefetch enable 								*/
        volatile uint32_t icen      	:  1 ; 	/*!< [Bit 9] 				Instruction cache enable 						*/
        volatile uint32_t dcen      	:  1 ; 	/*!< [Bit 10] 				Data cache enable 								*/
        volatile uint32_t icrst     	:  1 ;  /*!< [Bit 11] 				Instruction cache reset 						*/
        volatile uint32_t dcrst     	:  1 ;  /*!< [Bit 12] 				Data cache reset 								*/
        		  uint32_t reserved_1	: 19 ;  /*!< [Bits 31:13] 			Reserved, must be kept cleared. 				*/
    	   }bit;
}FLASH_ACR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t eop      		:  1 ;     /*!< [Bit 0] 			End of operation 								*/
        volatile uint32_t operr    		:  1 ;     /*!< [Bit 1] 			Operation error 								*/
        		  uint32_t reserved_0	:  2 ;     /*!< [Bit 3:2] 			Reserved, must be kept cleared. 				*/
        volatile uint32_t wrperr   		:  1 ;     /*!< [Bit 4] 			Write protection error 							*/
        volatile uint32_t pgaerr   		:  1 ;     /*!< [Bit 5] 			Programming alignment error 					*/
        volatile uint32_t pgperr   		:  1 ;     /*!< [Bit 6] 			Programming parallelism error 					*/
        volatile uint32_t pgserr   		:  1 ;     /*!< [Bit 7] 			Programming sequence error 						*/
        		  uint32_t reserved_1	:  8 ;     /*!< [Bits 15:8] 		Reserved, must be kept cleared. 				*/
        volatile uint32_t bsy      		:  1 ;     /*!< [Bit 16] 			Busy 											*/
        		  uint32_t reserved_2	: 15 ;     /*!< [Bits 31:17] 		Reserved, must be kept cleared. 				*/
    	   }bit;
}FLASH_SR_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t pg        	: 1 ;     /*!< [Bit 0] 				Programming 									*/
        volatile uint32_t ser       	: 1 ;     /*!< [Bit 1] 				Sector Erase 									*/
        volatile uint32_t mer       	: 1 ;     /*!< [Bit 2] 				Mass Erase 										*/
        volatile uint32_t snb          	: 4 ;     /*!< [Bits 6:3] 			Sector number 									*/
        		  uint32_t reserved_0	: 1 ;     /*!< [Bit 7] 				Reserved, must be kept cleared. 				*/
        volatile uint32_t psize      	: 2 ;     /*!< [Bits 9:8] 			Program size 									*/
        		  uint32_t reserved_1	: 6 ;     /*!< [Bits 15:10] 		Reserved, must be kept cleared. 				*/
        volatile uint32_t strt    		: 1 ;     /*!< [Bit 16] 			Start 											*/
        		  uint32_t reserved_2	: 7 ;     /*!< [Bits 23:17] 		Reserved, must be kept cleared. 				*/
        volatile uint32_t eopie     	: 1 ;     /*!< [Bit 24] 			End of operation interrupt enable 				*/
        volatile uint32_t errie     	: 1 ;     /*!< [Bit 25] 			Error interrupt enable 							*/
        		  uint32_t reserved_3	: 5 ;     /*!< [Bits 30:26] 		Reserved, must be kept cleared. 				*/
        volatile uint32_t lock      	: 1 ;     /*!< [Bit 31] 			Lock 											*/
    } bit;
}FLASH_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t optlock   	:  1 ;     /*!< [Bit 0] 			Option lock 									*/
        volatile uint32_t optstrt   	:  1 ;     /*!< [Bit 1] 			Option start 									*/
        volatile uint32_t bor_lev 		:  2 ;     /*!< [Bits 3:2] 			BOR reset Level 								*/
        		  uint32_t reserved_0	:  1 ;     /*!< [Bit 4] 			Reserved, must be kept cleared. 				*/
        volatile uint32_t wdg_sw   		:  1 ;     /*!< [Bit 5] 			WDG_SW 											*/
        volatile uint32_t nrst_stop  	:  1 ;     /*!< [Bit 6] 			nRST_STOP 										*/
        volatile uint32_t nrst_stdby 	:  1 ;     /*!< [Bit 7] 			nRST_STDBY 										*/
        volatile uint32_t rdp     		:  8 ;     /*!< [Bits 15:8] 		Read protect 									*/
        volatile uint32_t nwrp    		: 12 ;     /*!< [Bits 27:16] 		Not write protect 								*/
        		  uint32_t reserved_1	:  4 ;     /*!< [Bits 31:28] 		Reserved, must be kept cleared. 				*/
    	   }bit;
}FLASH_OPTCR_Reg_t;

typedef struct
{
FLASH_ACR_Reg_t 	ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
volatile uint32_t 	KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
volatile uint32_t 	OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
FLASH_SR_Reg_t 		SR;       /*!< FLASH status register,           Address offset: 0x0C */
FLASH_CR_Reg_t 		CR;       /*!< FLASH control register,          Address offset: 0x10 */
FLASH_OPTCR_Reg_t 	OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
}FLASH_RegDef_t;

/**
  * @brief Flexible Static Memory Controller
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t mbken     	:  1 ;     /*!< [Bit 0] 			Memory bank enable bit 							*/
        volatile uint32_t muxen 		:  1 ;     /*!< [Bit 1] 			Address/data multiplexing enable bit 			*/
        volatile uint32_t mtyp  		:  2 ;     /*!< [Bits 3:2] 		Memory type 									*/
        volatile uint32_t mwid  		:  2 ;     /*!< [Bits 5:4] 		Memory databus width 							*/
        volatile uint32_t faccen  		:  1 ;     /*!< [Bit 6] 			Flash access enable 							*/
        		  uint32_t reserved_0	:  1 ;     /*!< [Bit 7] 			Reserved, must be kept at reset value. 			*/
        volatile uint32_t bursten  		:  1 ;     /*!< [Bit 8] 			Burst enable bit 								*/
        volatile uint32_t waitpol    	:  1 ;     /*!< [Bit 9] 			Wait signal polarity bit 						*/
        volatile uint32_t wrapmod   	:  1 ;     /*!< [Bit 10] 		Wrapped burst mode support 						*/
        volatile uint32_t waitcfg   	:  1 ;     /*!< [Bit 11] 		Wait timing configuration 						*/
        volatile uint32_t wren      	:  1 ;     /*!< [Bit 12] 		Write enable bit 								*/
        volatile uint32_t waiten    	:  1 ;     /*!< [Bit 13] 		Wait enable bit 								*/
        volatile uint32_t extmod    	:  1 ;     /*!< [Bit 14] 		Extended mode enable 							*/
        volatile uint32_t asyncwait 	:  1 ;     /*!< [Bit 15] 		Wait signal during asynchronous transfers 		*/
        volatile uint32_t cpsize  		:  3 ;     /*!< [Bits 18:16] 	CRAM page size 									*/
        volatile uint32_t cburstrw  	:  1 ;     /*!< [Bit 19] 		Write burst enable 								*/
        		  uint32_t reserved_1	: 12 ;     /*!< [Bits 31:20] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}FSMC_BCRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t addset     	: 4 ;     /*!< [Bits 3:0] 		Address setup phase duration 					*/
        volatile uint32_t addhld     	: 4 ;     /*!< [Bits 7:4] 		Address-hold phase duration 					*/
        volatile uint32_t datast     	: 8 ;     /*!< [Bits 15:8] 		Data-phase duration 							*/
        volatile uint32_t busturn    	: 4 ;     /*!< [Bits 19:16]		Bus turnaround phase duration 					*/
        volatile uint32_t clkdiv     	: 4 ;     /*!< [Bits 23:20] 		Clock divide ratio 								*/
        volatile uint32_t datlat     	: 4 ;     /*!< [Bits 27:24] 		Data latency for synchronous memory 			*/
        volatile uint32_t accmod     	: 2 ;     /*!< [Bits 29:28] 		Access mode 									*/
        		  uint32_t reserved		: 2 ;     /*!< [Bits 31:30] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}FSMC_BTRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	:  1 ;    /*!< [Bit 0] 			Reserved, must be kept at reset value. 			*/
        volatile uint32_t pwaiten   	:  1 ;    /*!< [Bit 1] 			Wait feature enable bit. 						*/
        volatile uint32_t pbken     	:  1 ;    /*!< [Bit 2] 			PC Card/NAND Flash memory bank enable bit. 		*/
        volatile uint32_t ptyp      	:  1 ;    /*!< [Bit 3] 			Memory type. 									*/
        volatile uint32_t pwid      	:  2 ;    /*!< [Bits 5:4] 		Databus width. 									*/
        volatile uint32_t eccen     	:  1 ;    /*!< [Bit 6] 			ECC computation logic enable bit. 				*/
        		  uint32_t reserved_1	:  2 ;    /*!< [Bits 8:7] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t tclr2     	:  4 ;    /*!< [Bits 12:9] 		CLE to RE delay. 								*/
        volatile uint32_t tar2      	:  4 ;    /*!< [Bits 16:13] 		ALE to RE delay. 								*/
        volatile uint32_t eccps     	:  3 ;    /*!< [Bits 19:17] 		ECC page size. 									*/
        		  uint32_t reserved_2	: 12 ;    /*!< [Bits 31:20] 		Reserved, must be kept at reset value. 			*/
    	   }bit;
}FSMC_PCRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t irs       	:  1 ;     /*!< [Bit 0] 			Interrupt rising edge status. 					*/
        volatile uint32_t ils       	:  1 ;     /*!< [Bit 1] 			Interrupt high-level status. 					*/
        volatile uint32_t ifs    		:  1 ;     /*!< [Bit 2] 			Interrupt falling edge status. 					*/
        volatile uint32_t iren			:  1 ;     /*!< [Bit 3] 			Reserved, must be kept at reset value. 			*/
        volatile uint32_t ilen     		:  1 ;     /*!< [Bit 4] 			Interrupt high-level detection enable bit. 		*/
        volatile uint32_t ifen     		:  1 ;     /*!< [Bit 5] 			Interrupt falling edge detection enable bit. 	*/
        volatile uint32_t fempt    		:  1 ;     /*!< [Bit 6] 			FIFO empty. 									*/
        		  uint32_t reserved		: 24 ;     /*!< [Bits 31:8] 		Reserved, must be kept at reset value. 			*/
    	   }bit;
}FSMC_SRx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t memset   		: 8 ;     /*!< [Bits 7:0] 		Common memory x setup time. 					*/
        volatile uint32_t memwait  		: 8 ;     /*!< [Bits 15:8] 		Common memory x wait time. 						*/
        volatile uint32_t memhold  		: 8 ;     /*!< [Bits 23:16] 		Common memory x hold time. 						*/
        volatile uint32_t memhiz	   	: 8 ;     /*!< [Bits 31:24] 		Common memory x databus HiZ time. 				*/
    	   }bit;
}FSMC_PMEMx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t attset   		: 8 ;     /*!< [Bits 7:0] 		Attribute memory x setup time. 					*/
        volatile uint32_t attwait  		: 8 ;     /*!< [Bits 15:8] 		Attribute memory x wait time. 					*/
        volatile uint32_t atthold  		: 8 ;     /*!< [Bits 23:16] 		Attribute memory x hold time. 					*/
        volatile uint32_t atthiz   		: 8 ;     /*!< [Bits 31:24] 		Attribute memory x databus HiZ time. 			*/
    	   }bit;
}FSMC_PATTx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ioset   		: 8 ;     /*!< [Bits 7:0] 		I/O x setup time. 								*/
        volatile uint32_t iowait  		: 8 ;     /*!< [Bits 15:8] 		I/O x wait time. 								*/
        volatile uint32_t iohold  		: 8 ;     /*!< [Bits 23:16] 		I/O x hold time. 								*/
        volatile uint32_t iohiz   		: 8 ;     /*!< [Bits 31:24] 		I/O x databus HiZ time. 						*/
    	   }bit;
}FSMC_PIO4_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t addset   		: 4 ;     /*!< [Bits 3:0] 		Address setup phase duration. 					*/
        volatile uint32_t addhld   		: 4 ;     /*!< [Bits 7:4] 		Address-hold phase duration. 					*/
        volatile uint32_t datast   		: 8 ;     /*!< [Bits 15:8] 		Data-phase duration. 							*/
        volatile uint32_t busturn  		: 4 ;     /*!< [Bits 19:16] 		Bus turnaround phase duration. 					*/
        		  uint32_t reserved_0	: 8 ;     /*!< [Bits 27:20] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t accmod   		: 3 ;     /*!< [Bits 29:28] 		Access mode. 									*/
        		  uint32_t reserved_1	: 2 ;     /*!< [Bits 31:30] 		Reserved, must be kept at reset value. 			*/
    	   }bit;
}FSMC_BWTRx_Reg_t;

typedef struct
{
FSMC_BCRx_Reg_t    BCR1;	/*!< NOR/PSRAM chip-select control register 1, 	Address offset: 0x00 */
FSMC_BTRx_Reg_t	   BTR1;	/*!< NOR/PSRAM chip-select timing register 1, 	Address offset: 0x04 */
FSMC_BCRx_Reg_t    BCR2;	/*!< NOR/PSRAM chip-select control register 2,  	Address offset: 0x08 */
FSMC_BTRx_Reg_t	   BTR2;	/*!< NOR/PSRAM chip-select timing register 2,  	Address offset: 0x0C */
FSMC_BCRx_Reg_t    BCR3;	/*!< NOR/PSRAM chip-select control register 3,  Address offset: 0x10 */
FSMC_BTRx_Reg_t	   BTR3;	/*!< NOR/PSRAM chip-select timing register 3,  	Address offset: 0x14 */
FSMC_BCRx_Reg_t    BCR4;	/*!< NOR/PSRAM chip-select control register 4,  Address offset: 0x18 */
FSMC_BTRx_Reg_t	   BTR4;	/*!< NOR/PSRAM chip-select timing register 4,  	Address offset: 0x1C */
}FSMC_Bank1_RegDef_t;

/**
  * @brief Flexible Static Memory Controller Bank1E
  */
typedef struct
{
FSMC_BWTRx_Reg_t   BWTR1;	/*!< SRAM/NOR-Flash write timing register 1,  	Address offset: 0x104 */
FSMC_BWTRx_Reg_t   BWTR2;	/*!< SRAM/NOR-Flash write timing register 2,  	Address offset: 0x10C */
FSMC_BWTRx_Reg_t   BWTR3;	/*!< SRAM/NOR-Flash write timing register 3,  	Address offset: 0x114 */
FSMC_BWTRx_Reg_t   BWTR4;	/*!< SRAM/NOR-Flash write timing register 4,  	Address offset: 0x11C */
}FSMC_Bank1E_RegDef_t;

/**
  * @brief Flexible Static Memory Controller Bank2
  */

typedef struct
{
FSMC_PCRx_Reg_t    PCR2;  /*!< PC Card/NAND Flash control register 2,  		Address offset: 0x60 */
FSMC_SRx_Reg_t 	   SR2;	/*!< FIFO status and interrupt register 2,  			Address offset: 0x64 */
FSMC_PMEMx_Reg_t   PMEM2;	/*!< Common memory space timing register 2,  	Address offset: 0x68 */
FSMC_PATTx_Reg_t   PATT2; /*!< Attribute memory space timing register 2,  	Address offset: 0x6C */
uint32_t      	   RESERVED0;  /*!< Reserved, 								Address offset: 0x70 */
volatile uint32_t ECCR2; /*!< ECC result register 2,  						Address offset: 0x74 */
uint32_t      	   RESERVED1;  /*!< Reserved, 								Address offset: 0x78 */
uint32_t      	   RESERVED2;  /*!< Reserved, 								Address offset: 0x7C */
FSMC_PCRx_Reg_t    PCR3;  /*!< PC Card/NAND Flash control register 3,  		Address offset: 0x80 */
FSMC_SRx_Reg_t 	   SR3;	/*!< FIFO status and interrupt register 3,  			Address offset: 0x84 */
FSMC_PMEMx_Reg_t   PMEM3;	/*!< Common memory space timing register 3,  	Address offset: 0x88 */
FSMC_PATTx_Reg_t   PATT3; /*!< Attribute memory space timing register 3,  	Address offset: 0x8C */
uint32_t      	   RESERVED3;  /*!< Reserved, 								Address offset: 0x90 */
volatile uint32_t ECCR3; /*!< ECC result register 3,  						Address offset: 0x94 */
}FSMC_Bank2_3_RegDef_t;

/**
  * @brief Flexible Static Memory Controller Bank4
  */

typedef struct
{
FSMC_PCRx_Reg_t    PCR4;  /*!< PC Card/NAND Flash control register 4,  		Address offset: 0xA0 */
FSMC_SRx_Reg_t 	   SR4;	/*!< FIFO status and interrupt register 4,  			Address offset: 0xA4 */
FSMC_PMEMx_Reg_t   PMEM4;	/*!< Common memory space timing register 4,  	Address offset: 0xA8 */
FSMC_PATTx_Reg_t   PATT4; /*!< Attribute memory space timing register 4,  	Address offset: 0xAC */
FSMC_PIO4_Reg_t    PIO4;  /*!< I/O space timing register 4,  				Address offset: 0xB0 */
}FSMC_Bank4_RegDef_t;

/**
  * @brief General Purpose I/O
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t moder_0   	: 2 ;     /*!< [Bits 1:0] 		Port x Pin 0 configuration bits 				*/
        volatile uint32_t moder_1   	: 2 ;     /*!< [Bits 3:2] 		Port x Pin 1 configuration bits 				*/
        volatile uint32_t moder_2   	: 2 ;     /*!< [Bits 5:4] 		Port x Pin 2 configuration bits 				*/
        volatile uint32_t moder_3   	: 2 ;     /*!< [Bits 7:6] 		Port x Pin 3 configuration bits 				*/
        volatile uint32_t moder_4   	: 2 ;     /*!< [Bits 9:8] 		Port x Pin 4 configuration bits 				*/
        volatile uint32_t moder_5   	: 2 ;     /*!< [Bits 11:10] 		Port x Pin 5 configuration bits 				*/
        volatile uint32_t moder_6   	: 2 ;     /*!< [Bits 13:12] 		Port x Pin 6 configuration bits 				*/
        volatile uint32_t moder_7   	: 2 ;     /*!< [Bits 15:14] 		Port x Pin 7 configuration bits 				*/
        volatile uint32_t moder_8   	: 2 ;     /*!< [Bits 17:16] 		Port x Pin 8 configuration bits 				*/
        volatile uint32_t moder_9   	: 2 ;     /*!< [Bits 19:18] 		Port x Pin 9 configuration bits 				*/
        volatile uint32_t moder_10  	: 2 ;     /*!< [Bits 21:20] 		Port x Pin 10 configuration bits 				*/
        volatile uint32_t moder_11  	: 2 ;     /*!< [Bits 23:22] 		Port x Pin 11 configuration bits 				*/
        volatile uint32_t moder_12  	: 2 ;     /*!< [Bits 25:24] 		Port x Pin 12 configuration bits 				*/
        volatile uint32_t moder_13  	: 2 ;     /*!< [Bits 27:26] 		Port x Pin 13 configuration bits 				*/
        volatile uint32_t moder_14  	: 2 ;     /*!< [Bits 29:28] 		Port x Pin 14 configuration bits 				*/
        volatile uint32_t moder_15  	: 2 ;     /*!< [Bits 31:30] 		Port x Pin 15 configuration bits 				*/
    	   }bit;
}GPIO_MODE_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t ot_0       	:  1 ;     /*!< [Bit 0] 			Port x Pin 0 configuration bit 					*/
        volatile uint32_t ot_1       	:  1 ;     /*!< [Bit 1] 			Port x Pin 1 configuration bit 					*/
        volatile uint32_t ot_2       	:  1 ;     /*!< [Bit 2] 			Port x Pin 2 configuration bit 					*/
        volatile uint32_t ot_3       	:  1 ;     /*!< [Bit 3] 			Port x Pin 3 configuration bit 					*/
        volatile uint32_t ot_4       	:  1 ;     /*!< [Bit 4] 			Port x Pin 4 configuration bit 					*/
        volatile uint32_t ot_5       	:  1 ;     /*!< [Bit 5] 			Port x Pin 5 configuration bit 					*/
        volatile uint32_t ot_6       	:  1 ;     /*!< [Bit 6] 			Port x Pin 6 configuration bit 					*/
        volatile uint32_t ot_7       	:  1 ;     /*!< [Bit 7] 			Port x Pin 7 configuration bit 					*/
        volatile uint32_t ot_8       	:  1 ;     /*!< [Bit 8] 			Port x Pin 8 configuration bit 					*/
        volatile uint32_t ot_9       	:  1 ;     /*!< [Bit 9] 			Port x Pin 9 configuration bit 					*/
        volatile uint32_t ot_10      	:  1 ;     /*!< [Bit 10] 		Port x Pin 10 configuration bit 				*/
        volatile uint32_t ot_11      	:  1 ;     /*!< [Bit 11] 		Port x Pin 11 configuration bit 				*/
        volatile uint32_t ot_12      	:  1 ;     /*!< [Bit 12] 		Port x Pin 12 configuration bit 				*/
        volatile uint32_t ot_13      	:  1 ;     /*!< [Bit 13] 		Port x Pin 13 configuration bit 				*/
        volatile uint32_t ot_14      	:  1 ;     /*!< [Bit 14] 		Port x Pin 14 configuration bit 				*/
        volatile uint32_t ot_15      	:  1 ;     /*!< [Bit 15] 		Port x Pin 15 configuration bit 				*/
        		  uint32_t reserved		: 16 ;     /*!< [Bits 31:16] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}GPIO_OTYPE_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ospeedr_0   	: 2 ;     	/*!< [Bits 1:0] 		Port x Pin 0 configuration bit 					*/
        volatile uint32_t ospeedr_1		: 2 ;     	/*!< [Bits 3:2] 		Port x Pin 1 configuration bit 					*/
        volatile uint32_t ospeedr_2		: 2 ;     	/*!< [Bits 5:4] 		Port x Pin 2 configuration bit 					*/
        volatile uint32_t ospeedr_3		: 2 ;     	/*!< [Bits 7:6] 		Port x Pin 3 configuration bit 					*/
        volatile uint32_t ospeedr_4		: 2 ;     	/*!< [Bits 9:8] 		Port x Pin 4 configuration bit 					*/
        volatile uint32_t ospeedr_5		: 2 ;     	/*!< [Bits 11:10] 	Port x Pin 5 configuration bit 					*/
        volatile uint32_t ospeedr_6		: 2 ;     	/*!< [Bits 13:12] 	Port x Pin 6 configuration bit 					*/
        volatile uint32_t ospeedr_7		: 2 ;     	/*!< [Bits 15:14] 	Port x Pin 7 configuration bit 					*/
        volatile uint32_t ospeedr_8		: 2 ;     	/*!< [Bits 17:16] 	Port x Pin 8 configuration bit 					*/
        volatile uint32_t ospeedr_9		: 2 ;     	/*!< [Bits 19:18] 	Port x Pin 9 configuration bit 					*/
        volatile uint32_t ospeedr_10	: 2 ;     	/*!< [Bits 21:20] 	Port x Pin 10 configuration bit 				*/
        volatile uint32_t ospeedr_11	: 2 ;     	/*!< [Bits 23:22] 	Port x Pin 11 configuration bit 				*/
        volatile uint32_t ospeedr_12	: 2 ;     	/*!< [Bits 25:24] 	Port x Pin 12 configuration bit 				*/
        volatile uint32_t ospeedr_13	: 2 ;     	/*!< [Bits 27:26] 	Port x Pin 13 configuration bit 				*/
        volatile uint32_t ospeedr_14	: 2 ;     	/*!< [Bits 29:28] 	Port x Pin 14 configuration bit 				*/
        volatile uint32_t ospeedr_15	: 2 ;     	/*!< [Bits 31:30] 	Port x Pin 15 configuration bit 				*/
    	   }bit;
}GPIO_OSPEED_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pupdr_0      	: 2 ;     	/*!< [Bits 1:0] 		Port x Pin 0 configuration bit 					*/
        volatile uint32_t pupdr_1      	: 2 ;     	/*!< [Bits 3:2] 		Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_2      	: 2 ;     	/*!< [Bits 5:4] 		Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_3      	: 2 ;		/*!< [Bits 7:6] 		Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_4      	: 2 ;     	/*!< [Bits 9:8] 		Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_5      	: 2 ;     	/*!< [Bits 11:10] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_6      	: 2 ;     	/*!< [Bits 13:12] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_7      	: 2 ;     	/*!< [Bits 15:14] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_8      	: 2 ;     	/*!< [Bits 17:16] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_9      	: 2 ;     	/*!< [Bits 19:18] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_10     	: 2 ;     	/*!< [Bits 21:20] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_11     	: 2 ;     	/*!< [Bits 23:22] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_12     	: 2 ;     	/*!< [Bits 25:24] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_13     	: 2 ;     	/*!< [Bits 27:26] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_14     	: 2 ;     	/*!< [Bits 29:28] 	Port x Pin 1 configuration bit 					*/
        volatile uint32_t pupdr_15     	: 2 ;     	/*!< [Bits 31:30] 	Port x Pin 1 configuration bit 					*/
    	   }bit;
}GPIO_PUPD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t idr_0       	:  1 ;   	/*!< [Bit 0] 		Pin 0 input data 								*/
        volatile uint32_t idr_1        	:  1 ;   	/*!< [Bit 1] 		Pin 1 input data 								*/
        volatile uint32_t idr_2        	:  1 ;     	/*!< [Bit 2] 		Pin 2 input data 								*/
        volatile uint32_t idr_3        	:  1 ;     	/*!< [Bit 3] 		Pin 3 input data 								*/
        volatile uint32_t idr_4        	:  1 ;     	/*!< [Bit 4] 		Pin 4 input data 								*/
        volatile uint32_t idr_5        	:  1 ;     	/*!< [Bit 5] 		Pin 5 input data 								*/
        volatile uint32_t idr_6        	:  1 ;     	/*!< [Bit 6] 		Pin 6 input data 								*/
        volatile uint32_t idr_7        	:  1 ;     	/*!< [Bit 7] 		Pin 7 input data 								*/
        volatile uint32_t idr_8        	:  1 ;     	/*!< [Bit 8] 		Pin 8 input data 								*/
        volatile uint32_t idr_9        	:  1 ;     	/*!< [Bit 9] 		Pin 9 input data 								*/
        volatile uint32_t idr_10       	:  1 ;     	/*!< [Bit 10] 		Pin 10 input data 								*/
        volatile uint32_t idr_11       	:  1 ;     	/*!< [Bit 11] 		Pin 11 input data 								*/
        volatile uint32_t idr_12       	:  1 ;     	/*!< [Bit 12] 		Pin 12 input data 								*/
        volatile uint32_t idr_13       	:  1 ;     	/*!< [Bit 13] 		Pin 13 input data 								*/
        volatile uint32_t idr_14       	:  1 ;     	/*!< [Bit 14] 		Pin 14 input data 								*/
        volatile uint32_t idr_15       	:  1 ;     	/*!< [Bit 15] 		Pin 15 input data 								*/
        uint32_t reserved              	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}GPIO_ID_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t odr_0       	:  1 ;   	/*!< [Bit 0] 		Pin 0 output data 								*/
        volatile uint32_t odr_1        	:  1 ;   	/*!< [Bit 1] 		Pin 1 output data 								*/
        volatile uint32_t odr_2        	:  1 ;     	/*!< [Bit 2] 		Pin 2 output data 								*/
        volatile uint32_t odr_3        	:  1 ;     	/*!< [Bit 3] 		Pin 3 output data 								*/
        volatile uint32_t odr_4        	:  1 ;     	/*!< [Bit 4] 		Pin 4 output data 								*/
        volatile uint32_t odr_5        	:  1 ;     	/*!< [Bit 5] 		Pin 5 output data 								*/
        volatile uint32_t odr_6        	:  1 ;     	/*!< [Bit 6] 		Pin 6 output data 								*/
        volatile uint32_t odr_7        	:  1 ;     	/*!< [Bit 7] 		Pin 7 output data 								*/
        volatile uint32_t odr_8        	:  1 ;     	/*!< [Bit 8] 		Pin 8 output data 								*/
        volatile uint32_t odr_9        	:  1 ;     	/*!< [Bit 9] 		Pin 9 output data 								*/
        volatile uint32_t odr_10       	:  1 ;     	/*!< [Bit 10] 		Pin 10 output data 								*/
        volatile uint32_t odr_11       	:  1 ;     	/*!< [Bit 11] 		Pin 11 output data 								*/
        volatile uint32_t odr_12       	:  1 ;     	/*!< [Bit 12] 		Pin 12 output data 								*/
        volatile uint32_t odr_13       	:  1 ;     	/*!< [Bit 13] 		Pin 13 output data 								*/
        volatile uint32_t odr_14       	:  1 ;     	/*!< [Bit 14] 		Pin 14 output data 								*/
        volatile uint32_t odr_15       	:  1 ;     	/*!< [Bit 15] 		Pin 15 output data 								*/
        uint32_t reserved              	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}GPIO_OD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t bs_0         : 1 ;     	/*!< [Bit 0] 		Port x set bit 0								*/
        volatile uint32_t bs_1         : 1 ;     	/*!< [Bit 1] 		Port x set bit 1								*/
        volatile uint32_t bs_2         : 1 ;     	/*!< [Bit 2] 		Port x set bit 2								*/
        volatile uint32_t bs_3         : 1 ;     	/*!< [Bit 3] 		Port x set bit 3								*/
        volatile uint32_t bs_4         : 1 ;     	/*!< [Bit 4] 		Port x set bit 4								*/
        volatile uint32_t bs_5         : 1 ;     	/*!< [Bit 5] 		Port x set bit 5								*/
        volatile uint32_t bs_6         : 1 ;     	/*!< [Bit 6] 		Port x set bit 6								*/
        volatile uint32_t bs_7         : 1 ;     	/*!< [Bit 7] 		Port x set bit 7								*/
        volatile uint32_t bs_8         : 1 ;     	/*!< [Bit 8] 		Port x set bit 8								*/
        volatile uint32_t bs_9         : 1 ;     	/*!< [Bit 9] 		Port x set bit 9								*/
        volatile uint32_t bs_10        : 1 ;     	/*!< [Bit 10] 		Port x set bit 10								*/
        volatile uint32_t bs_11        : 1 ;     	/*!< [Bit 11] 		Port x set bit 11								*/
        volatile uint32_t bs_12        : 1 ;     	/*!< [Bit 12] 		Port x set bit 12								*/
        volatile uint32_t bs_13        : 1 ;     	/*!< [Bit 13] 		Port x set bit 13								*/
        volatile uint32_t bs_14        : 1 ;     	/*!< [Bit 14] 		Port x set bit 14								*/
        volatile uint32_t bs_15        : 1 ;     	/*!< [Bit 15] 		Port x set bit 15								*/
        volatile uint32_t br_0         : 1 ;     	/*!< [Bit 16] 		Port x reset bit 0								*/
        volatile uint32_t br_1         : 1 ;     	/*!< [Bit 17] 		Port x reset bit 1								*/
        volatile uint32_t br_2         : 1 ;     	/*!< [Bit 18] 		Port x reset bit 2								*/
        volatile uint32_t br_3         : 1 ;     	/*!< [Bit 19] 		Port x reset bit 3								*/
        volatile uint32_t br_4         : 1 ;     	/*!< [Bit 20] 		Port x reset bit 4								*/
        volatile uint32_t br_5         : 1 ;     	/*!< [Bit 21] 		Port x reset bit 5								*/
        volatile uint32_t br_6         : 1 ;     	/*!< [Bit 22] 		Port x reset bit 6								*/
        volatile uint32_t br_7         : 1 ;     	/*!< [Bit 23] 		Port x reset bit 7								*/
        volatile uint32_t br_8         : 1 ;     	/*!< [Bit 24] 		Port x reset bit 8								*/
        volatile uint32_t br_9         : 1 ;     	/*!< [Bit 25] 		Port x reset bit 9								*/
        volatile uint32_t br_10        : 1 ;     	/*!< [Bit 26] 		Port x reset bit 10								*/
        volatile uint32_t br_11        : 1 ;     	/*!< [Bit 27] 		Port x reset bit 11								*/
        volatile uint32_t br_12        : 1 ;     	/*!< [Bit 28] 		Port x reset bit 12								*/
        volatile uint32_t br_13        : 1 ;     	/*!< [Bit 29] 		Port x reset bit 13								*/
        volatile uint32_t br_14        : 1 ;     	/*!< [Bit 30] 		Port x reset bit 14								*/
        volatile uint32_t br_15        : 1 ;     	/*!< [Bit 31] 		Port x reset bit 15								*/
    	   }bit;
}GPIO_BSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t lck_0       	:  1 ;     	/*!< [Bit 0] 		Lock bit 0 										*/
        volatile uint32_t lck_1        	:  1 ;     	/*!< [Bit 1] 		Lock bit 1 										*/
        volatile uint32_t lck_2        	:  1 ;     	/*!< [Bit 2] 		Lock bit 2 										*/
        volatile uint32_t lck_3        	:  1 ;     	/*!< [Bit 3] 		Lock bit 3 										*/
        volatile uint32_t lck_4        	:  1 ;     	/*!< [Bit 4] 		Lock bit 4 										*/
        volatile uint32_t lck_5        	:  1 ;     	/*!< [Bit 5] 		Lock bit 5 										*/
        volatile uint32_t lck_6        	:  1 ;     	/*!< [Bit 6] 		Lock bit 6 										*/
        volatile uint32_t lck_7        	:  1 ;     	/*!< [Bit 7] 		Lock bit 7 										*/
        volatile uint32_t lck_8        	:  1 ;     	/*!< [Bit 8] 		Lock bit 8 										*/
        volatile uint32_t lck_9        	:  1 ;     	/*!< [Bit 9] 		Lock bit 9 										*/
        volatile uint32_t lck_10       	:  1 ;     	/*!< [Bit 10] 		Lock bit 10 									*/
        volatile uint32_t lck_11       	:  1 ;     	/*!< [Bit 11] 		Lock bit 11 									*/
        volatile uint32_t lck_12       	:  1 ;     	/*!< [Bit 12] 		Lock bit 12 									*/
        volatile uint32_t lck_13       	:  1 ;     	/*!< [Bit 13] 		Lock bit 13 									*/
        volatile uint32_t lck_14       	:  1 ;     	/*!< [Bit 14] 		Lock bit 14 									*/
        volatile uint32_t lck_15       	:  1 ;     	/*!< [Bit 15] 		Lock bit 15 									*/
        volatile uint32_t lckk         	:  1 ;     	/*!< [Bit 16] 		Lock key 										*/
        		  uint32_t reserved   	: 15 ;    	/*!< [Bits 31:17] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}GPIO_LCK_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t afrx_0      	: 4 ;     	/*!< [Bits 3:0] 		Alternate function selection for bit 0 			*/
        volatile uint32_t afrx_1      	: 4 ;     	/*!< [Bits 7:4] 		Alternate function selection for bit 1 			*/
        volatile uint32_t afrx_2      	: 4 ;     	/*!< [Bits 11:8] 	Alternate function selection for bit 2 			*/
        volatile uint32_t afrx_3      	: 4 ;     	/*!< [Bits 15:12] 	Alternate function selection for bit 3 			*/
        volatile uint32_t afrx_4      	: 4 ;     	/*!< [Bits 19:16] 	Alternate function selection for bit 4 			*/
        volatile uint32_t afrx_5      	: 4 ;     	/*!< [Bits 23:20] 	Alternate function selection for bit 5 			*/
        volatile uint32_t afrx_6      	: 4 ;     	/*!< [Bits 27:24] 	Alternate function selection for bit 6 			*/
        volatile uint32_t afrx_7      	: 4 ;     	/*!< [Bits 31:28] 	Alternate function selection for bit 7 			*/
    	   }bit;
}GPIO_AFx_Reg_t;

typedef struct
{
GPIO_MODE_Reg_t   MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
GPIO_OTYPE_Reg_t  OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
GPIO_OSPEED_Reg_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
GPIO_PUPD_Reg_t   PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
GPIO_ID_Reg_t 	  IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
GPIO_OD_Reg_t     ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
GPIO_BSR_Reg_t    BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
GPIO_LCK_Reg_t    LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
GPIO_AFx_Reg_t    AFRL;     /*!< GPIO alternate function registers,     Address offset: 0x20 	  */
GPIO_AFx_Reg_t    AFRH;     /*!< GPIO alternate function registers,     Address offset: 0x24 	  */
}GPIO_RegDef_t;

/**
  * @brief System configuration controller
  */

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t mem_mode 		:  2 ;		/*!< [Bits 1:0] 		Memory mapping selection						*/
				  uint32_t reserved		: 30 ;		/*!< [Bits 31:2] 	Reserved, must be kept at reset value. 			*/
	}bit;
}SYSCFG_MEMRMP_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    			  uint32_t reserved_0  	: 23 ;  	/*!< [Bits 22:0] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t mii_rmii_sel 	:  1 ;  	/*!< [Bit 23] 		Ethernet PHY interface selection. 				*/
        		  uint32_t reserved_1 	:  8 ;  	/*!< [Bits 31:24] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_PMC_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t exti_0    	:  4 ;     	/*!< [Bits 3:0] 		EXTI 0 configuration. 							*/
        volatile uint32_t exti_1    	:  4 ;     	/*!< [Bits 7:4] 		EXTI 1 configuration. 							*/
        volatile uint32_t exti_2    	:  4 ;     	/*!< [Bits 11:8] 	EXTI 2 configuration. 							*/
        volatile uint32_t exti_3    	:  4 ;     	/*!< [Bits 15:12] 	EXTI 3 configuration. 							*/
         	 	  uint32_t reserved  	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_EXTICR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t exti_4    	:  4 ;     	/*!< [Bits 3:0] 		EXTI 4 configuration. 							*/
        volatile uint32_t exti_5    	:  4 ;     	/*!< [Bits 7:4] 		EXTI 5 configuration. 							*/
        volatile uint32_t exti_6    	:  4 ;     	/*!< [Bits 11:8] 	EXTI 6 configuration. 							*/
        volatile uint32_t exti_7    	:  4 ;     	/*!< [Bits 15:12] 	EXTI 7 configuration. 							*/
        		  uint32_t reserved  	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_EXTICR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t exti_8    	:  4 ;     	/*!< [Bits 3:0] 		EXTI 8 configuration. 							*/
        volatile uint32_t exti_9    	:  4 ;     	/*!< [Bits 7:4] 		EXTI 9 configuration. 							*/
        volatile uint32_t exti_10   	:  4 ;     	/*!< [Bits 11:8] 	EXTI 10 configuration. 							*/
        volatile uint32_t exti_11   	:  4 ;     	/*!< [Bits 15:12] 	EXTI 11 configuration. 							*/
        		  uint32_t reserved  	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_EXTICR3_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t exti_12   	:  4 ;     	/*!< [Bits 3:0] 		EXTI 12 configuration. 							*/
        volatile uint32_t exti_13   	:  4 ;     	/*!< [Bits 7:4] 		EXTI 13 configuration. 							*/
        volatile uint32_t exti_14   	:  4 ;     	/*!< [Bits 11:8] 	EXTI 14 configuration. 							*/
        volatile uint32_t exti_15   	:  4 ;     	/*!< [Bits 15:12] 	EXTI 15 configuration. 							*/
        		  uint32_t reserved  	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_EXTICR4_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cmp_pd   		:  1 ;    	/*!< [Bit 0] 		Compensation cell power-down. 					*/
        		  uint32_t reserved_0 	:  6 ;   	/*!< [Bits 7:2] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t ready   		:  1 ;      /*!< [Bit 8] 		Compensation cell ready flag. 					*/
        		  uint32_t reserved_1 	: 23 ;  	/*!< [Bits 31:9] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SYSCFG_CMPCR_Reg_t;

typedef struct
{
SYSCFG_MEMRMP_Reg_t  MEMRMP;      	/*!< SYSCFG memory remap register,                       Address offset: 0x00 		*/
SYSCFG_PMC_Reg_t 	 PMC;         	/*!< SYSCFG peripheral mode configuration register,     	 Address offset: 0x04 		*/
SYSCFG_EXTICR1_Reg_t EXTICR1;     	/*!< SYSCFG external interrupt configuration register 1, Address offset: 0x08 		*/
SYSCFG_EXTICR2_Reg_t EXTICR2;     	/*!< SYSCFG external interrupt configuration register 2, Address offset: 0x0C 		*/
SYSCFG_EXTICR3_Reg_t EXTICR3;     	/*!< SYSCFG external interrupt configuration register 3, Address offset: 0x10 		*/
SYSCFG_EXTICR4_Reg_t EXTICR4;     	/*!< SYSCFG external interrupt configuration register 4, Address offset: 0x14 		*/
  uint32_t      	 RESERVED[2];  	/*!< Reserved, 											 Address offset: 0x18-0x1C 	*/
SYSCFG_CMPCR_Reg_t   CMPCR;        	/*!< SYSCFG Compensation cell control register,          Address offset: 0x20      	*/
}SYSCFG_RegDef_t;

/**
  * @brief Inter-integrated Circuit Interface
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pe         	: 1 ;     	/*!< [Bit 0] 		Peripheral enable 								*/
        volatile uint32_t smbus      	: 1 ;     	/*!< [Bit 1] 		SMBus mode 										*/
        		  uint32_t reserved_0 	: 1 ;     	/*!< [Bit 2] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t smbtype    	: 1 ;     	/*!< [Bit 3] 		SMBus type 										*/
        volatile uint32_t enarp      	: 1 ;     	/*!< [Bit 4] 		ARP enable 										*/
        volatile uint32_t enpec      	: 1 ;     	/*!< [Bit 5] 		PEC enable 										*/
        volatile uint32_t engc       	: 1 ;     	/*!< [Bit 6] 		General call enable 							*/
        volatile uint32_t nostretch  	: 1 ;     	/*!< [Bit 7] 		Clock stretching disable (Slave mode) 			*/
        volatile uint32_t start      	: 1 ;     	/*!< [Bit 8] 		Start generation 								*/
        volatile uint32_t stop       	: 1 ;     	/*!< [Bit 9] 		Stop generation 								*/
        volatile uint32_t ack        	: 1 ;     	/*!< [Bit 10] 		Acknowledge enable 								*/
        volatile uint32_t pos        	: 1 ;     	/*!< [Bit 11] 		Acknowledge/PEC Position (for data reception) 	*/
        volatile uint32_t pec        	: 1 ;     	/*!< [Bit 12] 		Packet error checking 							*/
        volatile uint32_t alert      	: 1 ;     	/*!< [Bit 13] 		SMBus alert 									*/
        		  uint32_t reserved_1 	: 1 ;     	/*!< [Bit 14] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t swrst      	: 1 ;     	/*!< [Bit 15] 		Software reset 									*/
    	   }bit;
}I2C_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t freq       	: 6 ;     	/*!< [Bits 5:0] 		Peripheral clock frequency 						*/
        		  uint32_t reserved_0 	: 2 ;     	/*!< [Bits 7:6] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t iterren    	: 1 ;     	/*!< [Bit 8] 		Error interrupt enable 							*/
        volatile uint32_t itevtlen  	: 1 ;     	/*!< [Bit 9] 		Event interrupt enable 							*/
        volatile uint32_t itbufen   	: 1 ;     	/*!< [Bit 10] 		Buffer interrupt enable 						*/
        volatile uint32_t dmaen     	: 1 ;     	/*!< [Bit 11] 		DMA requests enable 							*/
        volatile uint32_t last      	: 1 ;     	/*!< [Bit 12] 		DMA last transfer 								*/
        		  uint32_t reserved_1 	: 3 ;     	/*!< [Bits 15:13] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}I2C_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t add_0      	: 1 ;     	/*!< [Bit 0] 		Interface address 								*/
        volatile uint32_t add_7_1    	: 7 ;     	/*!< [Bits 7:1] 		Interface address 								*/
        volatile uint32_t add_9_8    	: 2 ;     	/*!< [Bits 9:8] 		Interface address 								*/
        		  uint32_t reserved		: 5 ;     	/*!< [Bits 14:10] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t addmode   	: 1 ;     	/*!< [Bit 15] 		Addressing mode (slave mode) 					*/
    	   }bit;
}I2C_OAR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t endual     	: 1 ;     	/*!< [Bit 0] 		Dual addressing mode enable 					*/
        volatile uint32_t add2      	: 7 ;     	/*!< [Bits 7:1] 		Interface address in dual addressing mode 		*/
        uint32_t reserved		: 8 ;     	/*!< [Bits 15:8] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}I2C_OAR2_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t dr         	: 8 ;     	/*!< [Bits 7:0] 		8-bit data register 							*/
        		  uint32_t reserved		: 8 ;    	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 			*/
    } bit;
}I2C_DR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sb         	: 1 ;     	/*!< [Bit 0] 		Start bit (Master mode) 						*/
        volatile uint32_t addr       	: 1 ;     	/*!< [Bit 1] 		Address sent (master mode)/matched (slave mode) */
        volatile uint32_t btf        	: 1 ;     	/*!< [Bit 2] 		Byte transfer finished 							*/
        volatile uint32_t add10      	: 1 ;     	/*!< [Bit 3] 		10-bit header sent (Master mode) 				*/
        volatile uint32_t stopf      	: 1 ;     	/*!< [Bit 4] 		Stop detection (slave mode) 					*/
        volatile uint32_t reserved_0 	: 1 ;     	/*!< [Bit 5] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t rxne        	: 1 ;     	/*!< [Bit 6] 		Data register not empty (receivers) 			*/
        volatile uint32_t txe         	: 1 ;     	/*!< [Bit 7] 		Data register empty (transmitters) 				*/
        volatile uint32_t berr        	: 1 ;     	/*!< [Bit 8] 		Bus error 										*/
        volatile uint32_t arlo        	: 1 ;     	/*!< [Bit 9] 		Arbitration lost (master mode) 					*/
        volatile uint32_t af          	: 1 ;     	/*!< [Bit 10] 		Acknowledge failure 							*/
        volatile uint32_t ovr         	: 1 ;     	/*!< [Bit 11] 		Overrun/Underrun 								*/
        volatile uint32_t pecerr      	: 1 ;     	/*!< [Bit 12] 		PEC Error in reception 							*/
        volatile uint32_t reserved_1  	: 1 ;     	/*!< [Bit 13] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t timeout     	: 1 ;     	/*!< [Bit 14] 		Timeout or Tlow error 							*/
        volatile uint32_t smbalert    	: 1 ;     	/*!< [Bit 15] 		SMBus alert 									*/
    	   }bit;
}I2C_SR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t msl        	: 1 ;     	/*!< [Bit 0] 		Master/slave 									*/
        volatile uint32_t busy       	: 1 ;     	/*!< [Bit 1] 		Bus busy 										*/
        volatile uint32_t tra        	: 1 ;     	/*!< [Bit 2] 		Transmitter/receiver 							*/
        		  uint32_t reserved	 	: 1 ;     	/*!< [Bit 3] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t gencall    	: 1 ;     	/*!< [Bit 4] 		General call address (Slave mode) 				*/
        volatile uint32_t smbdefault 	: 1 ;     	/*!< [Bit 5] 		SMBus device default address (Slave mode) 		*/
        volatile uint32_t smbhost    	: 1 ;     	/*!< [Bit 6] 		SMBus host header (Slave mode) 					*/
        volatile uint32_t dualf      	: 1 ;     	/*!< [Bit 7] 		Dual flag (Slave mode) 							*/
        volatile uint32_t pec        	: 8 ;     	/*!< [Bits 15:8] 	Packet error checking register 					*/
    	   }bit;
}I2C_SR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ccr        	: 12 ;    	/*!< [Bits 11:0] 	Clock control register in Fm/Sm mode (Master mode) */
        		  uint32_t reserved_0 	:  2 ;     	/*!< [Bits 13:12] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t duty       	:  1 ;     	/*!< [Bit 14] 		Fm mode duty cycle 								*/
        volatile uint32_t fs         	:  1 ;     	/*!< [Bit 15] 		I2C master mode selection 						*/
    	   }bit;
}I2C_CCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t trise      	:  6 ;     	/*!< [Bits 5:0] 		Maximum rise time in Fm/Sm mode (Master mode) 	*/
        		  uint32_t reserved   	: 10 ;    	/*!< [Bits 15:6] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}I2C_TRISE_Reg_t;

typedef struct
{
I2C_CR1_Reg_t 	CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
I2C_CR2_Reg_t 	CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
I2C_OAR1_Reg_t 	OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
I2C_OAR2_Reg_t 	OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
I2C_DR_Reg_t 	DR;         /*!< I2C Data register,          Address offset: 0x10 */
I2C_SR1_Reg_t 	SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
I2C_SR2_Reg_t 	SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
I2C_CCR_Reg_t 	CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
I2C_TRISE_Reg_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
}I2C_RegDef_t;

/**
  * @brief Independent WATCHDOG
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t key        	: 16 ;    	/*!< [Bits 15:0] 	Key value (write only, read 0000h) 				*/
        		 uint32_t reserved   	: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}IWDG_KR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pr         	:  3 ;     	/*!< [Bits 2:0] 		Prescaler divider 								*/
        		  uint32_t reserved   	: 29 ;    	/*!< [Bits 31:3] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}IWDG_PR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t rl         	: 12 ;    	/*!< [Bits 11:0] 	Watchdog counter reload value 					*/
         	 	  uint32_t reserved   	: 20 ;    	/*!< [Bits 31:12] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}IWDG_RLR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pvu        	:  1 ;     	/*!< [Bit 0] 		Watchdog prescaler value update 				*/
        volatile uint32_t rvu        	:  1 ;     	/*!< [Bit 1] 		Watchdog counter reload value update 			*/
         	 	  uint32_t reserved   	: 30 ;   	/*!< [Bits 31:2] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}IWDG_SR_Reg_t;

typedef struct
{
IWDG_KR_Reg_t 	KR;   /*!< IWDG Key register,       Address offset: 0x00 */
IWDG_PR_Reg_t 	PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
IWDG_RLR_Reg_t 	RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
IWDG_SR_Reg_t 	SR;   /*!< IWDG Status register,    Address offset: 0x0C */
}IWDG_RegDef_t;

/**
  * @brief Power Control
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t lpds       	:  1 ;     	/*!< [Bit 0] 		Low-power deep sleep 							*/
        volatile uint32_t pdds       	:  1 ;     	/*!< [Bit 1] 		Power-down deep sleep 							*/
        volatile uint32_t cwuf       	:  1 ;     	/*!< [Bit 2] 		Clear wakeup flag 								*/
        volatile uint32_t csbf       	:  1 ;     	/*!< [Bit 3] 		Clear standby flag 								*/
        volatile uint32_t pvde       	:  1 ;     	/*!< [Bit 4] 		Power voltage detector enable 					*/
        volatile uint32_t pls 		  	:  3 ;     	/*!< [Bits 7:5] 		PVD level selection 							*/
        volatile uint32_t dbp 		  	:  1 ;     	/*!< [Bit 8] 		Disable backup domain write protection 			*/
        volatile uint32_t fpds       	:  1 ;     	/*!< [Bit 9] 		Flash power-down in Stop mode 					*/
         	 	  uint32_t reserved_0 	:  4 ;     	/*!< [Bits 13:10] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t vos        	:  1 ;     	/*!< [Bit 14] 		Regulator voltage scaling output selection 		*/
        		  uint32_t reserved_1 	: 17 ;    	/*!< [Bits 31:15] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}PWR_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t wuf        	:  1 ;     	/*!< [Bit 0] 		Wakeup flag 									*/
        volatile uint32_t sbf        	:  1 ;     	/*!< [Bit 1] 		Standby flag 									*/
        volatile uint32_t pvdo       	:  1 ;     	/*!< [Bit 2] 		PVD output 										*/
        volatile uint32_t brr        	:  1 ;     	/*!< [Bit 3] 		Backup regulator ready 							*/
        		  uint32_t reserved_0 	:  4 ;     	/*!< [Bits 7:4] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ewup       	:  1 ;     	/*!< [Bit 8] 		Enable WKUP pin 								*/
        volatile uint32_t bre        	:  1 ;     	/*!< [Bit 9] 		Backup regulator enable 						*/
        		  uint32_t reserved_1 	:  4 ;     	/*!< [Bits 13:10] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t vosrdy     	:  1 ;     	/*!< [Bit 14] 		Regulator voltage scaling output selection ready bit */
        		 uint32_t reserved_2 	: 17 ;    	/*!< [Bits 31:15] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}PWR_CSR_Reg_t;

typedef struct
{
PWR_CR_Reg_t  CR;   	/*!< PWR power control register,        Address offset: 0x00 */
PWR_CSR_Reg_t CSR;  	/*!< PWR power control/status register, Address offset: 0x04 */
}PWR_RegDef_t;

/**
  * @brief Reset and Clock Control
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t hsion       	: 1 ;     	/*!< [Bit 0] 		Internal high-speed clock enable 				*/
        volatile uint32_t hsirdy       	: 1 ;     	/*!< [Bit 1] 		Internal high-speed clock ready flag 			*/
        		  uint32_t reserved_0   : 1 ;     	/*!< [Bit 2] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t hsitrim      	: 5 ;     	/*!< [Bits 7:3] 		Internal high-speed clock trimming 				*/
        volatile uint32_t hsical       	: 8 ;     	/*!< [Bits 15:8] 	Internal high-speed clock calibration 			*/
        volatile uint32_t hseon        	: 1 ;     	/*!< [Bit 16] 		HSE clock enable 								*/
        volatile uint32_t hserdy       	: 1 ;     	/*!< [Bit 17] 		HSE clock ready flag 							*/
        volatile uint32_t hsebyp       	: 1 ;     	/*!< [Bit 18] 		HSE clock bypass 								*/
        volatile uint32_t csson        	: 1 ;     	/*!< [Bit 19] 		Clock security system enable 					*/
        		  uint32_t reserved_1   : 4 ;     	/*!< [Bits 23:20] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t pllon        	: 1 ;     	/*!< [Bit 24] 		Main PLL (PLL) enable 							*/
        volatile uint32_t pllrdy       	: 1 ;     	/*!< [Bit 25] 		Main PLL (PLL) clock ready flag 				*/
        volatile uint32_t plli2son     	: 1 ;     	/*!< [Bit 26] 		PLLI2S enable 									*/
        volatile uint32_t plli2srdy    	: 1 ;     	/*!< [Bit 27] 		PLLI2S clock ready flag 						*/
        volatile uint32_t pllsairdy    	: 1 ;     	/*!< [Bit 28] 		PLLSAI clock ready flag 						*/
        volatile uint32_t pllsai       	: 1 ;     	/*!< [Bit 29] 		PLLSAI enable 									*/
        		  uint32_t reserved_2   : 2 ;     	/*!< [Bits 31:30] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pllm        	: 6 ;     	/*!< [Bits 5:0] 		Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock 				*/
        volatile uint32_t plln        	: 9 ;     	/*!< [Bits 14:6] 	Main PLL (PLL) multiplication factor for VCO 											*/
        volatile uint32_t reserved_0   	: 1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 													*/
        volatile uint32_t pllp         	: 2 ;     	/*!< [Bits 17:16] 	Main PLL (PLL) division factor for main system clock 									*/
        volatile uint32_t reserved_1   	: 4 ;     	/*!< [Bits 21:18] 	Reserved, must be kept at reset value 													*/
        volatile uint32_t pllsrc       	: 1 ;     	/*!< [Bit 22] 		Main PLL(PLL) and audio PLL (PLLI2S) entry clock source 								*/
        volatile uint32_t reserved_2   	: 1 ;     	/*!< [Bit 23] 		Reserved, must be kept at reset value 													*/
        volatile uint32_t pllq         	: 4 ;      	/*!< [Bits 27:24] 	Main PLL (PLL) division factor for USB OTG FS, SDIO, and random number generator clocks */
        volatile uint32_t reserved_3   	: 4 ;     	/*!< [Bits 31:28] 	Reserved, must be kept at reset value 													*/
    	   }bit;
}RCC_PLLCFGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sw          	: 2 ;     	/*!< [Bits 1:0] 		System clock switch 							*/
        volatile uint32_t sws          	: 2 ;     	/*!< [Bits 3:2] 		System clock switch status 						*/
        volatile uint32_t hpre         	: 4 ;     	/*!< [Bits 7:4] 		AHB prescaler 									*/
        volatile uint32_t reserved_1   	: 2 ;     	/*!< [Bits 9:8] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ppre1        	: 3 ;     	/*!< [Bits 12:10] 	APB Low-speed prescaler (APB1) 					*/
        volatile uint32_t ppre2        	: 3 ;     	/*!< [Bits 15:13] 	APB high-speed prescaler (APB2) 				*/
        volatile uint32_t rtcpres      	: 5 ;     	/*!< [Bits 20:16] 	HSE division factor for RTC clock 				*/
        volatile uint32_t mco1         	: 2 ;     	/*!< [Bits 22:21] 	Microcontroller clock output 1 					*/
        volatile uint32_t i2ssrc       	: 1 ;     	/*!< [Bit 23] 		I2S clock selection 							*/
        volatile uint32_t mco1pre      	: 3 ;     	/*!< [Bits 26:24] 	MCO1 prescaler 									*/
        volatile uint32_t mco2pre      	: 3 ;     	/*!< [Bits 29:27] 	MCO2 prescaler 									*/
        volatile uint32_t mco2         	: 2 ;     	/*!< [Bits 31:30] 	Microcontroller clock output 2 					*/
    	   }bit;
}RCC_CFGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t lsirdyf      	: 1	;     	/*!< [Bit 0] 		LSI ready interrupt flag 						*/
        volatile uint32_t lserdyf      	: 1	;     	/*!< [Bit 1] 		LSE ready interrupt flag 						*/
        volatile uint32_t hsirdyf      	: 1	;     	/*!< [Bit 2] 		HSI ready interrupt flag 						*/
        volatile uint32_t hserdyf      	: 1	;     	/*!< [Bit 3] 		HSE ready interrupt flag 						*/
        volatile uint32_t pllrdyf      	: 1	;     	/*!< [Bit 4] 		Main PLL (PLL) ready interrupt flag 			*/
        volatile uint32_t plli2srdyf   	: 1	;     	/*!< [Bit 5] 		PLLI2S ready interrupt flag 					*/
        volatile uint32_t pllsairdyf   	: 1	;     	/*!< [Bit 6] 		PLLSAI ready interrupt flag 					*/
        volatile uint32_t cssf         	: 1	;     	/*!< [Bit 7] 		Clock security system interrupt flag 			*/
        volatile uint32_t lsirdyie     	: 1	;     	/*!< [Bit 8] 		LSI ready interrupt enable 						*/
        volatile uint32_t lserdyie     	: 1	;     	/*!< [Bit 9] 		LSE ready interrupt enable 						*/
        volatile uint32_t hsirdyie     	: 1	;     	/*!< [Bit 10] 		HSI ready interrupt enable 						*/
        volatile uint32_t hserdyie     	: 1	;     	/*!< [Bit 11] 		HSE ready interrupt enable 						*/
        volatile uint32_t pllrdyie     	: 1	;     	/*!< [Bit 12] 		Main PLL (PLL) ready interrupt enable 			*/
        volatile uint32_t plli2srdyie  	: 1	;     	/*!< [Bit 13] 		PLLI2S ready interrupt enable 					*/
        volatile uint32_t pllsairdyie  	: 1	;     	/*!< [Bit 14] 		PLLSAI ready interrupt enable 					*/
        volatile uint32_t reserved_0   	: 1	;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t lsirdyc      	: 1	;     	/*!< [Bit 16] 		LSI ready interrupt clear 						*/
        volatile uint32_t lserdyc      	: 1	;     	/*!< [Bit 17] 		LSE ready interrupt clear 						*/
        volatile uint32_t hsirdyc      	: 1	;     	/*!< [Bit 18] 		HSI ready interrupt clear 						*/
        volatile uint32_t hserdyc      	: 1	;     	/*!< [Bit 19] 		HSE ready interrupt clear 						*/
        volatile uint32_t pllrdyc      	: 1	;     	/*!< [Bit 20] 		Main PLL (PLL) ready interrupt clear 			*/
        volatile uint32_t plli2srdyc   	: 1	;     	/*!< [Bit 21] 		PLLI2S ready interrupt clear 					*/
        volatile uint32_t pllsairdyc   	: 1	;     	/*!< [Bit 22] 		PLLSAI ready interrupt clear 					*/
        volatile uint32_t cssc         	: 1	;     	/*!< [Bit 23] 		Clock security system interrupt clear 			*/
         	 	  uint32_t reserved_1   : 8	;     	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    }bit;
}RCC_CIR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t gpioarst     	: 1 ;     	/*!< [Bit 0] 		IO port A reset 								*/
        volatile uint32_t gpiobrst     	: 1 ;     	/*!< [Bit 1] 		IO port B reset 								*/
        volatile uint32_t gpiocrst     	: 1 ;     	/*!< [Bit 2] 		IO port C reset 								*/
        volatile uint32_t gpiodrst     	: 1 ;     	/*!< [Bit 3] 		IO port D reset 								*/
        volatile uint32_t gpioerst     	: 1 ;     	/*!< [Bit 4] 		IO port E reset 								*/
        volatile uint32_t gpiofrst     	: 1 ;     	/*!< [Bit 5] 		IO port F reset 								*/
        volatile uint32_t gpiogrst     	: 1 ;     	/*!< [Bit 6] 		IO port G reset 								*/
        volatile uint32_t gpiohrst     	: 1 ;     	/*!< [Bit 7] 		IO port H reset 								*/
        volatile uint32_t gpioirst     	: 1 ;     	/*!< [Bit 8] 		IO port I reset 								*/
        volatile uint32_t gpiojrst     	: 1 ;     	/*!< [Bit 9] 		IO port J reset 								*/
        volatile uint32_t gpiokrst     	: 1 ;     	/*!< [Bit 10] 		IO port K reset 								*/
        		  uint32_t reserved_0   : 1 ;     	/*!< [Bit 11] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t crcrst       	: 1 ;     	/*!< [Bit 12] 		CRC reset 										*/
        		  uint32_t reserved_1   : 8 ;     	/*!< [Bits 20:13] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t dma1rst      	: 1 ;     	/*!< [Bit 21] 		DMA1 reset 										*/
        volatile uint32_t dma2rst      	: 1 ;     	/*!< [Bit 22] 		DMA2 reset 										*/
        volatile uint32_t dma2drst     	: 1 ;     	/*!< [Bit 23] 		DMA2D reset 									*/
        		 uint32_t reserved_2   	: 1 ;     	/*!< [Bit 24] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ethmacrst  	: 1 ;     	/*!< [Bit 25] 		Ethernet MAC reset 								*/
        		  uint32_t reserved_3   : 3 ;     	/*!< [Bits 28:26] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t otghsrst     	: 1 ;     	/*!< [Bit 29] USB 	OTG HS module reset 							*/
        		  uint32_t reserved_4   : 2 ;     	/*!< [Bits 31:30] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB1RSTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dcmirst      	:  1 ;     	/*!< [Bit 0] 		Camera interface reset 							*/
        		  uint32_t reserved_0   :  3 ;     	/*!< [Bits 3:1] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cryptorst    	:  1 ;     	/*!< [Bit 4] 		Cryptographic module reset 						*/
        volatile uint32_t hashrst      	:  1 ;     	/*!< [Bit 5] 		Hash module reset 								*/
        volatile uint32_t rngrst      	:  1 ;     	/*!< [Bit 6] 		Random number generator module reset 			*/
        volatile uint32_t otgfsrst     	:  1 ;     	/*!< [Bit 7] 		USB OTG FS module reset 						*/
        		 uint32_t reserved_1   	: 24 ;    	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB2RSTR_Reg_t;

typedef union{
	uint32_t reg;
	struct{
		volatile uint32_t fmcrst		:  1 ;		/*!< [Bit 0] 		Flexible memory controller module reset 		*/
				 uint32_t reserved		: 31 ;		/*!< [Bits 31:1] 	Reserved, must be kept at reset value 			*/
		   }bit;
}RCC_AHB3RSTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim2rst      	: 1 ;     	/*!< [Bit 0] 		TIM2 reset 										*/
        volatile uint32_t tim3rst      	: 1 ;     	/*!< [Bit 1] 		TIM3 reset 										*/
        volatile uint32_t tim4rst      	: 1 ;     	/*!< [Bit 2] 		TIM4 reset 										*/
        volatile uint32_t tim5rst      	: 1 ;     	/*!< [Bit 3] 		TIM5 reset 										*/
        volatile uint32_t tim6rst      	: 1 ;     	/*!< [Bit 4] 		TIM6 reset 										*/
        volatile uint32_t tim7rst      	: 1 ;     	/*!< [Bit 5] 		TIM7 reset 										*/
        volatile uint32_t tim12rst     	: 1 ;     	/*!< [Bit 6] 		TIM12 reset 									*/
        volatile uint32_t tim13rst     	: 1 ;     	/*!< [Bit 7] 		TIM13 reset 									*/
        volatile uint32_t tim14rst     	: 1 ;     	/*!< [Bit 8] 		TIM14 reset 									*/
        volatile uint32_t reserved_0   	: 2 ;     	/*!< [Bits 10:9] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t wwdgrst     	: 1 ;     	/*!< [Bit 11] 		Window watchdog reset 							*/
        volatile uint32_t reserved_1   	: 2 ;     	/*!< [Bits 13:12] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t spi2rst     	: 1 ;     	/*!< [Bit 14] 		SPI2 reset 										*/
        volatile uint32_t spi3rst     	: 1 ;     	/*!< [Bit 15] 		SPI3 reset 										*/
        volatile uint32_t reserved_2   	: 1 ;     	/*!< [Bit 16] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t usart2rst   	: 1 ;     	/*!< [Bit 17] 		USART2 reset 									*/
        volatile uint32_t usart3rst   	: 1 ;     	/*!< [Bit 18] 		USART3 reset 									*/
        volatile uint32_t uart4rst   	: 1 ;     	/*!< [Bit 19] 		USART4 reset 									*/
        volatile uint32_t uart5rst    	: 1 ;     	/*!< [Bit 20] 		UART5 reset 									*/
        volatile uint32_t i2c1rst     	: 1 ;     	/*!< [Bit 21] 		I2C1 reset 										*/
        volatile uint32_t i2c2rst     	: 1 ;     	/*!< [Bit 22] 		I2C2 reset 										*/
        volatile uint32_t i2c3rst     	: 1 ;     	/*!< [Bit 23] 		I2C3 reset 										*/
        volatile uint32_t reserved_3   	: 1 ;     	/*!< [Bit 24]	 	Reserved, must be kept at reset value 			*/
        volatile uint32_t can1rst     	: 1 ;     	/*!< [Bit 25] 		CAN1 reset 										*/
        volatile uint32_t can2rst     	: 1 ;     	/*!< [Bit 26] 		CAN2 reset 										*/
        volatile uint32_t reserved_4   	: 1 ;     	/*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t pwrrst     	: 1 ;     	/*!< [Bit 28] 		Power interface reset 							*/
        volatile uint32_t dacrst     	: 1 ;     	/*!< [Bit 29] 		DAC reset 										*/
        volatile uint32_t uart7rst    	: 1 ;     	/*!< [Bit 30] 		UART7 reset 									*/
        volatile uint32_t uart8rst    	: 1 ;     	/*!< [Bit 31] 		UART8 reset 									*/
    	   }bit;
}RCC_APB1RSTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim1rst     	: 1 ;     	/*!< [Bit 0] 		TIM1 reset 										*/
        volatile uint32_t tim8rst     	: 1 ;     	/*!< [Bit 1] 		TIM8 reset 										*/
        		  uint32_t reserved_0  	: 2 ;     	/*!< [Bits 3:2] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t usart1rst  	: 1 ;     	/*!< [Bit 4] 		USART1 reset 									*/
        volatile uint32_t usart6rst  	: 1 ;     	/*!< [Bit 5] 		USART6 reset 									*/
        		  uint32_t reserved_1  	: 2 ;     	/*!< [Bits 7:6] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t adcrst     	: 1 ;     	/*!< [Bit 8] 		ADC interface reset (common to all ADCs) 		*/
        		  uint32_t reserved_2  	: 2 ;     	/*!< [Bits 10:9] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t sdiorst    	: 1 ;     	/*!< [Bit 11] 		SDIO reset 										*/
        volatile uint32_t spi1rst    	: 1 ;     	/*!< [Bit 12] 		SPI1 reset 										*/
        volatile uint32_t spi4rst    	: 1 ;     	/*!< [Bit 13] 		SPI4 reset 										*/
        volatile uint32_t syscfgrst  	: 1 ;     	/*!< [Bit 14] 		System configuration controller reset 			*/
        		  uint32_t reserved_3  	: 1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tim9rst    	: 1 ;     	/*!< [Bit 16] 		TIM9 reset 										*/
        volatile uint32_t tim10rst   	: 1 ;     	/*!< [Bit 17] 		TIM10 reset 									*/
        volatile uint32_t tim11rst   	: 1 ;     	/*!< [Bit 18] 		TIM11 reset 									*/
        		  uint32_t reserved_4  	: 1 ;     	/*!< [Bit 19] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t spi5rst    	: 1 ;     	/*!< [Bit 20] 		SPI5 reset 										*/
        volatile uint32_t spi6rst    	: 1 ;     	/*!< [Bit 21]	 	SPI6 reset 										*/
        volatile uint32_t sai1rst    	: 1 ;     	/*!< [Bit 22] 		SAI1 reset 										*/
        		  uint32_t reserved_5  	: 5 ;     	/*!< [Bits 25:23] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t ltdcrst    	: 1 ;     	/*!< [Bit 26] 		LTDC reset 										*/
        		  uint32_t reserved_6  	: 5 ;     	/*!< [Bits 31:27] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_APB2RSTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t gpioaen    	: 1 ;     	/*!< [Bit 0] 		IO port A clock enable 							*/
        volatile uint32_t gpioben    	: 1 ;      	/*!< [Bit 1] 		IO port B clock enable 							*/
        volatile uint32_t gpiocen    	: 1 ;      	/*!< [Bit 2] 		IO port C clock enable 							*/
        volatile uint32_t gpioden    	: 1 ;      	/*!< [Bit 3] 		IO port D clock enable 							*/
        volatile uint32_t gpioeen    	: 1 ;      	/*!< [	 4] 		IO port E clock enable 							*/
        volatile uint32_t gpiofen    	: 1 ;      	/*!< [Bit 5] 		IO port F clock enable 							*/
        volatile uint32_t gpiogen    	: 1 ;      	/*!< [Bit 6] 		IO port G clock enable 							*/
        volatile uint32_t gpiohen    	: 1 ;      	/*!< [Bit 7] 		IO port H clock enable 							*/
        volatile uint32_t gpioien    	: 1 ;      	/*!< [Bit 8] 		IO port I clock enable 							*/
        volatile uint32_t gpiojen    	: 1 ;      	/*!< [Bit 9] 		IO port J clock enable 							*/
        volatile uint32_t gpioken   	: 1 ;      	/*!< [Bit 10] 		IO port K clock enable 							*/
        		  uint32_t reserved_0 	: 1 ;      	/*!< [Bit 11] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t crcen      	: 1 ;      	/*!< [Bit 12] 		CRC clock enable 								*/
        		  uint32_t reserved_1 	: 5 ;      	/*!< [Bits 17:13] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t bkpsramen  	: 1 ;      	/*!< [Bit 18] 		Backup SRAM interface clock enable 				*/
        		  uint32_t reserved_2 	: 1 ;      	/*!< [Bit 19] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ccmdataramen	: 1 ;    	/*!< [Bit 20] 		CCM data RAM clock enable 						*/
        volatile uint32_t dma1en     	: 1 ;     	/*!< [Bit 21] 		DMA1 clock enable 								*/
        volatile uint32_t dma2en     	: 1 ;     	/*!< [Bit 22] 		DMA2 clock enable 								*/
        volatile uint32_t dma2den    	: 1 ;     	/*!< [Bit 23] 		DMA2D clock enable 								*/
        		  uint32_t reserved_3 	: 1 ;     	/*!< [Bit 24] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ethmacen   	: 1 ;     	/*!< [Bit 25] 		Ethernet MAC clock enable 						*/
        volatile uint32_t ethmactxen 	: 1 ;    	/*!< [Bit 26] 		Ethernet Transmission clock enable 				*/
        volatile uint32_t ethmacrxen 	: 1 ;    	/*!< [Bit 27] 		Ethernet Reception clock enable 				*/
        volatile uint32_t ethmacptpen 	: 1 ;   	/*!< [Bit 28] 		Ethernet PTP clock enable 						*/
        volatile uint32_t otghsen    	: 1 ;     	/*!< [Bit 29] 		USB OTG HS clock enable 						*/
        volatile uint32_t otghsulpien 	: 1 ;   	/*!< [Bit 30] 		USB OTG HSULPI clock enable 					*/
        		  uint32_t reserved_4 	: 1 ;     	/*!< [Bit 31] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB1ENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dcmien     	:  1 ;     	/*!< [Bit 0] 		Camera interface enable 						*/
        		  uint32_t reserved_0 	:  3 ;     	/*!< [Bits 1:3] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t crypen     	:  1 ;     	/*!< [Bit 4] 		Cryptographic modules clock enable 				*/
        volatile uint32_t hashen     	:  1 ;     	/*!< [Bit 5] 		Hash modules clock enable 						*/
        volatile uint32_t rngen      	:  1 ;     	/*!< [Bit 6] 		Random number generator clock enable 			*/
        volatile uint32_t otgfsen   	:  1 ;     	/*!< [Bit 7] 		USB OTG FS clock enable 						*/
         	 	  uint32_t reserved_1 	: 24 ;    	/*!< [Bits 8:31] 	Reserved, must be kept at reset value 			*/
    }bit;
}RCC_AHB2ENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fmcen      	:  1 ;     	/*!< [Bit 0] 		Flexible memory controller module clock enable 	*/
        		  uint32_t reserved   	: 31 ;    	/*!< [Bits 1:31] 	Reserved, must be kept at reset value 			*/
    }bit;
}RCC_AHB3ENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim2en     	: 1 ;     	/*!< [Bit 0] 		TIM2 clock enable 								*/
        volatile uint32_t tim3en     	: 1 ;     	/*!< [Bit 1] 		TIM3 clock enable 								*/
        volatile uint32_t tim4en     	: 1 ;     	/*!< [Bit 2] 		TIM4 clock enable 								*/
        volatile uint32_t tim5en     	: 1 ;     	/*!< [Bit 3] 		TIM5 clock enable 								*/
        volatile uint32_t tim6en     	: 1 ;     	/*!< [Bit 4] 		TIM6 clock enable 								*/
        volatile uint32_t tim7en     	: 1 ;     	/*!< [Bit 5] 		TIM7 clock enable 								*/
        volatile uint32_t tim12en    	: 1 ;     	/*!< [Bit 6] 		TIM12 clock enable 								*/
        volatile uint32_t tim13en    	: 1 ;     	/*!< [Bit 7] 		TIM13 clock enable 								*/
        volatile uint32_t tim14en    	: 1 ;     	/*!< [Bit 8] 		TIM14 clock enable 								*/
        		  uint32_t reserved_0  	: 2 ;     	/*!< [Bits 9:10] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t wwdgen     	: 1 ;     	/*!< [Bit 11] 		Window watchdog clock enable 					*/
        		  uint32_t reserved_1  	: 2 ;     	/*!< [Bits 13:12] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t spi2en     	: 1 ;     	/*!< [Bit 14] 		SPI2 clock enable 								*/
        volatile uint32_t spi3en     	: 1 ;     	/*!< [Bit 15] 		SPI3 clock enable 								*/
         	 	  uint32_t reserved_2  	: 1 ;     	/*!< [Bit 16] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t usart2en   	: 1 ;     	/*!< [Bit 17] 		USART2 clock enable 							*/
        volatile uint32_t usart3en   	: 1 ;     	/*!< [Bit 18] 		USART3 clock enable 							*/
        volatile uint32_t uart4en    	: 1 ;     	/*!< [Bit 19] 		UART4 clock enable 								*/
        volatile uint32_t uart5en    	: 1 ;     	/*!< [Bit 20] 		UART5 clock enable 								*/
        volatile uint32_t i2c1en     	: 1 ;     	/*!< [Bit 21] 		I2C1 clock enable 								*/
        volatile uint32_t i2c2en     	: 1 ;     	/*!< [Bit 22] 		I2C2 clock enable 								*/
        volatile uint32_t i2c3en     	: 1 ;     	/*!< [Bit 23] 		I2C3 clock enable 								*/
         	 	  uint32_t reserved_3  	: 1 ;     	/*!< [Bit 24] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t can1en     	: 1 ;     	/*!< [Bit 25] 		CAN1 clock enable 								*/
        volatile uint32_t can2en     	: 1 ;     	/*!< [Bit 26] 		CAN2 clock enable 								*/
        		  uint32_t reserved_4  	: 1 ;     	/*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t pwren      	: 1 ;     	/*!< [Bit 28] 		Power interface clock enable 					*/
        volatile uint32_t dacen      	: 1 ;     	/*!< [Bit 29] 		DAC interface clock enable 						*/
        volatile uint32_t uart7en    	: 1 ;     	/*!< [Bit 30] 		UART7 clock enable 								*/
        volatile uint32_t uart8en    	: 1 ;     	/*!< [Bit 31] 		UART8 clock enable 								*/
    	    }bit;
}RCC_APB1ENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim1en    	:  1 ;     	/*!< [Bit 0] 		TIM1 clock enable 								*/
        volatile uint32_t tim8en    	:  1 ;     	/*!< [Bit 1] 		TIM8 clock enable 								*/
         	 	  uint32_t reserved_0 	:  2 ;     	/*!< [Bits 3:2] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t usart1en  	:  1 ;     	/*!< [Bit 4] 		USART1 clock enable 							*/
        volatile uint32_t usart6en  	:  1 ;     	/*!< [Bit 5] 		USART6 clock enable 							*/
         	 	  uint32_t reserved_1 	:  2 ;     	/*!< [Bits 7:6] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t adc1en    	:  1 ;     	/*!< [Bit 8] 		ADC1 clock enable 								*/
        volatile uint32_t adc2en    	:  1 ;     	/*!< [Bit 9] 		ADC2 clock enable 								*/
        volatile uint32_t adc3en    	:  1 ;     	/*!< [Bit 10] 		ADC3 clock enable 								*/
        volatile uint32_t sdioen    	:  1 ;     	/*!< [Bit 11] 		SDIO clock enable 								*/
        volatile uint32_t spi1en    	:  1 ;     	/*!< [Bit 12] 		SPI1 clock enable 								*/
         	 	  uint32_t reserved_2   :  1 ;     	/*!< [Bit 13] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t syscfgen  	:  1 ;     	/*!< [Bit 14] 		System configuration controller clock enable 	*/
        		  uint32_t reserved_3   :  1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tim9en    	:  1 ;     	/*!< [Bit 16] 		TIM9 clock enable 								*/
        volatile uint32_t tim10en   	:  1 ;     	/*!< [Bit 17] 		TIM10 clock enable 								*/
        volatile uint32_t tim11en   	:  1 ;     	/*!< [Bit 18] 		TIM11 clock enable 								*/
         	 	  uint32_t reserved_4 	: 13 ;     	/*!< [Bits 31:19] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_APB2ENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t gpioalpen    	: 1 ;     	/*!< [Bit 0] 		IO port A clock enable during Sleep mode 		*/
        volatile uint32_t gpioblpen    	: 1 ;     	/*!< [Bit 1] 		IO port B clock enable during Sleep mode 		*/
        volatile uint32_t gpioclpen    	: 1 ;     	/*!< [Bit 2] 		IO port C clock enable during Sleep mode 		*/
        volatile uint32_t gpiodlpen    	: 1 ;     	/*!< [Bit 3] 		IO port D clock enable during Sleep mode 		*/
        volatile uint32_t gpioelpen    	: 1 ;     	/*!< [Bit 4] 		IO port E clock enable during Sleep mode 		*/
        volatile uint32_t gpioflpen    	: 1 ;     	/*!< [Bit 5] 		IO port F clock enable during Sleep mode 		*/
        volatile uint32_t gpioglpen    	: 1 ;     	/*!< [Bit 6] 		IO port G clock enable during Sleep mode 		*/
        volatile uint32_t gpiohlpen    	: 1 ;     	/*!< [Bit 7] 		IO port H clock enable during Sleep mode 		*/
        volatile uint32_t gpioilpen    	: 1 ;     	/*!< [Bit 8] 		IO port I clock enable during Sleep mode 		*/
        volatile uint32_t gpiojlpen    	: 1 ;     	/*!< [Bit 9] 		IO port J clock enable during Sleep mode 		*/
        volatile uint32_t gpioklpen    	: 1 ;     	/*!< [Bit 10] 		IO port K clock enable during Sleep mode 		*/
        		  uint32_t reserved_0   : 1 ;     	/*!< [Bit 11] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t crclpen      	: 1 ;     	/*!< [Bit 12] 		CRC clock enable during Sleep mode 				*/
        		  uint32_t reserved_1  	: 2 ;     	/*!< [Bits 14:13] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t flitflpen    	: 1 ;     	/*!< [Bit 15] 		Flash interface clock enable during Sleep mode 	*/
        volatile uint32_t sram1lpen    	: 1 ;     	/*!< [Bit 16] 		SRAM1 interface clock enable during Sleep mode 	*/
        volatile uint32_t sram2lpen    	: 1 ;     	/*!< [Bit 17] 		SRAM2 interface clock enable during Sleep mode 	*/
        volatile uint32_t bkpsramlpen  	: 1 ;     	/*!< [Bit 18]		Backup SRAM interface clock enable during Sleep mode */
        volatile uint32_t sram3lpen    	: 1 ;     	/*!< [Bit 19] 		SRAM3 interface clock enable during Sleep mode 	*/
        	  	  uint32_t reserved_2   : 1 ;     	/*!< [Bit 20] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t dma1lpen     	: 1 ;     	/*!< [Bit 21] 		DMA1 clock enable during Sleep mode 			*/
        volatile uint32_t dma2lpen     	: 1 ;     	/*!< [Bit 22] 		DMA2 clock enable during Sleep mode 			*/
        volatile uint32_t dma2dlpen    	: 1 ;     	/*!< [Bit 23] 		DMA2D clock enable during Sleep mode 			*/
        		  uint32_t reserved_3  	: 1 ;     	/*!< [Bit 24] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ethmaclpen   	: 1 ;     	/*!< [Bit 25] 		Ethernet MAC clock enable during Sleep mode 	*/
        volatile uint32_t ethmactxlpen 	: 1 ;     	/*!< [Bit 26] 		Ethernet transmission clock enable during Sleep mode */
        volatile uint32_t ethmacrxlpen 	: 1 ;     	/*!< [Bit 27] 		Ethernet reception clock enable during Sleep mode */
        volatile uint32_t ethmacptplpen	: 1 ;     	/*!< [Bit 28] 		Ethernet PTP clock enable during Sleep mode 	*/
        volatile uint32_t otghslpen    	: 1 ;     	/*!< [Bit 29] 		USB OTG HS clock enable during Sleep mode 		*/
        volatile uint32_t otghsulpilpen	: 1 ;     	/*!< [Bit 30] 		USB OTG HS ULPI clock enable during Sleep mode 	*/
        		  uint32_t reserved_4   : 1 ;     	/*!< [Bit 31] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB1LPENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dcmilpen     	:  1 ;     	/*!< [Bit 0] 		Camera interface enable during Sleep mode 		*/
         	 	  uint32_t reserved_0   :  3 ;     	/*!< [Bits 1:3] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cryplpen     	:  1 ;     	/*!< [Bit 4] 		Cryptography modules clock enable during Sleep mode */
        volatile uint32_t hashlpen     	:  1 ;     	/*!< [Bit 5] 		Hash modules clock enable during Sleep mode 	*/
        volatile uint32_t rnglpen      	:  1 ;     	/*!< [Bit 6] 		Random number generator clock enable during Sleep mode */
        volatile uint32_t otgfslpen    	:  1 ;     	/*!< [Bit 7] 		USB OTG FS clock enable during Sleep mode 		*/
         	 	  uint32_t reserved_1  	: 24 ;    	/*!< [Bits 8:31] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB2LPENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fmclpen      	:  1 ;     	/*!< [Bit 0] 		Flexible memory controller module clock enable
        																during Sleep mode 								*/
         	 	  uint32_t reserved    	: 31 ;    	/*!< [Bits 31:1] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_AHB3LPENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim2lpen     	: 1 ;     	/*!< [Bit 0] 		TIM2 clock enable during Sleep mode 			*/
        volatile uint32_t tim3lpen     	: 1 ;     	/*!< [Bit 1] 		TIM3 clock enable during Sleep mode 			*/
        volatile uint32_t tim4lpen     	: 1 ;     	/*!< [Bit 2] 		TIM4 clock enable during Sleep mode 			*/
        volatile uint32_t tim5lpen     	: 1 ;     	/*!< [Bit 3] 		TIM5 clock enable during Sleep mode 			*/
        volatile uint32_t tim6lpen     	: 1 ;     	/*!< [Bit 4] 		TIM6 clock enable during Sleep mode 			*/
        volatile uint32_t tim7lpen     	: 1 ;     	/*!< [Bit 5] 		TIM7 clock enable during Sleep mode 			*/
        volatile uint32_t tim12lpen    	: 1 ;     	/*!< [Bit 6] 		TIM12 clock enable during Sleep mode 			*/
        volatile uint32_t tim13lpen    	: 1 ;     	/*!< [Bit 7] 		TIM13 clock enable during Sleep mode 			*/
        volatile uint32_t tim14lpen    	: 1 ;     	/*!< [Bit 8] 		TIM14 clock enable during Sleep mode 			*/
        		  uint32_t reserved_0   : 2 ;     	/*!< [Bits 10:9] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t wwdglpen     	: 1 ;     	/*!< [Bit 11] 		Window watchdog clock enable during Sleep mode 	*/
        		  uint32_t reserved_1   : 2 ;     	/*!< [Bits 13:12] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t spi2lpen     	: 1 ;     	/*!< [Bit 14] 		SPI2 clock enable during Sleep mode 			*/
        volatile uint32_t spi3lpen     	: 1 ;     	/*!< [Bit 15] 		SPI3 clock enable during Sleep mode 			*/
        		  uint32_t reserved_2   : 1 ;     	/*!< [Bit 16] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t usart2lpen   	: 1 ;     	/*!< [Bit 17] 		USART2 clock enable during Sleep mode 			*/
        volatile uint32_t usart3lpen   	: 1 ;     	/*!< [Bit 18] 		USART3 clock enable during Sleep mode 			*/
        volatile uint32_t uart4lpen    	: 1 ;     	/*!< [Bit 19] 		UART4 clock enable during Sleep mode 			*/
        volatile uint32_t uart5lpen    	: 1 ;     	/*!< [Bit 20] 		UART5 clock enable during Sleep mode 			*/
        volatile uint32_t i2c1lpen     	: 1 ;     	/*!< [Bit 21] 		I2C1 clock enable during Sleep mode 			*/
        volatile uint32_t i2c2lpen     	: 1 ;     	/*!< [Bit 22] 		I2C2 clock enable during Sleep mode 			*/
        volatile uint32_t i2c3lpen     	: 1 ;     	/*!< [Bit 23] 		I2C3 clock enable during Sleep mode 			*/
        		  uint32_t reserved_3   : 1 ;     	/*!< [Bit 24] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t can1lpen     	: 1 ;     	/*!< [Bit 25]	 	CAN 1 clock enable during Sleep mode 			*/
        volatile uint32_t can2lpen     	: 1 ;     	/*!< [Bit 26] 		CAN 2 clock enable during Sleep mode 			*/
        volatile uint32_t pwrlpen      	: 1 ;     	/*!< [Bit 28] 		Power interface clock enable during Sleep mode 	*/
        volatile uint32_t daclpen      	: 1 ;     	/*!< [Bit 29] 		DAC interface clock enable during Sleep mode 	*/
        volatile uint32_t uart7lpen    	: 1 ;     	/*!< [Bit 30] 		UART7 clock enable during Sleep mode 			*/
        volatile uint32_t uart8lpen   	: 1 ;     	/*!< [Bit 31] 		UART8 clock enable during Sleep mode 			*/
    	   }bit;
}RCC_APB1LPENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tim1lpen    	: 1 ;     	/*!< [Bit 0] 		TIM1 clock enable during Sleep mode 			*/
        volatile uint32_t tim8lpen     	: 1 ;     	/*!< [Bit 1] 		TIM8 clock enable during Sleep mode 			*/
         	 	  uint32_t reserved_0   : 2 ;     	/*!< [Bits 3:2] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t usart1lpen   	: 1 ;     	/*!< [Bit 4] 		USART1 clock enable during Sleep mode 			*/
        volatile uint32_t usart6lpen   	: 1 ;     	/*!< [Bit 5] 		USART6 clock enable during Sleep mode 			*/
         	 	  uint32_t reserved_1   : 2 ;     	/*!< [Bits 6:7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t adc1lpen     	: 1 ;     	/*!< [Bit 8] 		ADC1 clock enable during Sleep mode 			*/
        volatile uint32_t adc2lpen     	: 1 ;     	/*!< [Bit 9] 		ADC2 clock enable during Sleep mode 			*/
        volatile uint32_t adc3lpen     	: 1 ;     	/*!< [Bit 10] 		ADC3 clock enable during Sleep mode 			*/
        volatile uint32_t sdiolpen     	: 1 ;     	/*!< [Bit 11] 		SDIO clock enable during Sleep mode 			*/
        volatile uint32_t spi1lpen     	: 1 ;     	/*!< [Bit 12] 		SPI1 clock enable during Sleep mode 			*/
        volatile uint32_t spi4lpen     	: 1 ;     	/*!< [Bit 13] 		SPI4 clock enable during Sleep mode 			*/
        volatile uint32_t syscfglpen   	: 1 ;     	/*!< [Bit 14] 		System configuration controller clock enable during Sleep mode */
         	 	  uint32_t reserved_2   : 1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tim9lpen     	: 1 ;     	/*!< [Bit 16] 		TIM9 clock enable during Sleep mode 			*/
        volatile uint32_t tim10lpen    	: 1 ;     	/*!< [Bit 17] 		TIM10 clock enable during Sleep mode 			*/
        volatile uint32_t tim11lpen    	: 1 ;     	/*!< [Bit 18] 		TIM11 clock enable during Sleep mode 			*/
         	 	  uint32_t reserved_3   : 1 ;     	/*!< [Bit 19] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t spi5lpen     	: 1 ;     	/*!< [Bit 20] 		SPI5 clock enable during Sleep mode 			*/
        volatile uint32_t spi6lpen     	: 1 ;     	/*!< [Bit 21] 		SPI6 clock enable during Sleep mode 			*/
        volatile uint32_t sai1lpen     	: 1 ;     	/*!< [Bit 22] 		SAI1 clock enable during Sleep mode 			*/
         	 	  uint32_t reserved_4   : 3 ;     	/*!< [Bits 25:23] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t ltdclpen     	: 1 ;     	/*!< [Bit 26] 		LTDC clock enable during Sleep mode 			*/
         	  	  uint32_t reserved_5   : 5 ;     	/*!< [Bits 31:27] 	Reserved, must be kept at reset value 			*/
    }bit;
}RCC_APB2LPENR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t lseon        	:  1 ;     	/*!< [Bit 0] 		External low-speed oscillator enable 			*/
        volatile uint32_t lserdy       	:  1 ;     	/*!< [Bit 1] 		External low-speed oscillator ready 			*/
        volatile uint32_t lsebyp       	:  1 ;     	/*!< [Bit 2] 		External low-speed oscillator bypass 			*/
         	 	  uint32_t reserved_0 	:  5 ;     	/*!< [Bits 3:7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t rtcssel      	:  2 ;     	/*!< [Bits 8:9] 		RTC clock source selection 						*/
         	 	  uint32_t reserved_1   :  5 ;     	/*!< [Bits 10:14] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t rtcen        	:  1 ;     	/*!< [Bit 15] 		RTC clock enable 								*/
        volatile uint32_t bdrst        	:  1 ;     	/*!< [Bit 16] 		Backup domain software reset 					*/
         	 	  uint32_t reserved_2   : 15 ;     	/*!< [Bits 17:31] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RCC_BDCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t lsion      	:  1 ;     	/*!< [Bit 0] 		Internal low-speed oscillator enable 			*/
        volatile uint32_t lsirdy      	:  1 ;     	/*!< [Bit 1] 		Internal low-speed oscillator ready 			*/
         	 	  uint32_t reserved   	: 22 ;     	/*!< [Bits 23:2] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t rmvf        	:  1 ;     	/*!< [Bit 24] 		Remove reset flag 								*/
        volatile uint32_t borrstf     	:  1 ;     	/*!< [Bit 25] 		BOR reset flag 									*/
        volatile uint32_t pinrstf     	:  1 ;     	/*!< [Bit 26] 		PIN reset flag 									*/
        volatile uint32_t porrstf     	:  1 ;     	/*!< [Bit 27] 		POR/PDR reset flag 								*/
        volatile uint32_t sftrstf     	:  1 ;     	/*!< [Bit 28] 		Software reset flag	 							*/
        volatile uint32_t iwdgrstf    	:  1 ;     	/*!< [Bit 29] 		Independent watchdog reset flag 				*/
        volatile uint32_t wwdgrstf    	:  1 ;     	/*!< [Bit 30] 		Window watchdog reset flag 						*/
        volatile uint32_t lpwrrstf    	:  1 ;     	/*!< [Bit 31] 		Low-power reset flag 							*/
    	   }bit;
}RCC_CSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t modper       	: 13 ;     	/*!< [Bits 0:12] 	Modulation period 								*/
        volatile uint32_t incstep      	: 15 ;     	/*!< [Bits 27:13] 	Incrementation step 							*/
         	 	  uint32_t reserved   	:  2 ;     	/*!< [Bits 29:28] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t spreadsel    	:  1 ;     	/*!< [Bit 30] 		Spread Select 									*/
        volatile uint32_t sscgen       	:  1 ;     	/*!< [Bit 31] 		Spread spectrum modulation enable 				*/
    	   }bit;
}RCC_SSCGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t reserved_0   	: 6 ;     	/*!< [Bits 0:5] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t plli2sn      	: 9 ;     	/*!< [Bits 14:6] 	PLLI2S multiplication factor for VCO 			*/
        volatile uint32_t reserved_1   	: 9 ;     	/*!< [Bits 23:15] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t plli2sq      	: 4 ;     	/*!< [Bits 27:24] 	PLLI2S division factor for SAI1 clock 			*/
        volatile uint32_t plli2sr      	: 3 ;     	/*!< [Bits 30:28] 	PLLI2S division factor for I2S clocks 			*/
        volatile uint32_t reserved_2   	: 1 ;     	/*!< [Bit 31] 		Reserved, must be kept at reset value 			*/
    }bit;
}RCC_PLLI2SCFGR_Reg_t;

typedef struct
{
RCC_CR_Reg_t 		CR;          	/*!< RCC clock control register,                                  Address offset: 0x00 */
RCC_PLLCFGR_Reg_t 	PLLCFGR;       	/*!< RCC PLL configuration register,                              Address offset: 0x04 */
RCC_CFGR_Reg_t 		CFGR;          	/*!< RCC clock configuration register,                            Address offset: 0x08 */
RCC_CIR_Reg_t 		CIR;           	/*!< RCC clock interrupt register,                                Address offset: 0x0C */
RCC_AHB1RSTR_Reg_t 	AHB1RSTR;     	/*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
RCC_AHB2RSTR_Reg_t 	AHB2RSTR;      	/*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
RCC_AHB3RSTR_Reg_t 	AHB3RSTR;     	/*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint32_t      	RESERVED0;     	/*!< Reserved, 0x1C                                                                    */
RCC_APB1RSTR_Reg_t 	APB1RSTR;      	/*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
RCC_APB2RSTR_Reg_t 	APB2RSTR;     	/*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint32_t      	RESERVED1;  	/*!< Reserved, 0x28-0x2C                                                               */
RCC_AHB1ENR_Reg_t 	AHB1ENR;      	/*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
RCC_AHB2ENR_Reg_t 	AHB2ENR;      	/*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
RCC_AHB3ENR_Reg_t 	AHB3ENR;      	/*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      	RESERVED2;    	/*!< Reserved, 0x3C                                                                    */
RCC_APB1ENR_Reg_t 	APB1ENR;      	/*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
RCC_APB2ENR_Reg_t 	APB2ENR;      	/*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint32_t      	RESERVED3[2];  	/*!< Reserved, 0x48-0x4C                                                               */
RCC_AHB1LPENR_Reg_t AHB1LPENR;   	/*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
RCC_AHB2LPENR_Reg_t AHB2LPENR;  	/*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
RCC_AHB3LPENR_Reg_t AHB3LPENR;    	/*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint32_t      	RESERVED4;      /*!< Reserved, 0x5C                                                                    */
RCC_APB1LPENR_Reg_t APB1LPENR;    	/*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
RCC_APB2LPENR_Reg_t APB2LPENR;    	/*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t      	RESERVED5[2];  	/*!< Reserved, 0x68-0x6C                                                               */
RCC_BDCR_Reg_t 		BDCR;          	/*!< RCC Backup domain control register,                          Address offset: 0x70 */
RCC_CSR_Reg_t 		CSR;           	/*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint32_t      	RESERVED6[2];  	/*!< Reserved, 0x78-0x7C                                                               */
RCC_SSCGR_Reg_t 	SSCGR;         	/*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
RCC_PLLI2SCFGR_Reg_t PLLI2SCFGR;  	/*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
}RCC_RegDef_t;

/**
  * @brief Real-Time Clock
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t su         	: 4 ;     	/*!< [Bits 0:3] 		Second units in BCD format 						*/
        volatile uint32_t st         	: 3 ;     	/*!< [Bits 4:6] 		Second tens in BCD format 						*/
        		  uint32_t reserved_0 	: 1 ;     	/*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mnu        	: 4 ;     	/*!< [Bits 11:8] 	Minute units in BCD format 						*/
        volatile uint32_t mnt        	: 3 ;     	/*!< [Bits 14:12] 	Minute tens in BCD format 						*/
        	  	  uint32_t reserved_1 	: 1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t hu         	: 4 ;     	/*!< [Bits 19:16] 	Hour units in BCD format 						*/
        volatile uint32_t ht         	: 2 ;     	/*!< [Bits 21:20] 	Hour tens in BCD format 						*/
        volatile uint32_t pm         	: 1 ;     	/*!< [Bit 22] 		AM/PM notation 									*/
        		  uint32_t reserved_2 	: 9 ;     	/*!< [Bits 31:23] 	Reserved 										*/
    	   }bit;
}RTC_TR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t du         	: 4 ;     	/*!< [Bits 0:3] 		Date units in BCD format 						*/
        volatile uint32_t dt         	: 2 ;     	/*!< [Bits 4:5]	 	Date tens in BCD format 						*/
         	 	  uint32_t reserved_0 	: 2 ;     	/*!< [Bits 6:7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mu         	: 4 ;     	/*!< [Bits 8:11] 	Month units in BCD format 						*/
        volatile uint32_t mt         	: 1 ;     	/*!< [Bit 12] 		Month tens in BCD format 						*/
        volatile uint32_t wdu        	: 3 ;     	/*!< [Bits 13:15] 	Week day units (forbidden values are 000) 		*/
        volatile uint32_t yu         	: 4 ;     	/*!< [Bits 16:19] 	Year units in BCD format 						*/
        volatile uint32_t yt         	: 4 ;     	/*!< [Bits 20:23] 	Year tens in BCD format 						*/
         	 	  uint32_t reserved_1 	: 8 ;     	/*!< [Bits 24:31] 	Reserved 										*/
    	   }bit;
}RTC_DR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t wucksel     	: 3 ;     	/*!< [Bits 2:0] 		Wakeup clock selection 							*/
        volatile uint32_t tsedge      	: 1 ;     	/*!< [Bit 3] 		Timestamp event active edge 					*/
        volatile uint32_t refckon     	: 1 ;     	/*!< [Bit 4] 		Reference clock detection enable (50 or 60 Hz) 	*/
        volatile uint32_t bypshad     	: 1 ;     	/*!< [Bit 5] 		Bypass the shadow registers 					*/
        volatile uint32_t fmt         	: 1 ;     	/*!< [Bit 6] 		Hour format 									*/
        volatile uint32_t dce         	: 1 ;     	/*!< [Bit 7] 		Coarse digital calibration enable 				*/
        volatile uint32_t alrae       	: 1 ;     	/*!< [Bit 8] 		Alarm A enable 									*/
        volatile uint32_t alrbe       	: 1 ;     	/*!< [Bit 9] 		larm B enable 									*/
        volatile uint32_t wute       	: 1 ;     	/*!< [Bit 10] 		Wakeup timer enable 							*/
        volatile uint32_t tse        	: 1 ;     	/*!< [Bit 11] 		Time stamp enable 								*/
        volatile uint32_t alraie     	: 1 ;     	/*!< [Bit 12] 		Alarm A interrupt enable 						*/
        volatile uint32_t alrbie     	: 1 ;     	/*!< [Bit 13] 		Alarm B interrupt enable 						*/
        volatile uint32_t wutie      	: 1 ;     	/*!< [Bit 14] 		Wakeup timer interrupt enable 					*/
        volatile uint32_t tsie       	: 1 ;     	/*!< [Bit 15] 		Timestamp interrupt enable 						*/
        volatile uint32_t add1h      	: 1 ;     	/*!< [Bit 16] 		Add 1 hour (summer time change) 				*/
        volatile uint32_t sub1h      	: 1 ;     	/*!< [Bit 17] 		Subtract 1 hour (winter time change) 			*/
        volatile uint32_t bkp        	: 1 ;     	/*!< [Bit 18] 		Backup 											*/
        volatile uint32_t cosel      	: 1 ;     	/*!< [Bit 19] 		Calibration output selection 					*/
        volatile uint32_t pol        	: 1 ;     	/*!< [Bit 20] 		Output polarity 								*/
        volatile uint32_t osel       	: 2 ;     	/*!< [Bits 22:21] 	Output selection 								*/
        volatile uint32_t coe        	: 1 ;     	/*!< [Bit 23] 		Calibration output enable 						*/
        		  uint32_t reserved 	: 8 ;     	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RTC_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t alrawf     	:  1 ;     	/*!< [Bit 0] 		Alarm A write flag 								*/
        volatile uint32_t alrbwf     	:  1 ;     	/*!< [Bit 1] 		Alarm B write flag 								*/
        volatile uint32_t wutwf     	:  1 ;     	/*!< [Bit 2] 		Wakeup timer write flag 						*/
        volatile uint32_t shpf      	:  1 ;     	/*!< [Bit 3] 		Shift operation pending 						*/
        volatile uint32_t inits     	:  1 ;     	/*!< [Bit 4] 		Initialization status flag 						*/
        volatile uint32_t rsf       	:  1 ;     	/*!< [Bit 5] 		Registers synchronization flag 					*/
        volatile uint32_t initf     	:  1 ;     	/*!< [Bit 6] 		Initialization flag 							*/
        volatile uint32_t init      	:  1 ;     	/*!< [Bit 7] 		Initialization mode 							*/
        volatile uint32_t alraf     	:  1 ;     	/*!< [Bit 8] 		Alarm A flag 									*/
        volatile uint32_t alrbf     	:  1 ;     	/*!< [Bit 9] 		Alarm B flag 									*/
        volatile uint32_t wutf      	:  1 ;     	/*!< [Bit 10] 		Wakeup timer flag 								*/
        volatile uint32_t tsf       	:  1 ;     	/*!< [Bit 11] 		Timestamp flag 									*/
        volatile uint32_t tsovf     	:  1 ;     	/*!< [Bit 12] 		Timestamp overflow flag 						*/
        volatile uint32_t tamp1f    	:  1 ;     	/*!< [Bit 13] 		Tamper detection flag 							*/
        volatile uint32_t tamp2f    	:  1 ;     	/*!< [Bit 14] 		TAMPER2 detection flag 							*/
        		  uint32_t reserved_0	:  1 ;     	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t recalpf   	:  1 ;     	/*!< [Bit 16] 		Recalibration pending Flag 						*/
        		  uint32_t reserved_1	: 15 ;     	/*!< [Bits 31:17] 	Reserved 										*/
    	   }bit;
}RTC_ISR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t prediv_s  	: 15 ;    	/*!< [Bits 14:0] 	Synchronous prescaler factor 					*/
        		  uint32_t reserved_0	:  1 ;    	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t prediv_a  	:  7 ;    	/*!< [Bits 22:16] 	Asynchronous prescaler factor 					*/
        		  uint32_t reserved_1	:  9 ;    	/*!< [Bits 31:23] 	Reserved 										*/
    	   }bit;
}RTC_PRER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t wut  			: 16 ;    	/*!< [Bits 15:0] 	Wakeup auto-reload value bits 					*/
        		  uint32_t reserved		: 16 ;  	/*!< [Bits 31:16] 	Reserved 										*/
    	   }bit;
}RTC_WUTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dc    		:  5 ;     	/*!< [Bits 4:0] 		Digital calibration 							*/
        		  uint32_t reserved_0  	:  2 ; 		/*!< [Bits 6:5] 		Reserved 										*/
        volatile uint32_t dcs   		:  1 ;     	/*!< [Bit 7] 		Digital calibration sign 						*/
        		  uint32_t reserved_1  	: 24 ; 		/*!< [Bits 31:8] 	Reserved 										*/
    	   }bit;
}RTC_CALIBR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t su    		: 4 ;     	/*!< [Bits 3:0] 		Second units in BCD format 						*/
        volatile uint32_t st    		: 3 ;     	/*!< [Bits 6:4] 		Second tens in BCD format 						*/
        volatile uint32_t msk1  		: 1 ;     	/*!< [Bit 7] 		Alarm x seconds mask 							*/
        volatile uint32_t mnu   		: 4 ;     	/*!< [Bits 11:8] 	Minute units in BCD format 						*/
        volatile uint32_t mnt   		: 3 ;     	/*!< [Bits 14:12] 	Minute tens in BCD format 						*/
        volatile uint32_t msk2  		: 1 ;     	/*!< [Bit 15] 		Alarm x minutes mask 							*/
        volatile uint32_t hu    		: 4 ;     	/*!< [Bits 19:16] 	Hour units in BCD format 						*/
        volatile uint32_t ht    		: 2 ;     	/*!< [Bits 21:20] 	Hour tens in BCD format 						*/
        volatile uint32_t pm    		: 1 ;     	/*!< [Bit 22] 		AM/PM notation 									*/
        volatile uint32_t msk3  		: 1 ;     	/*!< [Bit 23] 		Alarm x hours mask 								*/
        volatile uint32_t du    		: 4 ;     	/*!< [Bits 27:24] 	Date units or day in BCD format 				*/
        volatile uint32_t dt    		: 2 ;     	/*!< [Bits 29:28] 	Date tens in BCD format 						*/
        volatile uint32_t wdsel 		: 1 ;     	/*!< [Bit 30] 		Week day selection 								*/
        volatile uint32_t msk4  		: 1 ;     	/*!< [Bit 31] 		Alarm x date mask 								*/
    	   }bit;
}RTC_ALRMx_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t key   		:  8 ;     	/*!< [Bits 7:0] 		Write protection key 							*/
         	 	  uint32_t reserved 	: 24 ; 		/*!< [Bits 31:8] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RTC_WPR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ss   			: 16 ;    	/*!< [Bits 15:0] 	Sub second value 								*/
         	 	  uint32_t reserved 	: 16 ; 		/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RTC_SSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t subfs  		: 15 ;  	/*!< [Bits 14:0] 	Subtract a fraction of a second 				*/
        		  uint32_t reserved 	: 16 ; 		/*!< [Bits 30:15] 	Reserved 										*/
        volatile uint32_t add1s  		:  1 ;   	/*!< [Bit 31] 		Add one second 									*/
    	   }bit;
}RTC_SHIFTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t su  			: 4 ;      	/*!< [Bits 3:0] 		Second units in BCD format 						*/
        volatile uint32_t st  			: 3 ;      	/*!< [Bits 6:4] 		Second tens in BCD format 						*/
        		  uint32_t reserved_0  	: 1 ;       /*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mnu  			: 4 ;     	/*!< [Bits 11:8] 	Minute units in BCD format 						*/
        volatile uint32_t mnt  			: 3 ;     	/*!< [Bits 14:12] 	Minute tens in BCD format 						*/
        		  uint32_t reserved_1  	: 1 ;       /*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t hu  			: 4 ;      	/*!< [Bits 19:16] 	Hour units in BCD format 						*/
        volatile uint32_t ht  			: 2 ;      	/*!< [Bits 21:20] 	Hour tens in BCD format 						*/
        volatile uint32_t pm  			: 1 ;      	/*!< [Bit 22] 		AM/PM notation 									*/
        		  uint32_t reserved_2  	: 9 ;       /*!< [Bits 31:23] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RTC_TSTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t du  			:  4 ;      /*!< [Bits 0:3] 		Date units in BCD format 						*/
        volatile uint32_t dt  			:  2 ;      /*!< [Bits 5:4] 		Date tens in BCD format 						*/
        		  uint32_t reserved_1  	:  2 ;      /*!< [Bits 7:6] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mu  			:  4 ;      /*!< [Bits 11:8] 	Month units in BCD format 						*/
        volatile uint32_t mt  			:  1 ;    	/*!< [Bit 12] 		Month tens in BCD format 						*/
        volatile uint32_t wdu  			:  3 ;     	/*!< [Bits 15:13] 	Week day units 									*/
        		  uint32_t reserved_3  	: 16 ;      /*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RTC_TSDR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ss       		: 16 ; 		/*!< [Bits 15:0] 	Sub second value 								*/
        		  uint32_t reserved    	: 16 ; 		/*!< [Bits 31:16] 	Reserved 										*/
    	   }bit;
}RTC_TSSSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t calm       	:  9 ;  	/*!< [Bits 8:0] 		Calibration minus 								*/
        	  	  uint32_t reserved_0	:  4 ;  	/*!< [Bits 12:9] 	Reserved 										*/
        volatile uint32_t calw16       	:  1 ;  	/*!< [Bit 13] 		Use a 16-second calibration cycle period 		*/
        volatile uint32_t calw8  		:  1 ;  	/*!< [Bit 14] 		Use an 8-second calibration cycle period 		*/
        volatile uint32_t calp    		:  1 ;  	/*!< [Bit 15] 		Increase frequency of RTC by 488.5 ppm 			*/
        		  uint32_t reserved_1 	: 16 ; 		/*!< [Bits 31:16] 	Reserved 										*/
    }bit;
}RTC_CALR_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t tamp1e      	:  1 ;  	/*!< [Bit 0] 		Tamper 1 detection enable 						*/
        volatile uint32_t tamp1trg		:  1 ;  	/*!< [Bit 1] 		Active level for tamper 1 						*/
        volatile uint32_t tampie     	:  1 ;  	/*!< [Bit 2] 		Tamper interrupt enable 						*/
        volatile uint32_t tamp2e     	:  1 ;  	/*!< [Bit 3] 		Tamper 2 detection enable 						*/
        volatile uint32_t tamp2trg   	:  1 ;  	/*!< [Bit 4] 		Active level for tamper 2 						*/
        		  uint32_t reserved_0   :  2 ;  	/*!< [Bits 6:5] 		Reserved 										*/
        volatile uint32_t tampts     	:  1 ;  	/*!< [Bit 7] 		Activate timestamp on tamper detection event 	*/
        volatile uint32_t tampfreq   	:  3 ;  	/*!< [Bits 10:8] 	Tamper sampling frequency 						*/
        volatile uint32_t tampflt    	:  2 ;  	/*!< [Bits 12:11] 	Tamper filter count 							*/
        volatile uint32_t tampprch   	:  2 ;  	/*!< [Bits 14:13] 	Tamper precharge duration 						*/
        volatile uint32_t tamppudis  	:  1 ;  	/*!< [Bit 15] 		TAMPER pull-up disable 							*/
        volatile uint32_t tamp1insel 	:  1 ;  	/*!< [Bit 16] 		TAMP1INSEL: TAMPER1 mapping 					*/
        volatile uint32_t tsinsel    	:  1 ;  	/*!< [Bit 17] 		TSINSEL: TIMESTAMP mapping 						*/
        volatile uint32_t alarmouttype 	:  1 ; 		/*!< [Bit 18] 		ALARMOUTTYPE: RTC_ALARM output type 			*/
        		  uint32_t reserved_1 	: 13 ; 		/*!< [Bits 31:19] 	Reserved 										*/
    	   }bit;
}RTC_TAFCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ss         	: 15 ; 		/*!< [Bits 14:0] 	Sub seconds value 								*/
        		  uint32_t reserved_0 	:  9 ;  	/*!< [Bits 23:15] 	Reserved 										*/
        volatile uint32_t maskss    	:  4 ;  	/*!< [Bits 27:24] 	Mask the most-significant bits starting at this bit */
        		  uint32_t reserved_1  	:  4 ;  	/*!< [Bits 31:28] 	Reserved 										*/
    	   }bit;
}RTC_ALRMxSS_Reg_t;

typedef struct
{
RTC_TR_Reg_t       TR;      	/*!< RTC time register,                                        Address offset: 0x00 */
RTC_DR_Reg_t       DR;      	/*!< RTC date register,                                        Address offset: 0x04 */
RTC_CR_Reg_t       CR;      	/*!< RTC control register,                                     Address offset: 0x08 */
RTC_ISR_Reg_t      ISR;     	/*!< RTC initialization and status register,                   Address offset: 0x0C */
RTC_PRER_Reg_t     PRER;    	/*!< RTC prescaler register,                                   Address offset: 0x10 */
RTC_WUTR_Reg_t     WUTR;    	/*!< RTC wakeup timer register,                                Address offset: 0x14 */
RTC_CALIBR_Reg_t   CALIBR;  	/*!< RTC calibration register,                                 Address offset: 0x18 */
RTC_ALRMx_Reg_t    ALRMAR;  	/*!< RTC alarm A register,                                     Address offset: 0x1C */
RTC_ALRMx_Reg_t    ALRMBR;  	/*!< RTC alarm B register,                                     Address offset: 0x20 */
RTC_WPR_Reg_t      WPR;     	/*!< RTC write protection register,                            Address offset: 0x24 */
RTC_SSR_Reg_t      SSR;     	/*!< RTC sub second register,                                  Address offset: 0x28 */
RTC_SHIFTR_Reg_t   SHIFTR;  	/*!< RTC shift control register,                               Address offset: 0x2C */
RTC_TSTR_Reg_t     TSTR;    	/*!< RTC time stamp time register,                             Address offset: 0x30 */
RTC_TSDR_Reg_t     TSDR;    	/*!< RTC time stamp date register,                             Address offset: 0x34 */
RTC_TSSSR_Reg_t    TSSSR;   	/*!< RTC time-stamp sub second register,                       Address offset: 0x38 */
RTC_CALR_Reg_t     CALR;    	/*!< RTC calibration register,                                 Address offset: 0x3C */
RTC_TAFCR_Reg_t    TAFCR;   	/*!< RTC tamper and alternate function configuration register, Address offset: 0x40 */
RTC_ALRMxSS_Reg_t  ALRMASSR;	/*!< RTC alarm A sub second register,                          Address offset: 0x44 */
RTC_ALRMxSS_Reg_t  ALRMBSSR;	/*!< RTC alarm B sub second register,                          Address offset: 0x48 */
  uint32_t 		   RESERVED_7;  /*!< Reserved, 0x4C                                                                 */
volatile uint32_t BKP0R;   		/*!< RTC backup register 1,                                    Address offset: 0x50 */
volatile uint32_t BKP1R;   		/*!< RTC backup register 1,                                    Address offset: 0x54 */
volatile uint32_t BKP2R;   		/*!< RTC backup register 2,                                    Address offset: 0x58 */
volatile uint32_t BKP3R;   		/*!< RTC backup register 3,                                    Address offset: 0x5C */
volatile uint32_t BKP4R;   		/*!< RTC backup register 4,                                    Address offset: 0x60 */
volatile uint32_t BKP5R;   		/*!< RTC backup register 5,                                    Address offset: 0x64 */
volatile uint32_t BKP6R;   		/*!< RTC backup register 6,                                    Address offset: 0x68 */
volatile uint32_t BKP7R;   		/*!< RTC backup register 7,                                    Address offset: 0x6C */
volatile uint32_t BKP8R;   		/*!< RTC backup register 8,                                    Address offset: 0x70 */
volatile uint32_t BKP9R;   		/*!< RTC backup register 9,                                    Address offset: 0x74 */
volatile uint32_t BKP10R;  		/*!< RTC backup register 10,                                   Address offset: 0x78 */
volatile uint32_t BKP11R;  		/*!< RTC backup register 11,                                   Address offset: 0x7C */
volatile uint32_t BKP12R;  		/*!< RTC backup register 12,                                   Address offset: 0x80 */
volatile uint32_t BKP13R;  		/*!< RTC backup register 13,                                   Address offset: 0x84 */
volatile uint32_t BKP14R;  		/*!< RTC backup register 14,                                   Address offset: 0x88 */
volatile uint32_t BKP15R;  		/*!< RTC backup register 15,                                   Address offset: 0x8C */
volatile uint32_t BKP16R;  		/*!< RTC backup register 16,                                   Address offset: 0x90 */
volatile uint32_t BKP17R;  		/*!< RTC backup register 17,                                   Address offset: 0x94 */
volatile uint32_t BKP18R;  		/*!< RTC backup register 18,                                   Address offset: 0x98 */
volatile uint32_t BKP19R;  		/*!< RTC backup register 19,                                   Address offset: 0x9C */
}RTC_RegDef_t;

/**
  * @brief SD host Interface
  */
typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pwrctrl      	:  2 ; 		/*!< [Bits 1:0] 		Power supply control bits. 						*/
        		  uint32_t reserved  	: 30 ;  	/*!< [Bits 31:2] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_POWER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t clkdiv     	:  8 ;  	/*!< [Bits 7:0] 		Clock divide factor 							*/
        volatile uint32_t clken      	:  1 ;  	/*!< [Bit 8] 		Clock enable bit 								*/
        volatile uint32_t pwrsav     	:  1 ;  	/*!< [Bit 9] 		Power saving configuration bit 					*/
        volatile uint32_t bypass     	:  1 ;  	/*!< [Bit 10] 		Clock divider bypass enable bit 				*/
        volatile uint32_t widbus     	:  2 ;  	/*!< [Bits 12:11] 	Wide bus mode enable bit 						*/
        volatile uint32_t negedge    	:  1 ;  	/*!< [Bit 13] 		SDIO_CK dephasing selection bit 				*/
        volatile uint32_t hwfc_en    	:  1 ;  	/*!< [Bit 14] 		HW Flow Control enable 							*/
        		  uint32_t reserved   	: 17 ; 		/*!< [Bits 31:15] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_CLKCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cmdindex   	:  6 ;  	/*!< [Bits 5:0] 		Command index 									*/
        volatile uint32_t waitresp   	:  2 ;  	/*!< [Bits 7:6] 		Wait for response bits 							*/
        volatile uint32_t waitint    	:  1 ;  	/*!< [Bit 8] 		CPSM waits for interrupt request 				*/
        volatile uint32_t waitpend   	:  1 ;  	/*!< [Bit 9] 		CPSM Waits for ends of data transfer 			*/
        volatile uint32_t cpsmen     	:  1 ;  	/*!< [Bit 10] 		Command path state machine (CPSM) Enable bit 	*/
        volatile uint32_t sdiosuspend	:  1 ;  	/*!< [Bit 11] 		SD I/O suspend command 							*/
        volatile uint32_t encmdcompl 	:  1 ;  	/*!< [Bit 12] 		Enable CMD completion 							*/
        volatile uint32_t nien      	:  1 ;  	/*!< [Bit 13] 		not Interrupt Enable 							*/
        volatile uint32_t ce_atacmd    	:  1 ;  	/*!< [Bit 14] 		CE-ATA command 									*/
        		  uint32_t reserved    	: 17 ; 		/*!< [Bits 31:15] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_CMD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
    volatile const uint32_t respcmd	:  6 ; 		/*!< [Bits 5:0] 		Response command index. 						*/
        		  uint32_t reserved  	: 26 ;  	/*!< [Bits 31:6] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_RESPCMD_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t datalength   	: 25 ; 		/*!< [Bits 24:0] 	Data length value 								*/
        		  uint32_t reserved  	:  7 ;  	/*!< [Bits 31:25] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_DLEN_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dten       	:  1 ;  	/*!< [Bit 0] 		Data transfer enabled bit 						*/
        volatile uint32_t dtdir      	:  1 ;  	/*!< [Bit 1] 		Data transfer direction selection 				*/
        volatile uint32_t dtmode     	:  1 ;  	/*!< [Bit 2] 		Data transfer mode selection 					*/
        volatile uint32_t dmaen      	:  1 ;  	/*!< [Bit 3] 		DMA enable bit 									*/
        volatile uint32_t dblocksize 	:  4 ;  	/*!< [Bits 7:4] 		Data block size 								*/
        volatile uint32_t rwstart    	:  1 ;  	/*!< [Bit 8] 		Read wait start 								*/
        volatile uint32_t rwstop     	:  1 ;  	/*!< [Bit 9] 		Read wait stop 									*/
        volatile uint32_t rwmod      	:  1 ;  	/*!< [Bit 10] 		Read wait mode 									*/
        volatile uint32_t sdioen     	:  1 ;  	/*!< [Bit 11] 		SD I/O enable functions 						*/
        		  uint32_t reserved     : 20 ; 		/*!< [Bits 31:12] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_DCTRL_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t datacount   	: 25 ; 		/*!< [Bits 24:0] 	Data count value 								*/
        		  uint32_t reserved  	:  7 ;  	/*!< [Bits 31:25] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_DCOUNT_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ccrcfail   	: 1 ;  		/*!< [Bit 0] 		Command response received (CRC check failed) 	*/
        volatile uint32_t dcrcfail   	: 1 ;  		/*!< [Bit 1] 		Data block sent/received (CRC check failed) 	*/
        volatile uint32_t ctimeout   	: 1 ;  		/*!< [Bit 2] 		Command response timeout 						*/
        volatile uint32_t dtimeout   	: 1 ;  		/*!< [Bit 3] 		Data timeout 									*/
        volatile uint32_t txunderr   	: 1 ;  		/*!< [Bit 4] 		Transmit FIFO underrun error 					*/
        volatile uint32_t rxoverr    	: 1 ;  		/*!< [Bit 5] 		Received FIFO overrun error 					*/
        volatile uint32_t cmdrend    	: 1 ;  		/*!< [Bit 6] 		Command response received (CRC check passed) 	*/
        volatile uint32_t cmdsent    	: 1 ;  		/*!< [Bit 7] 		Command sent (no response required) 			*/
        volatile uint32_t dataend    	: 1 ;  		/*!< [Bit 8] 		Data end (data counter, SDIDCOUNT, is zero) 	*/
        volatile uint32_t dbckend    	: 1 ;  		/*!< [Bit 9] 		Data block sent/received (CRC check passed) 	*/
        volatile uint32_t cmdact     	: 1 ;  		/*!< [Bit 10] 		Command transfer in progress 					*/
        volatile uint32_t txact      	: 1 ;  		/*!< [Bit 12] 		Data transmit in progress 						*/
        volatile uint32_t rxact      	: 1 ;  		/*!< [Bit 13] 		Data receive in progress 						*/
        volatile uint32_t txfifohe   	: 1 ;  		/*!< [Bit 14] 		Transmit FIFO half empty: at least 8 words can be written into the FIFO */
        volatile uint32_t rxfifohf   	: 1 ;  		/*!< [Bit 15] 		Receive FIFO half full: there are at least 8 words in the FIFO */
        volatile uint32_t txfifof   	: 1 ;   	/*!< [Bit 16] 		Transmit FIFO full 								*/
        volatile uint32_t rxfifof   	: 1 ;   	/*!< [Bit 17] 		Receive FIFO full 								*/
        volatile uint32_t txfifoe   	: 1 ;   	/*!< [Bit 18] 		Transmit FIFO empty 							*/
        volatile uint32_t rxfifoe   	: 1 ;   	/*!< [Bit 19] 		Receive FIFO empty 								*/
        volatile uint32_t txdavl    	: 1 ;   	/*!< [Bit 20] 		Data available in transmit FIFO 				*/
        volatile uint32_t rxdavl    	: 1 ;   	/*!< [Bit 21] 		Data available in receive FIFO 					*/
        volatile uint32_t sdioit    	: 1 ;   	/*!< [Bit 22] 		SDIO interrupt received 						*/
        volatile uint32_t ceataend  	: 1 ;   	/*!< [Bit 23] 		CE-ATA command completion signal received for CMD61 */
        		  uint32_t reserved     : 8 ;    	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    }bit;
}SDIO_STA_Reg_t;


typedef union {
    uint32_t reg;
    struct{
        volatile uint32_t ccrcfailc   	:  1 ;  	/*!< [Bit 0] 		CCRCFAIL flag clear bit 						*/
        volatile uint32_t dcrcfailc   	:  1 ;  	/*!< [Bit 1] 		DCRCFAIL flag clear bit 						*/
        volatile uint32_t ctimeoutc   	:  1 ;  	/*!< [Bit 2] 		CTIMEOUT flag clear bit 						*/
        volatile uint32_t dtimeoutc   	:  1 ;  	/*!< [Bit 3] 		DTIMEOUT flag clear bit 						*/
        volatile uint32_t txunderrc   	:  1 ;  	/*!< [Bit 4] 		TXUNDERR flag clear bit 						*/
        volatile uint32_t rxoverrc    	:  1 ;  	/*!< [Bit 5] 		RXOVERR flag clear bit 							*/
        volatile uint32_t cmdrendc    	:  1 ;  	/*!< [Bit 6] 		CMDREND flag clear bit 							*/
        volatile uint32_t cmdsentc    	:  1 ;  	/*!< [Bit 7] 		CMDSENT flag clear bit 							*/
        volatile uint32_t dataendc    	:  1 ;  	/*!< [Bit 8] 		DATAEND flag clear bit 							*/
        volatile uint32_t dbckendc    	:  1 ;  	/*!< [Bit 10] 		DBCKEND flag clear bit 							*/
        		  uint32_t reserved_0 	: 11 ;    	/*!< [Bits 21:11] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t sdioitc     	:  1 ;  	/*!< [Bit 22] 		SDIOIT flag clear bit 							*/
        volatile uint32_t ceataendc   	:  1 ;  	/*!< [Bit 23] 		CEATAEND flag clear bit 						*/
        		  uint32_t reserved_1   :  8 ;    	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    }bit;
}SDIO_ICR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ccrcfailie  	: 1 ;  	 	/*!< [Bit 0] 		CCRCFAILIE interrupt enable 					*/
        volatile uint32_t dcrcfailie   	: 1 ;    	/*!< [Bit 1] 		DCRCFAILIE interrupt enable 					*/
        volatile uint32_t ctimeoutie   	: 1 ;    	/*!< [Bit 2] 		CTIMEOUTIE interrupt enable 					*/
        volatile uint32_t dtimeoutie   	: 1 ;    	/*!< [Bit 3] 		DTIMEOUTIE interrupt enable 					*/
        volatile uint32_t txunderrie   	: 1 ;    	/*!< [Bit 4] 		TXUNDERRIE interrupt enable 					*/
        volatile uint32_t rxoverrie    	: 1 ;    	/*!< [Bit 5] 		RXOVERRIE interrupt enable 						*/
        volatile uint32_t cmdrendie    	: 1 ;    	/*!< [Bit 6] 		CMDRENDIE interrupt enable 						*/
        volatile uint32_t cmdsentie    	: 1 ;    	/*!< [Bit 7] 		CMDSENTIE interrupt enable 						*/
        volatile uint32_t dataendie    	: 1 ;    	/*!< [Bit 8] 		DATAENDIE interrupt enable 						*/
        volatile uint32_t dbckendie    	: 1 ;    	/*!< [Bit 10] 		DBCKENDIE interrupt enable 						*/
        volatile uint32_t cmdactie     	: 1 ;    	/*!< [Bit 11] 		CMDACTIE interrupt enable 						*/
        volatile uint32_t txactie      	: 1 ;    	/*!< [Bit 12] 		TXACTIE interrupt enable 						*/
        volatile uint32_t rxactie      	: 1 ;    	/*!< [Bit 13] 		RXACTIE interrupt enable 						*/
        volatile uint32_t txfifohfie   	: 1 ;    	/*!< [Bit 14] 		TXFIFOHFIE interrupt enable 					*/
        volatile uint32_t rxfifohfie   	: 1 ;    	/*!< [Bit 15] 		RXFIFOHFIE interrupt enable 					*/
        volatile uint32_t txfifofie    	: 1 ;    	/*!< [Bit 16] 		TXFIFOFIE interrupt enable 						*/
        volatile uint32_t rxfifofie    	: 1 ;    	/*!< [Bit 17] 		RXFIFOFIE interrupt enable 						*/
        volatile uint32_t txfifoeie    	: 1 ;    	/*!< [Bit 18] 		TXFIFOEIE interrupt enable 						*/
        volatile uint32_t rxfifoeie    	: 1 ;    	/*!< [Bit 19] 		RXFIFOEIE interrupt enable 						*/
        volatile uint32_t txdavlie     	: 1 ;    	/*!< [Bit 20] 		TXDAVLIE interrupt enable 						*/
        volatile uint32_t rxdavlie     	: 1 ;    	/*!< [Bit 21] 		RXDAVLIE interrupt enable 						*/
        volatile uint32_t sdioitie     	: 1 ;    	/*!< [Bit 22] 		SDIOITIE interrupt enable 						*/
        volatile uint32_t ceataendie   	: 1 ;    	/*!< [Bit 23] 		CEATAENDIE interrupt enable 					*/
        		  uint32_t reserved    	: 8 ;    	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_MASK_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t fifocount   	: 24 ; 		/*!< [Bits 23:0] 	Remaining number of words to be written to or read from the FIFO.*/
        		  uint32_t reserved  	:  8 ;  	/*!< [Bits 31:24] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}SDIO_FIFOCNT_Reg_t;

typedef struct
{
SDIO_POWER_Reg_t 			POWER;                 	/*!< SDIO power control register,    Address offset: 0x00 */
SDIO_CLKCR_Reg_t 			CLKCR;                 	/*!< SDI clock control register,     Address offset: 0x04 */
volatile uint32_t 			ARG;                   	/*!< SDIO argument register,         Address offset: 0x08 */
SDIO_CMD_Reg_t 				CMD;                   	/*!< SDIO command register,          Address offset: 0x0C */
SDIO_RESPCMD_Reg_t  		RESPCMD;        	 	/*!< SDIO command response register, Address offset: 0x10 */
volatile const uint32_t 	RESP1;          		/*!< SDIO response 1 register,       Address offset: 0x14 */
volatile const uint32_t  	RESP2;          		/*!< SDIO response 2 register,       Address offset: 0x18 */
volatile const uint32_t  	RESP3;          		/*!< SDIO response 3 register,       Address offset: 0x1C */
volatile const uint32_t  	RESP4;          		/*!< SDIO response 4 register,       Address offset: 0x20 */
volatile uint32_t 			DTIMER;                	/*!< SDIO data timer register,       Address offset: 0x24 */
SDIO_DLEN_Reg_t 			DLEN;                  	/*!< SDIO data length register,      Address offset: 0x28 */
SDIO_DCTRL_Reg_t 			DCTRL;                 	/*!< SDIO data control register,     Address offset: 0x2C */
SDIO_DCOUNT_Reg_t  			DCOUNT;         		/*!< SDIO data counter register,     Address offset: 0x30 */
SDIO_STA_Reg_t  			STA;            		/*!< SDIO status register,           Address offset: 0x34 */
SDIO_ICR_Reg_t 				ICR;                   	/*!< SDIO interrupt clear register,  Address offset: 0x38 */
SDIO_MASK_Reg_t 			MASK;                  	/*!< SDIO mask register,             Address offset: 0x3C */
  uint32_t      			RESERVED0[2];          	/*!< Reserved, 0x40-0x44                                  */
SDIO_FIFOCNT_Reg_t  		FIFOCNT;        		/*!< SDIO FIFO counter register,     Address offset: 0x48 */
  uint32_t      			RESERVED1[13];         	/*!< Reserved, 0x4C-0x7C                                  */
volatile uint32_t 			FIFO;                  	/*!< SDIO data FIFO register,        Address offset: 0x80 */
}SDIO_RegDef_t;

/**
  * @brief Serial Peripheral Interface
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cpha        	: 1 ;  		/*!< [Bit 0] 		Clock phase 									*/
        volatile uint32_t cpol    		: 1 ;  		/*!< [Bit 1] 		Clock polarity 									*/
        volatile uint32_t mstr  		: 1 ;  		/*!< [Bit 2] 		Master selection 								*/
        volatile uint32_t br      		: 3 ;  		/*!< [Bits 5:3] 		Baud rate control 								*/
        volatile uint32_t spe       	: 1 ;  		/*!< [Bit 6] 		SPI enable 										*/
        volatile uint32_t lsbfirst 		: 1 ;  		/*!< [Bit 7] 		Frame format 									*/
        volatile uint32_t ssi        	: 1 ;  		/*!< [Bit 8] 		Internal slave select 							*/
        volatile uint32_t ssm        	: 1 ;  		/*!< [Bit 9] 		Software slave management 						*/
        volatile uint32_t rxonly     	: 1 ;  		/*!< [Bit 10] 		Receive only 									*/
        volatile uint32_t dff        	: 1 ; 		/*!< [Bit 11] 		Data frame format 								*/
        volatile uint32_t crcnext    	: 1 ;  		/*!< [Bit 12] 		CRC transfer next 								*/
        volatile uint32_t crcen      	: 1 ;  		/*!< [Bit 13] 		Hardware CRC calculation enable 				*/
        volatile uint32_t bidioe   		: 1 ;  		/*!< [Bit 14] 		Output enable in bidirectional mode 			*/
        volatile uint32_t bidimode   	: 1 ;  		/*!< [Bit 15] 		Bidirectional data mode enable 					*/
    	   }bit;
}SPI_CR1_Reg_t;

typedef union{
	uint32_t reg;
    struct{
        volatile uint32_t rxdmaen    	: 1 ;  		/*!< [Bit 0] 		Rx buffer DMA enable 							*/
        volatile uint32_t txdmaen   	: 1 ;  		/*!< [Bit 1] 		Tx buffer DMA enable 							*/
        volatile uint32_t ssoe   		: 1 ;  		/*!< [Bit 2] 		SS output enable 								*/
        		  uint32_t reserved_0	: 1 ;  		/*!< [Bit 3] 		Reserved 										*/
        volatile uint32_t frf    		: 1 ;  		/*!< [Bit 4] 		Frame format 									*/
        volatile uint32_t errie     	: 1 ;  		/*!< [Bit 5] 		Error interrupt enable 							*/
        volatile uint32_t rxneie      	: 1 ;  		/*!< [Bit 6] 		RX buffer not empty interrupt enable 			*/
        volatile uint32_t txeie        	: 1 ;  		/*!< [Bit 7] 		Tx buffer empty interrupt enable 				*/
        		  uint32_t reserved_1	: 8 ;  		/*!< [Bits 15:8] 	Reserved 										*/
    	   }bit;
}SPI_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t rxne     		: 1 ;  		/*!< [Bit 0] 		Receive buffer not empty 						*/
        volatile uint32_t txe      		: 1 ;  		/*!< [Bit 1] 		Transmit buffer empty 							*/
        volatile uint32_t chside   		: 1 ;  		/*!< [Bit 2] 		Channel side 									*/
        volatile uint32_t udr      		: 1 ;  		/*!< [Bit 3] 		Underrun flag (not used in SPI mode) 			*/
        volatile uint32_t crcerr  		: 1 ;  		/*!< [Bit 4] 		CRC error flag 									*/
        volatile uint32_t modf     		: 1 ;  		/*!< [Bit 5] 		Mode fault (not used in I2S mode) 				*/
        volatile uint32_t ovr     		: 1 ;  		/*!< [Bit 6] 		Overrun flag 									*/
        volatile uint32_t bsy      		: 1 ;  		/*!< [Bit 7] 		Busy flag 										*/
        volatile uint32_t fre   		: 1 ;  		/*!< [Bit 8] 		Frame format error (used in TI/I2S slave mode) 	*/
        		  uint32_t reserved		: 7 ;  		/*!< [Bits 15:9] 	Reserved 										*/
    	   }bit;
}SPI_SR_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t chlen    		: 1 ;  		/*!< [Bit 0] 		Channel length (number of bits per audio channel)*/
        volatile uint32_t datlen   		: 2 ;  		/*!< [Bits 2:1] 	Data length to be transferred 					*/
        volatile uint32_t ckpol    		: 1 ;  		/*!< [Bit 3] 		Steady state clock polarity (not used in SPI mode) */
        volatile uint32_t i2sstd    	: 2 ;  		/*!< [Bits 5:4] 	I2S standard selection (not used in SPI mode) 	*/
        		  uint32_t reserved_0 	: 1 ;  		/*!< [Bit 6] 		Reserved: forced at 0 by hardware 				*/
        volatile uint32_t pcmsync  		: 1 ;  		/*!< [Bit 7] 		PCM frame synchronization (not used in SPI mode) */
        volatile uint32_t i2scfg   		: 2 ;  		/*!< [Bits 9:8] 	I2S configuration mode 							*/
        volatile uint32_t i2se       	: 1 ;  		/*!< [Bit 10] 		I2S Enable (not used in SPI mode) 				*/
        volatile uint32_t i2smod    	: 1 ;  		/*!< [Bit 11] 		I2S mode selection 								*/
        		  uint32_t reserved_1 	: 4 ;  		/*!< [Bits 15:12] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}SPI_I2SCFGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t i2sdiv   		: 8 ;  		/*!< [Bits 7:0] 	I2S Linear prescaler 							*/
        volatile uint32_t odd     		: 1 ;  		/*!< [Bit 8] 		Odd factor for the prescaler (not used in SPI mode) */
        volatile uint32_t mckoe    		: 1 ;  		/*!< [Bit 9] 		Master clock output enable (not used in SPI mode) */
        		  uint32_t reserved		: 6 ;  		/*!< [Bits 15:10] 	Reserved 										*/
    	   }bit;
}SPI_I2SPR_Reg_t;

typedef struct
{
SPI_CR1_Reg_t 		CR1;        /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
SPI_CR2_Reg_t 		CR2;        /*!< SPI control register 2,                             Address offset: 0x04 */
SPI_SR_Reg_t 		SR;         /*!< SPI status register,                                Address offset: 0x08 */
volatile uint32_t 	DR;         /*!< SPI data register,                                  Address offset: 0x0C */
volatile uint32_t 	CRCPR;      /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
volatile uint32_t 	RXCRCR;     /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
volatile uint32_t 	TXCRCR;     /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
SPI_I2SCFGR_Reg_t 	I2SCFGR;    /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
SPI_I2SPR_Reg_t 	I2SPR;      /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
}SPI_RegDef_t;

/**
  * @brief TIM1 and TIM8
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cen       	: 1 ;  		/*!< [Bit 0] 		Counter enable 									*/
        volatile uint32_t udis      	: 1 ;  		/*!< [Bit 1] 		Update disable 									*/
        volatile uint32_t urs       	: 1 ; 	 	/*!< [Bit 2] 		Update request source 							*/
        volatile uint32_t opm       	: 1 ;  		/*!< [Bit 3] 		One pulse mode 									*/
        volatile uint32_t dir       	: 1 ;  		/*!< [Bit 4] 		Direction 										*/
        volatile uint32_t cms       	: 2 ;  		/*!< [Bits 6:5] 	Center-aligned mode selection 					*/
        volatile uint32_t arpe      	: 1 ;  		/*!< [Bit 7] 		Auto-reload preload enable 						*/
        volatile uint32_t ckd       	: 2 ;  		/*!< [Bits 9:8] 	Clock division 									*/
        		  uint32_t reserved		: 6 ;  		/*!< [Bits 15:10] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_1_8_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ccpc       	: 1 ;  		/*!< [Bit 0] 		Capture/compare preloaded control 				*/
        		  uint32_t reserved_0	: 1 ;  		/*!< [Bit 1] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t ccus      	: 1 ;  		/*!< [Bit 2] 		Capture/compare control update selection 		*/
        volatile uint32_t ccds      	: 1 ;  		/*!< [Bit 3] 		Capture/compare DMA selection 					*/
        volatile uint32_t mms       	: 3 ;  		/*!< [Bits 6:4] 	Master mode selection 							*/
        volatile uint32_t ti1s      	: 1 ;  		/*!< [Bit 7] 		TI1 selection 									*/
        volatile uint32_t ois1      	: 1 ;  		/*!< [Bit 8] 		Output Idle state 1 (OC1 output) 				*/
        volatile uint32_t ois1n     	: 1 ;  		/*!< [Bit 9] 		Output Idle state 1 (OC1N output) 				*/
        volatile uint32_t ois2      	: 1 ;  		/*!< [Bit 10] 		Output Idle state 2 (OC2 output) 				*/
        volatile uint32_t ois2n     	: 1 ;  		/*!< [Bit 11] 		Output Idle state 2 (OC2N output) 				*/
        volatile uint32_t ois3      	: 1 ;  		/*!< [Bit 12] 		Output Idle state 3 (OC3 output) 				*/
        volatile uint32_t ois3n     	: 1 ;  		/*!< [Bit 13] 		Output Idle state 3 (OC3N output) 				*/
        volatile uint32_t ois4      	: 1 ;  		/*!< [Bit 14] 		Output Idle state 4 (OC4 output) 				*/
        		  uint32_t reserved_1	: 1 ;  		/*!< [Bit 15] 		Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_1_8_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sms       	: 3 ;  		/*!< [Bits 2:0] 	Slave mode selection 							*/
        		  uint32_t reserved		: 1 ;  		/*!< [Bit 3] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t ts        	: 3 ;  		/*!< [Bits 6:4] 	Trigger selection 								*/
        volatile uint32_t msm       	: 1 ;  		/*!< [Bit 7] 		Master/slave mode 								*/
        volatile uint32_t etf       	: 4 ;  		/*!< [Bits 11:8] 	External trigger filter 						*/
        volatile uint32_t etps      	: 2 ;  		/*!< [Bits 13:12] 	External trigger prescaler 						*/
        volatile uint32_t ece       	: 1 ;  		/*!< [Bit 14] 		External clock enable 							*/
        volatile uint32_t etp       	: 1 ;  		/*!< [Bit 15] 		External trigger polarity 						*/
    }bit;
}TIM_1_8_SMCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uie       	: 1 ;  		/*!< [Bit 0] 		Update interrupt enable 						*/
        volatile uint32_t cc1ie     	: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt enable 				*/
        volatile uint32_t cc2ie     	: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 interrupt enable 				*/
        volatile uint32_t cc3ie     	: 1 ;  		/*!< [Bit 3] 		Capture/Compare 3 interrupt enable 				*/
        volatile uint32_t cc4ie     	: 1 ;  		/*!< [Bit 4] 		Capture/Compare 4 interrupt enable 				*/
        volatile uint32_t comie    		: 1 ;  		/*!< [Bit 5] 		COM interrupt enable 							*/
        volatile uint32_t tie       	: 1 ;  		/*!< [Bit 6] 		Trigger interrupt enable 						*/
        volatile uint32_t bie       	: 1 ;  		/*!< [Bit 7] 		Break interrupt enable 							*/
        volatile uint32_t ude      		: 1 ;  		/*!< [Bit 8] 		Update DMA request enable 						*/
        volatile uint32_t cc1de    		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 1 DMA request enable 			*/
        volatile uint32_t cc2de    		: 1 ; 	 	/*!< [Bit 10] 		Capture/Compare 2 DMA request enable 			*/
        volatile uint32_t cc3de    		: 1 ;  		/*!< [Bit 11] 		Capture/Compare 3 DMA request enable 			*/
        volatile uint32_t cc4de    		: 1 ;  		/*!< [Bit 12] 		Capture/Compare 4 DMA request enable 			*/
        volatile uint32_t comde  		: 1 ;  		/*!< [Bit 13] 		COM DMA request enable 							*/
        volatile uint32_t tde       	: 1 ;  		/*!< [Bit 14] 		Trigger DMA request enable 						*/
        		  uint32_t reserved     : 1 ;  		/*!< [Bit 15] 		Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_1_8_DIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uif     		: 1 ;  		/*!< [Bit 0] 		Update interrupt flag 							*/
        volatile uint32_t cc1if   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt flag 				*/
        volatile uint32_t cc2if   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 interrupt flag 				*/
        volatile uint32_t cc3if   		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 3 interrupt flag 				*/
        volatile uint32_t cc4if   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 4 interrupt flag 				*/
        volatile uint32_t comif  		: 1 ;  		/*!< [Bit 5] 		COM interrupt flag 								*/
        volatile uint32_t tif      		: 1 ;  		/*!< [Bit 6] 		Trigger interrupt flag 							*/
        volatile uint32_t bif      		: 1 ;  		/*!< [Bit 7] 		Break interrupt flag 							*/
        		  uint32_t reserved_0	: 1 ;  		/*!< [Bit 8] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1of  		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 1 overcapture flag 				*/
        volatile uint32_t cc2of  		: 1 ;  		/*!< [Bit 10] 		Capture/Compare 2 overcapture flag 				*/
        volatile uint32_t cc3of  		: 1 ;  		/*!< [Bit 11] 		Capture/Compare 3 overcapture flag 				*/
        volatile uint32_t cc4of  		: 1 ;  		/*!< [Bit 12] 		Capture/Compare 4 overcapture flag 				*/
        		  uint32_t reserved_1	: 3 ;  		/*!< [Bits 15:13] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_1_8_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ug     		: 1 ;  		/*!< [Bit 0] 		Update generation 								*/
        volatile uint32_t cc1g   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 generation 					*/
        volatile uint32_t cc2g   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 generation 					*/
        volatile uint32_t cc3g   		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 3 generation 					*/
        volatile uint32_t cc4g   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 4 generation 					*/
        volatile uint32_t comg  		: 1 ;  		/*!< [Bit 5] 		Capture/Compare control update generation 		*/
        volatile uint32_t tg      		: 1 ;  		/*!< [Bit 6] 		Trigger generation 								*/
        volatile uint32_t bg      		: 1 ;  		/*!< [Bit 7] 		Break generation 								*/
        		  uint32_t reserved		: 8 ;  		/*!< [Bits 15:8] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_1_8_EGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc1s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 1 selection 					*/
        volatile uint32_t oc1fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 1 fast enable 					*/
        volatile uint32_t oc1pe 		: 1 ;  		/*!< [Bit 3] 		Output compare 1 preload enable 				*/
        volatile uint32_t oc1m   		: 3 ;  		/*!< [Bits 6:4] 	Output compare 1 mode 							*/
        volatile uint32_t oc1ce 		: 1 ; 	 	/*!< [Bit 7] 		Output compare 1 clear enable 					*/
        volatile uint32_t cc2s   		: 2 ;  		/*!< [Bits 9:8] 	Capture/Compare 2 selection 					*/
        volatile uint32_t oc2fe 		: 1 ;  		/*!< [Bit 10] 		Output compare 2 fast enable 					*/
        volatile uint32_t oc2pe 		: 1 ;  		/*!< [Bit 11] 		Output compare 2 preload enable 				*/
        volatile uint32_t oc2m   		: 3 ;  		/*!< [Bits 14:12] 	Output compare 2 mode 							*/
        volatile uint32_t oc2ce 		: 1 ;  		/*!< [Bit 15] 		Output compare 2 clear enable 					*/
    	   }bit;
}TIM_1_8_CCMR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc3s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 3 selection 					*/
        volatile uint32_t oc3fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 3 fast enable 					*/
        volatile uint32_t oc3pe 		: 1 ; 		/*!< [Bit 3] 		Output compare 3 preload enable 				*/
        volatile uint32_t oc3m   		: 3 ; 		/*!< [Bits 6:4] 	Output compare 3 mode 							*/
        volatile uint32_t oc3ce 		: 1 ;  		/*!< [Bit 7] 		Output compare 3 clear enable 					*/
        volatile uint32_t cc4s   		: 2 ;  		/*!< [Bits 9:8] 	Capture/Compare 4 selection 					*/
        volatile uint32_t oc4fe 		: 1 ;  		/*!< [Bit 10] 		Output compare 4 fast enable 					*/
        volatile uint32_t oc4pe 		: 1 ;  		/*!< [Bit 11] 		Output compare 4 preload enable 				*/
        volatile uint32_t oc4m   		: 3 ;  		/*!< [Bits 14:12] 	Output compare 4 mode 							*/
        volatile uint32_t oc4ce 		: 1 ;  		/*!< [Bit 15] 		Output compare 4 clear enable 					*/
    	   }bit;
}TIM_1_8_CCMR2_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t cc1e   		: 1 ;  		/*!< [Bit 0] 		Capture/Compare 1 output enable 				*/
        volatile uint32_t cc1p   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 output polarity 				*/
        volatile uint32_t cc1ne  		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 1 complementary output enable 	*/
        volatile uint32_t cc1np  		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 1 complementary output polarity */
        volatile uint32_t cc2e   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 2 output enable 				*/
        volatile uint32_t cc2p   		: 1 ;  		/*!< [Bit 5] 		Capture/Compare 2 output polarity 				*/
        volatile uint32_t cc2ne  		: 1 ;  		/*!< [Bit 6] 		Capture/Compare 2 complementary output enable 	*/
        volatile uint32_t cc2np  		: 1 ;  		/*!< [Bit 7] 		Capture/Compare 2 complementary output polarity */
        volatile uint32_t cc3e   		: 1 ;  		/*!< [Bit 8] 		Capture/Compare 3 output enable 				*/
        volatile uint32_t cc3p   		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 3 output polarity 				*/
        volatile uint32_t cc3ne  		: 1 ;  		/*!< [Bit 10] 		Capture/Compare 3 complementary output enable 	*/
        volatile uint32_t cc3np  		: 1 ;  		/*!< [Bit 11] 		Capture/Compare 3 complementary output polarity */
        volatile uint32_t cc4e   		: 1 ;  		/*!< [Bit 12] 		Capture/Compare 4 output enable 				*/
        volatile uint32_t cc4p   		: 1 ;  		/*!< [Bit 13] 		Capture/Compare 4 output polarity 				*/
        		  uint32_t reserved 	: 1 ;  		/*!< [Bit 14] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc4np  		: 1 ;  		/*!< [Bit 15] 		Capture/Compare 4 complementary output polarity */
    	   }bit;
}TIM_1_8_CCER_Reg_t;

typedef union{
	uint32_t reg;
	    struct{
			volatile uint32_t rep		: 8 ;		/*!< [Bits 7:0] 	Repetition counter value 						*/
					  uint32_t reserved : 8 ;		/*!< [Bits 15:8] 	Reserved, must be kept at reset value. 			*/
	       	  }bit;
}TIM_1_8_RCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dtg    		: 8 ;  		/*!< [Bits 7:0] 	Dead-time generator setup 						*/
        volatile uint32_t lock   		: 2 ;  		/*!< [Bits 9:8] 	Lock configuration 								*/
        volatile uint32_t ossi   		: 1 ;  		/*!< [Bit 10] 		Off-state selection for Idle mode 				*/
        volatile uint32_t ossr   		: 1 ;  		/*!< [Bit 11] 		Off-state selection for Run mode 				*/
        volatile uint32_t bke    		: 1 ;  		/*!< [Bit 12] 		Break enable 									*/
        volatile uint32_t bkp    		: 1 ;  		/*!< [Bit 13] 		Break polarity 									*/
        volatile uint32_t aoe    		: 1 ;  		/*!< [Bit 14] 		Automatic output enable 						*/
        volatile uint32_t moe    		: 1 ;  		/*!< [Bit 15] 		Main output enable 								*/
    	   }bit;
}TIM_1_8_BDTR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dba    		: 5 ;  		/*!< [Bits 4:0] 	DMA base address 								*/
        		  uint32_t reserved_0	: 3 ;  		/*!< [Bits 7:5] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t dbl   		: 5 ;  		/*!< [Bits 12:8] 	DMA burst length 								*/
        		  uint32_t reserved_1	: 3 ;  		/*!< [Bits 15:13] 	Reserved, must be kept at reset value 			*/
    }bit;
}TIM_1_8_DCR_Reg_t;

typedef struct
{
TIM_1_8_CR1_Reg_t 	CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
TIM_1_8_CR2_Reg_t 	CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
TIM_1_8_SMCR_Reg_t 	SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
TIM_1_8_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_1_8_SR_Reg_t 	SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_1_8_EGR_Reg_t 	EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
TIM_1_8_CCMR1_Reg_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
TIM_1_8_CCMR2_Reg_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
TIM_1_8_CCER_Reg_t 	CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
volatile uint32_t 	CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 	PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 	ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
TIM_1_8_RCR_Reg_t 	RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
volatile uint32_t 	CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
volatile uint32_t 	CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
volatile uint32_t 	CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
volatile uint32_t 	CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
TIM_1_8_BDTR_Reg_t 	BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
TIM_1_8_DCR_Reg_t 	DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
volatile uint32_t 	DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
}TIM_1_8_Reg_Def_t;

/**
  * @brief TIM2 and TIM5
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cen       	: 1 ;  		/*!< [Bit 0] 		Counter enable 									*/
        volatile uint32_t udis      	: 1 ;  		/*!< [Bit 1] 		Update disable 									*/
        volatile uint32_t urs       	: 1 ; 	 	/*!< [Bit 2] 		Update request source 							*/
        volatile uint32_t opm       	: 1 ;  		/*!< [Bit 3] 		One pulse mode 									*/
        volatile uint32_t dir       	: 1 ;  		/*!< [Bit 4] 		Direction 										*/
        volatile uint32_t cms       	: 2 ;  		/*!< [Bits 6:5] 	Center-aligned mode selection 					*/
        volatile uint32_t arpe      	: 1 ;  		/*!< [Bit 7] 		Auto-reload preload enable 						*/
        volatile uint32_t ckd       	: 2 ;  		/*!< [Bits 9:8] 	Clock division 									*/
        		  uint32_t reserved		: 6 ;  		/*!< [Bits 15:10] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_2_5_CR1_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        		  uint32_t reserved_0	: 3 ;  		/*!< [Bits 2:0] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t ccds         	: 1 ;  		/*!< [Bit 3] 		Capture/compare DMA selection 					*/
        volatile uint32_t mms         	: 3 ;  		/*!< [Bits 6:4] 	Master mode selection 							*/
        volatile uint32_t ti1s       	: 1 ;  		/*!< [Bit 7] 		TI1 selection 									*/
        		  uint32_t reserved_1	: 8 ;  		/*!< [Bits 15:8] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}TIM_2_5_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sms       	: 3 ;  		/*!< [Bits 2:0] 	Slave mode selection 							*/
        		  uint32_t reserved		: 1 ;  		/*!< [Bit 3] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t ts        	: 3 ;  		/*!< [Bits 6:4] 	Trigger selection 								*/
        volatile uint32_t msm       	: 1 ;  		/*!< [Bit 7] 		Master/slave mode 								*/
        volatile uint32_t etf       	: 4 ;  		/*!< [Bits 11:8] 	External trigger filter 						*/
        volatile uint32_t etps      	: 2 ;  		/*!< [Bits 13:12] 	External trigger prescaler 						*/
        volatile uint32_t ece       	: 1 ;  		/*!< [Bit 14] 		External clock enable 							*/
        volatile uint32_t etp       	: 1 ;  		/*!< [Bit 15] 		External trigger polarity 						*/
    }bit;
}TIM_2_5_SMCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uie   		: 1 ;   	/*!< [Bit 0] 		Update interrupt enable 						*/
        volatile uint32_t cc1ie 		: 1 ;   	/*!< [Bit 1] 		Capture/Compare 1 interrupt enable 				*/
        volatile uint32_t cc2ie 		: 1 ;   	/*!< [Bit 2] 		Capture/Compare 2 interrupt enable 				*/
        volatile uint32_t cc3ie 		: 1 ;   	/*!< [Bit 3] 		Capture/Compare 3 interrupt enable 				*/
        volatile uint32_t cc4ie 		: 1 ;   	/*!< [Bit 4] 		Capture/Compare 4 interrupt enable 				*/
        		  uint32_t reserved_0	: 1 ;   	/*!< [Bit 5] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tie   		: 1 ;   	/*!< [Bit 6] 		Trigger interrupt enable 						*/
        		  uint32_t reserved_1	: 1 ;   	/*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ude   		: 1 ;   	/*!< [Bit 8] 		Update DMA request enable 						*/
        volatile uint32_t cc1de 		: 1 ;   	/*!< [Bit 9] 		Capture/Compare 1 DMA request enable 			*/
        volatile uint32_t cc2de 		: 1 ;   	/*!< [Bit 10] 		Capture/Compare 2 DMA request enable 			*/
        volatile uint32_t cc3de 		: 1 ;   	/*!< [Bit 11] 		Capture/Compare 3 DMA request enable 			*/
        volatile uint32_t cc4de 		: 1 ;   	/*!< [Bit 12] 		Capture/Compare 4 DMA request enable 			*/
        		  uint32_t reserved_2	: 1 ;   	/*!< [Bit 13]	 	Reserved, always read as 0 						*/
        volatile uint32_t tde   		: 1 ;   	/*!< [Bit 14] 		Trigger DMA request enable 						*/
        		  uint32_t reserved_3	: 1 ;   	/*!< [Bit 15] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}TIM_2_5_DIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uif     		: 1 ;  		/*!< [Bit 0] 		Update interrupt flag 							*/
        volatile uint32_t cc1if   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt flag 				*/
        volatile uint32_t cc2if   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 interrupt flag 				*/
        volatile uint32_t cc3if   		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 3 interrupt flag 				*/
        volatile uint32_t cc4if   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 4 interrupt flag 				*/
		  	  	  uint32_t reserved_0	: 1 ;   	/*!< [Bit 5] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tif      		: 1 ;  		/*!< [Bit 6] 		Trigger interrupt flag 							*/
        		  uint32_t reserved_1	: 1 ;  		/*!< [Bits 8:7] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1of  		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 1 overcapture flag 				*/
        volatile uint32_t cc2of  		: 1 ;  		/*!< [Bit 10] 		Capture/Compare 2 overcapture flag 				*/
        volatile uint32_t cc3of  		: 1 ;  		/*!< [Bit 11] 		Capture/Compare 3 overcapture flag 				*/
        volatile uint32_t cc4of  		: 1 ;  		/*!< [Bit 12] 		Capture/Compare 4 overcapture flag 				*/
        		  uint32_t reserved_2	: 3 ;  		/*!< [Bits 15:13] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_2_5_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ug     		: 1 ;  		/*!< [Bit 0] 		Update generation 								*/
        volatile uint32_t cc1g   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 generation 					*/
        volatile uint32_t cc2g   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 generation 					*/
        volatile uint32_t cc3g   		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 3 generation 					*/
        volatile uint32_t cc4g   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 4 generation 					*/
	  	  	  	  uint32_t reserved_0	: 1 ;   	/*!< [Bit 5] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t tg      		: 1 ;  		/*!< [Bit 6] 		Trigger generation 								*/
        		  uint32_t reserved_1	: 8 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_2_5_EGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc1s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 1 selection 					*/
        volatile uint32_t oc1fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 1 fast enable 					*/
        volatile uint32_t oc1pe 		: 1 ;  		/*!< [Bit 3] 		Output compare 1 preload enable 				*/
        volatile uint32_t oc1m   		: 3 ;  		/*!< [Bits 6:4] 	Output compare 1 mode 							*/
        volatile uint32_t oc1ce 		: 1 ; 	 	/*!< [Bit 7] 		Output compare 1 clear enable 					*/
        volatile uint32_t cc2s   		: 2 ;  		/*!< [Bits 9:8] 	Capture/Compare 2 selection 					*/
        volatile uint32_t oc2fe 		: 1 ;  		/*!< [Bit 10] 		Output compare 2 fast enable 					*/
        volatile uint32_t oc2pe 		: 1 ;  		/*!< [Bit 11] 		Output compare 2 preload enable 				*/
        volatile uint32_t oc2m   		: 3 ;  		/*!< [Bits 14:12] 	Output compare 2 mode 							*/
        volatile uint32_t oc2ce 		: 1 ;  		/*!< [Bit 15] 		Output compare 2 clear enable 					*/
    	   }bit;
}TIM_2_5_CCMR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc3s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 3 selection 					*/
        volatile uint32_t oc3fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 3 fast enable 					*/
        volatile uint32_t oc3pe 		: 1 ; 		/*!< [Bit 3] 		Output compare 3 preload enable 				*/
        volatile uint32_t oc3m   		: 3 ; 		/*!< [Bits 6:4] 	Output compare 3 mode 							*/
        volatile uint32_t oc3ce 		: 1 ;  		/*!< [Bit 7] 		Output compare 3 clear enable 					*/
        volatile uint32_t cc4s   		: 2 ;  		/*!< [Bits 9:8] 	Capture/Compare 4 selection 					*/
        volatile uint32_t oc4fe 		: 1 ;  		/*!< [Bit 10] 		Output compare 4 fast enable 					*/
        volatile uint32_t oc4pe 		: 1 ;  		/*!< [Bit 11] 		Output compare 4 preload enable 				*/
        volatile uint32_t oc4m   		: 3 ;  		/*!< [Bits 14:12] 	Output compare 4 mode 							*/
        volatile uint32_t oc4ce 		: 1 ;  		/*!< [Bit 15] 		Output compare 4 clear enable 					*/
    	   }bit;
}TIM_2_5_CCMR2_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t cc1e   		: 1 ;  		/*!< [Bit 0] 		Capture/Compare 1 output enable 				*/
        volatile uint32_t cc1p   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 output polarity 				*/
  	  	  	  	  uint32_t reserved_0	: 1 ;   	/*!< [Bit 2] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cc1np  		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 1 complementary output polarity */
        volatile uint32_t cc2e   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 2 output enable 				*/
        volatile uint32_t cc2p   		: 1 ;  		/*!< [Bit 5] 		Capture/Compare 2 output polarity 				*/
	  	  	  	  uint32_t reserved_1	: 1 ;   	/*!< [Bit 6] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cc2np  		: 1 ;  		/*!< [Bit 7] 		Capture/Compare 2 complementary output polarity */
        volatile uint32_t cc3e   		: 1 ;  		/*!< [Bit 8] 		Capture/Compare 3 output enable 				*/
        volatile uint32_t cc3p   		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 3 output polarity 				*/
	  	  	  	  uint32_t reserved_2	: 1 ;   	/*!< [Bit 10] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cc3np  		: 1 ;  		/*!< [Bit 11] 		Capture/Compare 3 complementary output polarity */
        volatile uint32_t cc4e   		: 1 ;  		/*!< [Bit 12] 		Capture/Compare 4 output enable 				*/
        volatile uint32_t cc4p   		: 1 ;  		/*!< [Bit 13] 		Capture/Compare 4 output polarity 				*/
        		  uint32_t reserved_3 	: 1 ;  		/*!< [Bit 14] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc4np  		: 1 ;  		/*!< [Bit 15] 		Capture/Compare 4 complementary output polarity */
    	   }bit;
}TIM_2_5_CCER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dba    		: 5 ;  		/*!< [Bits 4:0] 	DMA base address 								*/
        		  uint32_t reserved_0	: 3 ;  		/*!< [Bits 7:5] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t dbl   		: 5 ;  		/*!< [Bits 12:8] 	DMA burst length 								*/
        		  uint32_t reserved_1	: 3 ;  		/*!< [Bits 15:13] 	Reserved, must be kept at reset value 			*/
    }bit;
}TIM_2_5_DCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
		  	  	  uint32_t reserved_0	: 6 ;  		/*!< [Bits 5:0] 	Reserved, must be kept at reset value 			*/
		volatile uint32_t TIM5_ti4_rmp 	: 2 ;  		/*!< [Bits 7:6] 	Timer Input 4 remap 							*/
				  uint32_t reserved_1	: 2 ;  		/*!< [Bits 9:8] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t TIM2_itr1_rmp	: 2 ;  		/*!< [Bits 11:10] 	Internal trigger 1 remap 						*/
        		  uint32_t reserved_2	: 4 ;  		/*!< [Bits 15:12] 	Reserved, must be kept at reset value 			*/
    }bit;
}TIM_2_5_OR_Reg_t;

typedef struct
{
TIM_2_5_CR1_Reg_t 	CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
TIM_2_5_CR2_Reg_t 	CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
TIM_2_5_SMCR_Reg_t 	SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
TIM_2_5_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_2_5_SR_Reg_t 	SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_2_5_EGR_Reg_t 	EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
TIM_2_5_CCMR1_Reg_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
TIM_2_5_CCMR2_Reg_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
TIM_2_5_CCER_Reg_t 	CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
volatile uint32_t 	CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 	PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 	ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
volatile uint32_t 	RESERVED1;    /*!< Reserved,     					   Address offset: 0x30 */
volatile uint32_t 	CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
volatile uint32_t 	CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
volatile uint32_t 	CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
volatile uint32_t 	CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
volatile uint32_t 	RESERVED2;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
TIM_2_5_DCR_Reg_t 	DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
volatile uint32_t 	DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
TIM_2_5_OR_Reg_t 	OR;          /*!< TIM option register,                 Address offset: 0x50 */
}TIM_2_5_Reg_Def_t;

/**
  * @brief TIM9 and TIM12
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cen       	: 1 ;  		/*!< [Bit 0] 		Counter enable 									*/
        volatile uint32_t udis      	: 1 ;  		/*!< [Bit 1] 		Update disable 									*/
        volatile uint32_t urs       	: 1 ; 	 	/*!< [Bit 2] 		Update request source 							*/
        volatile uint32_t opm       	: 1 ;  		/*!< [Bit 3] 		One pulse mode 									*/
         	 	  uint32_t reserved_0	: 3 ;  		/*!< [Bits 6:4] 	Center-aligned mode selection 					*/
        volatile uint32_t arpe      	: 1 ;  		/*!< [Bit 7] 		Auto-reload preload enable 						*/
        volatile uint32_t ckd       	: 2 ;  		/*!< [Bits 9:8] 	Clock division 									*/
        		  uint32_t reserved_1	: 6 ;  		/*!< [Bits 15:10] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_9_12_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
         	 	  uint32_t reserved_0	: 4 ;  		/*!< [Bits 3:0] 	Center-aligned mode selection 					*/
        volatile uint32_t mms      		: 3 ;  		/*!< [Bits 6:4] 	Master mode selection 							*/
        		  uint32_t reserved_1	: 9 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_9_12_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sms       	: 3 ;  		/*!< [Bits 2:0] 	Slave mode selection 							*/
        		  uint32_t reserved_0	: 1 ;  		/*!< [Bit 3] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t ts        	: 3 ;  		/*!< [Bits 6:4] 	Trigger selection 								*/
        volatile uint32_t msm       	: 1 ;  		/*!< [Bit 7] 		Master/slave mode 								*/
		  	  	  uint32_t reserved_1	: 8 ;  		/*!< [Bits 15:8] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_9_12_SMCR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uie       	: 1 ;  		/*!< [Bit 0] 		Update interrupt enable 						*/
        volatile uint32_t cc1ie     	: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt enable 				*/
        volatile uint32_t cc2ie     	: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 interrupt enable 				*/
		  	  	  uint32_t reserved_0	: 3 ;  		/*!< [Bits 5:3] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t tie       	: 1 ;  		/*!< [Bit 6] 		Trigger interrupt enable 						*/
	  	  	  	  uint32_t reserved_1	: 9 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_9_12_DIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uif     		: 1 ;  		/*!< [Bit 0] 		Update interrupt flag 							*/
        volatile uint32_t cc1if   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt flag 				*/
        volatile uint32_t cc2if   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 interrupt flag 				*/
	  	  	  	  uint32_t reserved_0	: 3 ;  		/*!< [Bits 5:3] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t tif      		: 1 ;  		/*!< [Bit 6] 		Trigger interrupt flag 							*/
        		  uint32_t reserved_1	: 1 ;  		/*!< [Bits 8:7] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1of  		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 1 overcapture flag 				*/
        volatile uint32_t cc2of  		: 1 ;  		/*!< [Bit 10] 		Capture/Compare 2 overcapture flag 				*/
        		  uint32_t reserved_2	: 5 ;  		/*!< [Bits 15:11] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_9_12_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ug     		: 1 ;  		/*!< [Bit 0] 		Update generation 								*/
        volatile uint32_t cc1g   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 generation 					*/
        volatile uint32_t cc2g   		: 1 ;  		/*!< [Bit 2] 		Capture/Compare 2 generation 					*/
  	  	  	  	  uint32_t reserved_0	: 3 ;  		/*!< [Bits 5:3] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t tg      		: 1 ;  		/*!< [Bit 6] 		Trigger generation 								*/
        		  uint32_t reserved_1	: 9 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_9_12_EGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc1s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 1 selection 					*/
        volatile uint32_t oc1fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 1 fast enable 					*/
        volatile uint32_t oc1pe 		: 1 ;  		/*!< [Bit 3] 		Output compare 1 preload enable 				*/
        volatile uint32_t oc1m   		: 3 ;  		/*!< [Bits 6:4] 	Output compare 1 mode 							*/
        		  uint32_t reserved_0 	: 1 ; 	 	/*!< [Bit 7] 		Reserved, must be kept at reset value.			*/
        volatile uint32_t cc2s   		: 2 ;  		/*!< [Bits 9:8] 	Capture/Compare 2 selection 					*/
        volatile uint32_t oc2fe 		: 1 ;  		/*!< [Bit 10] 		Output compare 2 fast enable 					*/
        volatile uint32_t oc2pe 		: 1 ;  		/*!< [Bit 11] 		Output compare 2 preload enable 				*/
        volatile uint32_t oc2m   		: 3 ;  		/*!< [Bits 14:12] 	Output compare 2 mode 							*/
 		  	  	  uint32_t reserved_1 	: 1 ; 	 	/*!< [Bit 15] 		Reserved, must be kept at reset value.			*/
    	   }bit;
}TIM_9_12_CCMR1_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t cc1e   		: 1 ;  		/*!< [Bit 0] 		Capture/Compare 1 output enable 				*/
        volatile uint32_t cc1p   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 output polarity 				*/
		  	  	  uint32_t reserved_0	: 1 ;  		/*!< [Bit 2] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1np  		: 1 ;  		/*!< [Bit 3] 		Capture/Compare 1 complementary output polarity */
        volatile uint32_t cc2e   		: 1 ;  		/*!< [Bit 4] 		Capture/Compare 2 output enable 				*/
        volatile uint32_t cc2p   		: 1 ;  		/*!< [Bit 5] 		Capture/Compare 2 output polarity 				*/
		  	  	  uint32_t reserved_1	: 1 ;  		/*!< [Bit 6] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc2np  		: 1 ;  		/*!< [Bit 7] 		Capture/Compare 2 complementary output polarity */
        		  uint32_t reserved_2 	: 8 ;  		/*!< [Bit 15:8] 		Reserved, must be kept at reset value. 		*/
    	   }bit;
}TIM_9_12_CCER_Reg_t;

typedef struct
{
TIM_9_12_CR1_Reg_t 		CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
TIM_9_12_CR2_Reg_t 		CR2;	 	 /*!< TIM control register 2,              Address offset: 0x04 */
TIM_9_12_SMCR_Reg_t 	SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
TIM_9_12_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_9_12_SR_Reg_t 		SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_9_12_EGR_Reg_t 		EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
TIM_9_12_CCMR1_Reg_t 	CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
volatile uint32_t  		RESERVED2; 	 /*!< Reserved, 						   Address offset: 0x1C */
TIM_9_12_CCER_Reg_t 	CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
volatile uint32_t 		CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 		PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 		ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
volatile uint32_t 		RESERVED3;   /*!< Reserved,     					   Address offset: 0x30 */
volatile uint32_t 		CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
volatile uint32_t 		CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
volatile uint32_t 		RESERVED4;   /*!< Reserved,      					   Address offset: 0x3C */
}TIM_9_12_Reg_Def_t;


/**
  * @brief TIM10 and TIM14
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cen       	: 1 ;  		/*!< [Bit 0] 		Counter enable 									*/
        volatile uint32_t udis      	: 1 ;  		/*!< [Bit 1] 		Update disable 									*/
        volatile uint32_t urs       	: 1 ; 	 	/*!< [Bit 2] 		Update request source 							*/
        volatile uint32_t opm       	: 1 ;  		/*!< [Bit 3] 		One pulse mode 									*/
        		  uint32_t reserved_0   : 3 ;  		/*!< [Bits 6:4] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t arpe      	: 1 ;  		/*!< [Bit 7] 		Auto-reload preload enable 						*/
        volatile uint32_t ckd       	: 2 ;  		/*!< [Bits 9:8] 	Clock division 									*/
        		  uint32_t reserved_1	: 6 ;  		/*!< [Bits 15:10] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_10_14_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uie       	:  1 ;  	/*!< [Bit 0] 		Update interrupt enable 						*/
        volatile uint32_t cc1ie     	:  1 ;  	/*!< [Bit 1] 		Capture/Compare 1 interrupt enable 				*/
        		  uint32_t reserved     : 14 ;  	/*!< [Bits 15:2] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_10_14_DIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uif     		: 1 ;  		/*!< [Bit 0] 		Update interrupt flag 							*/
        volatile uint32_t cc1if   		: 1 ;  		/*!< [Bit 1] 		Capture/Compare 1 interrupt flag 				*/
        		  uint32_t reserved_0	: 7 ;  		/*!< [Bits 8:2] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1of  		: 1 ;  		/*!< [Bit 9] 		Capture/Compare 1 overcapture flag 				*/
        		  uint32_t reserved_1	: 6 ;  		/*!< [Bits 15:10] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_10_14_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ug     		:  1 ;  	/*!< [Bit 0] 		Update generation 								*/
        volatile uint32_t cc1g   		:  1 ;  	/*!< [Bit 1] 		Capture/Compare 1 generation 					*/
        		  uint32_t reserved		: 14 ;  	/*!< [Bits 15:2] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_10_14_EGR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc1s   		: 2 ;  		/*!< [Bits 1:0] 	Capture/Compare 1 selection 					*/
        volatile uint32_t oc1fe 		: 1 ;  		/*!< [Bit 2] 		Output compare 1 fast enable 					*/
        volatile uint32_t oc1pe 		: 1 ;  		/*!< [Bit 3] 		Output compare 1 preload enable 				*/
        volatile uint32_t oc1m   		: 3 ;  		/*!< [Bits 6:4] 	Output compare 1 mode 							*/
 		  	  	  uint32_t reserved		: 9 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_10_14_CCMR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cc1e   		:  1 ;  	/*!< [Bit 0] 		Capture/Compare 1 output enable 				*/
        volatile uint32_t cc1p   		:  1 ;  	/*!< [Bit 1] 		Capture/Compare 1 output polarity 				*/
		  	  	  uint32_t reserved_0	:  1 ;  	/*!< [Bit 2] 		Reserved, must be kept at reset value. 			*/
        volatile uint32_t cc1np  		:  1 ;  	/*!< [Bit 3] 		Capture/Compare 1 complementary output polarity */
        		  uint32_t reserved_1 	: 12 ;  	/*!< [Bits 15:4] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_10_14_CCER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ti1_rmp	:  2 ;  		/*!< [Bits 1:0] 	TIM11 Input 1 remapping capability				*/
        		  uint32_t reserved		: 14 ;  	/*!< [Bits 15:13] 	Reserved, must be kept at reset value 			*/
    }bit;
}TIM_11_OR_Reg_t;

typedef struct
{
TIM_10_14_CR1_Reg_t 	CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
volatile uint32_t 		RESERVED1[2];/*!< Reserved,              			   Address offset: 0x04 */
TIM_10_14_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_10_14_SR_Reg_t 		SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_10_14_EGR_Reg_t 	EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
TIM_10_14_CCMR1_Reg_t 	CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
volatile uint32_t 		RESERVED2;   /*!< Reserved, 						   Address offset: 0x1C */
TIM_10_14_CCER_Reg_t 	CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
volatile uint32_t 		CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 		PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 		ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
volatile uint32_t 		RESERVED3;   /*!< Reserved,    	 					   Address offset: 0x30 */
volatile uint32_t 		CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
}TIM_10_14_Reg_Def_t;

typedef struct
{
TIM_10_14_CR1_Reg_t 	CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
volatile uint32_t 		RESERVED1[2];/*!< Reserved,              			   Address offset: 0x04 */
TIM_10_14_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_10_14_SR_Reg_t 		SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_10_14_EGR_Reg_t 	EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
TIM_10_14_CCMR1_Reg_t 	CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
volatile uint32_t 		RESERVED2;   /*!< Reserved, 						   Address offset: 0x1C */
TIM_10_14_CCER_Reg_t 	CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
volatile uint32_t 		CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 		PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 		ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
volatile uint32_t 		RESERVED3;   /*!< Reserved,    	 					   Address offset: 0x30 */
volatile uint32_t 		CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
volatile uint32_t 		RESERVED4[6];/*!< Reserved,    	 					   Address offset: 0x38 */
TIM_11_OR_Reg_t 		OR;			 /*!< TIM option register,                 Address offset: 0x50 */
}TIM_11_Reg_Def_t;

/**
  * @brief TIM6 and TIM7
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cen       	: 1 ;  		/*!< [Bit 0] 		Counter enable 									*/
        volatile uint32_t udis      	: 1 ;  		/*!< [Bit 1] 		Update disable 									*/
        volatile uint32_t urs       	: 1 ; 	 	/*!< [Bit 2] 		Update request source 							*/
        volatile uint32_t opm       	: 1 ;  		/*!< [Bit 3] 		One pulse mode 									*/
        		  uint32_t reserved_0 	: 3 ;  		/*!< [Bits 6:4] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t arpe      	: 1 ;  		/*!< [Bit 7] 		Auto-reload preload enable 						*/
        		  uint32_t reserved_1	: 8 ;  		/*!< [Bits 15:8] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_6_7_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	: 4 ;  		/*!< [Bits 3:0] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t mms       	: 3 ;  		/*!< [Bits 6:4] 	Master mode selection 							*/
        		  uint32_t reserved_1	: 9 ;  		/*!< [Bits 15:7] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_6_7_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uie       	: 1 ;  		/*!< [Bit 0] 		Update interrupt enable 						*/
        		  uint32_t reserved_0   : 7 ;  		/*!< [Bits 7:1] 	Reserved, must be kept at reset value. 			*/
        volatile uint32_t ude      		: 1 ;  		/*!< [Bit 8] 		Update DMA request enable 						*/
        		  uint32_t reserved_1	: 7 ;  		/*!< [Bits 15:9] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_6_7_DIER_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t uif     		:  1 ;  	/*!< [Bit 0] 		Update interrupt flag 							*/
        		  uint32_t reserved		: 15 ;  	/*!< [Bits 15:1] 	Reserved, must be kept at reset value. 			*/
    }bit;
}TIM_6_7_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ug     		:  1 ;  	/*!< [Bit 0] 		Update generation 								*/
        		  uint32_t reserved		: 15 ;  	/*!< [Bits 15:1] 	Reserved, must be kept at reset value. 			*/
    	   }bit;
}TIM_6_7_EGR_Reg_t;

typedef struct
{
TIM_6_7_CR1_Reg_t 	CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
TIM_6_7_CR2_Reg_t 	CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
volatile uint32_t 	RESERVED1;   /*!< Reserved,     					   Address offset: 0x08 */
TIM_6_7_DIER_Reg_t 	DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
TIM_6_7_SR_Reg_t 	SR;          /*!< TIM status register,                 Address offset: 0x10 */
TIM_6_7_EGR_Reg_t 	EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
volatile uint32_t 	RESERVED[3]; /*!< Reserved,     					   Address offset: 0x18 */
volatile uint32_t 	CNT;         /*!< TIM counter register,                Address offset: 0x24 */
volatile uint32_t 	PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
volatile uint32_t 	ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
}TIM_6_7_Reg_Def_t;

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t pe   			:  1 ;   	/*!< [Bit 0] 		Parity error 									*/
        volatile uint32_t fe   			:  1 ;   	/*!< [Bit 1] 		Framing error 									*/
        volatile uint32_t nf   			:  1 ;   	/*!< [Bit 2] 		Noise detected flag 							*/
        volatile uint32_t ore  			:  1 ;   	/*!< [Bit 3] 		Overrun error 									*/
        volatile uint32_t idle 			:  1 ;   	/*!< [Bit 4] 		IDLE line detected 								*/
        volatile uint32_t rxne 			:  1 ;   	/*!< [Bit 5] 		Read data register not empty 					*/
        volatile uint32_t tc   			:  1 ;   	/*!< [Bit 6] 		Transmission complete 							*/
        volatile uint32_t txe  			:  1 ;   	/*!< [Bit 7] 		Transmit data register empty 					*/
        volatile uint32_t lbd  			:  1 ;   	/*!< [Bit 8] 		LIN break detection flag 						*/
        volatile uint32_t cts  			:  1 ;   	/*!< [Bit 9] 		CTS flag 										*/
        		  uint32_t reserved		: 22 ;  	/*!< [Bits 31:10] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_SR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t dr        	:  9 ;   	/*!< [Bits 8:0]	 	Data value 										*/
        		  uint32_t reserved		: 23 ;  	/*!< [Bits 31:9] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_DR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t div_fraction	:  4 ;    	/*!< [Bits 3:0] 	Fraction of USARTDIV 							*/
        volatile uint32_t div_mantissa	: 12 ;   	/*!< [Bits 14:4] 	Mantissa of USARTDIV 							*/
        		  uint32_t reserved		: 16 ;    	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_BRR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t sbk      		:  1 ;    	/*!< [Bit 0] 		Send break 										*/
        volatile uint32_t rwu      		:  1 ;    	/*!< [Bit 1] 		Receiver wakeup 								*/
        volatile uint32_t re       		:  1 ;    	/*!< [Bit 2] 		Receiver enable 								*/
        volatile uint32_t te       		:  1 ;    	/*!< [Bit 3] 		Transmitter enable 								*/
        volatile uint32_t idleie   		:  1 ;    	/*!< [Bit 4] 		IDLE interrupt enable 							*/
        volatile uint32_t rxneie  		:  1 ;    	/*!< [Bit 5] 		RXNE interrupt enable 							*/
        volatile uint32_t tcie    		:  1 ;    	/*!< [Bit 6] 		Transmission complete interrupt enable 			*/
        volatile uint32_t txeie   		:  1 ;    	/*!< [Bit 7] 		TXE interrupt enable 							*/
        volatile uint32_t peie    		:  1 ;    	/*!< [Bit 8] 		PE interrupt enable 							*/
        volatile uint32_t ps      		:  1 ;    	/*!< [Bit 9] 		Parity selection 								*/
        volatile uint32_t pce     		:  1 ;    	/*!< [Bit 10] 		Parity control enable 							*/
        volatile uint32_t wake    		:  1 ;    	/*!< [Bit 11] 		Wakeup method 									*/
        volatile uint32_t m       		:  1 ;    	/*!< [Bit 12] 		Word length 									*/
        volatile uint32_t ue      		:  1 ;    	/*!< [Bit 13] 		USART enable 									*/
        		  uint32_t reserved_0	:  1 ;    	/*!< [Bit 14] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t over8   		:  1 ;    	/*!< [Bit 15] 		Oversampling mode 								*/
        		  uint32_t reserved_1	: 16 ;   	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_CR1_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t add      		:  4 ;    	/*!< [Bits 3:0] 	Address of the USART node 						*/
        		  uint32_t reserved_0	:  1 ;    	/*!< [Bit 4] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t lbdl     		:  1 ;    	/*!< [Bit 5] 		LIN break detection length 						*/
        volatile uint32_t lbdie    		:  1 ;    	/*!< [Bit 6] 		LIN break detection interrupt enable 			*/
        		  uint32_t reserved_1	:  1 ;    	/*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t lbcl  		:  1 ;    	/*!< [Bit 8] 		Last bit clock pulse 							*/
        volatile uint32_t cpha     		:  1 ;    	/*!< [Bit 9] 		Clock phase 									*/
        volatile uint32_t cpol     		:  1 ;    	/*!< [Bit 10] 		Clock polarity 									*/
        volatile uint32_t clken     	:  1 ;    	/*!< [Bit 11] 		Clock enable 									*/
        volatile uint32_t stop     		:  2 ;    	/*!< [Bits 13:12] 	STOP bits 										*/
        volatile uint32_t linen    		:  1 ;    	/*!< [Bit 14] 		LIN mode enable 								*/
        		  uint32_t reserved_2	: 17 ;   	/*!< [Bits 31:15] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_CR2_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t eie      		:  1 ;    	/*!< [Bit 0] 		Error interrupt enable 							*/
        volatile uint32_t iren     		:  1 ;    	/*!< [Bit 1] 		IrDA mode enable 								*/
        volatile uint32_t irlp     		:  1 ;    	/*!< [Bit 2] 		IrDA low-power 									*/
        volatile uint32_t hdsel    		:  1 ;    	/*!< [Bit 3] 		Half-duplex selection 							*/
        volatile uint32_t nack     		:  1 ;    	/*!< [Bit 4] 		Smartcard NACK enable 							*/
        volatile uint32_t scen     		:  1 ;    	/*!< [Bit 5] 		Smartcard mode enable 							*/
        volatile uint32_t dmar     		:  1 ;    	/*!< [Bit 6] 		DMA enable receiver 							*/
        volatile uint32_t dmat     		:  1 ;    	/*!< [Bit 7] 		DMA enable transmitter 							*/
        volatile uint32_t rtse     		:  1 ;    	/*!< [Bit 8] 		RTS enable 										*/
        volatile uint32_t ctse     		:  1 ;    	/*!< [Bit 9] 		CTS enable 										*/
        volatile uint32_t ctsie    		:  1 ;    	/*!< [Bit 10]		CTS interrupt enable 							*/
        volatile uint32_t onebit   		:  1 ;    	/*!< [Bit 11] 		One sample bit method enable 					*/
        		  uint32_t reserved		: 20 ;   	/*!< [Bits 31:12] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_CR3_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t psc      		:  8 ;    	/*!< [Bits 7:0] 	Prescaler value 								*/
        volatile uint32_t gt       		:  8 ;    	/*!< [Bits 15:8] 	Guard time value 								*/
        		  uint32_t reserved 	: 16 ;   	/*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}USART_GTPR_Reg_t;

typedef struct
{
USART_SR_Reg_t 		SR;         /*!< USART Status register,                   Address offset: 0x00 */
USART_DR_Reg_t 		DR;         /*!< USART Data register,                     Address offset: 0x04 */
USART_BRR_Reg_t 	BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
USART_CR1_Reg_t 	CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
USART_CR2_Reg_t		CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
USART_CR3_Reg_t 	CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
USART_GTPR_Reg_t 	GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
}USART_RegDef_t;

/**
  * @brief Window WATCHDOG
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t t       		:  7 ;    	/*!< [Bits 6:0] 	7-bit counter (MSB to LSB) 						*/
        volatile uint32_t wdga    		:  1 ;    	/*!< [Bit 7] 		Activation bit 									*/
        		  uint32_t reserved		: 24 ;   	/*!< [Bits 31:8] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}WWDG_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t w      		:  7 ;    	/*!< [Bits 6:0] 	7-bit window value 								*/
        volatile uint32_t wdgtb  		:  2 ;    	/*!< [Bits 8:7] 	Timer base 										*/
        volatile uint32_t ewi    		:  1 ;    	/*!< [Bit 9] 		Early wakeup interrupt 							*/
        		  uint32_t reserved     : 22 ;   	/*!< [Bits 31:10] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}WWDG_CFR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ewif     		:  1 ;    	/*!< [Bit 0] 		Early wakeup interrupt flag 					*/
        		  uint32_t reserved 	: 31 ;   	/*!< [Bits 31:1] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}WWDG_SR_Reg_t;

typedef struct
{
WWDG_CR_Reg_t 	CR;   /*!< WWDG Control register,       Address offset: 0x00 */
WWDG_CFR_Reg_t 	CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
WWDG_SR_Reg_t 	SR;   /*!< WWDG Status register,        Address offset: 0x08 */
}WWDG_RegDef_t;


/**
  * @brief RNG
  */

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0	:  2 ;    	/*!< [Bits 1:0] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t rngen     	:  1 ;    	/*!< [Bit 2] 		Random number generator enable 					*/
        volatile uint32_t ie        	:  1 ;    	/*!< [Bit 3] 		Interrupt enable 								*/
        		  uint32_t reserved_1	: 28 ;   	/*!< [Bits 31:4] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RNG_CR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t drdy 			:  1 ;    	/*!< [Bit 0] 		Data ready 										*/
        volatile uint32_t cecs 			:  1 ;    	/*!< [Bit 1] 		Clock error current status 						*/
        volatile uint32_t secs 			:  1 ;    	/*!< [Bit 2] 		Seed error current status 						*/
        		  uint32_t reserved_0 	:  2 ;      /*!< [Bits 4:3] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t ceis 			:  1 ;    	/*!< [Bit 5] 		Clock error interrupt status 					*/
        volatile uint32_t seis 			:  1 ;    	/*!< [Bit 6] 		Seed error interrupt status 					*/
        		  uint32_t reserved_1 	: 25 ;      /*!< [Bits 31:7] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}RNG_SR_Reg_t;

typedef struct
{
RNG_CR_Reg_t 		CR;  /*!< RNG control register, Address offset: 0x00 */
RNG_SR_Reg_t 		SR;  /*!< RNG status register,  Address offset: 0x04 */
volatile uint32_t 	DR;  /*!< RNG data register,    Address offset: 0x08 */
}RNG_RegDef_t;

/**
  * @brief USB_OTG_Core_Registers
  */

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t srqscs 		:  1 ;  	/*!< [Bit 0] 		Session request success 						*/
        volatile uint32_t srq 			:  1 ;      /*!< [Bit 1] 		Session request 								*/
        		  uint32_t reserved_0 	:  6 ;     	/*!< [Bits 7:2] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t hngscs 		:  1 ;    	/*!< [Bit 8] 		Host negotiation success 						*/
        volatile uint32_t hnprq 		:  1 ;     	/*!< [Bit 9] 		HNP request 									*/
        volatile uint32_t hshnpen 		:  1 ;   	/*!< [Bit 10] 		Host set HNP enable 							*/
        volatile uint32_t dhnpen 		:  1 ;    	/*!< [Bit 11] 		Device HNP enabled 								*/
        		  uint32_t reserved_1 	:  4 ;     	/*!< [Bits 15:12] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t cidsts 		:  1 ;    	/*!< [Bit 16] 		Connector ID status 							*/
        volatile uint32_t dbct 			:  1 ;      /*!< [Bit 17] 		Long/short debounce time 						*/
        volatile uint32_t asvld 		:  1 ;     	/*!< [Bit 18] 		A-session valid 								*/
        volatile uint32_t bsvld 		:  1 ;     	/*!< [Bit 19] 		B-session valid 								*/
        		  uint32_t reserved_2 	: 12 ;     	/*!< [Bits 31:20] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GOTGCTL_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	:  2 ;   	/*!< [Bits 1:0] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t sedet 		:  1 ;    	/*!< [Bit 2] 		Session end detected 							*/
        		  uint32_t reserved_1 	:  5 ;    	/*!< [Bits 7:3] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t srsschg 		:  1 ;  	/*!< [Bit 8] 		Session request success status change 			*/
        volatile uint32_t hnschg 		:  1 ;   	/*!< [Bit 9] 		Host negotiation success status change 			*/
        		  uint32_t reserved_2 	:  7 ;    	/*!< [Bits 16:10] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t hngdet 		:  1 ;   	/*!< [Bit 17] 		Host negotiation detected 						*/
        volatile uint32_t adtochg 		:  1 ;  	/*!< [Bit 18] 		A-device timeout change 						*/
        volatile uint32_t dbcdne 		:  1 ;   	/*!< [Bit 19] 		Debounce done 									*/
        		  uint32_t reserved_3 	: 12 ;      /*!< [Bits 31:20] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GOTGINT_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t gintmsk 		:  1 ;   	/*!< [Bit 0] 		Global interrupt mask 							*/
        		  uint32_t reserved_0	:  6 ;     	/*!< [Bits 6:1] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t txfelvl 		:  1 ;   	/*!< [Bit 7] 		TxFIFO empty level 								*/
        volatile uint32_t ptxfelvl 		:  1 ;  	/*!< [Bit 8] 		Periodic TxFIFO empty level 					*/
        		  uint32_t reserved_1 	: 23 ;    	/*!< [Bits 31:9] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GAHBCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t gint 			:  1 ;   	/*!< [Bit 0] 		Global interrupt mask 							*/
        volatile uint32_t hbstlen 		:  4 ;   	/*!< [Bits 4:1] 	Global interrupt mask 							*/
        volatile uint32_t dmaen 		:  1 ;   	/*!< [Bit 5] 		Global interrupt mask 							*/
        		  uint32_t reserved_0	:  1 ;     	/*!< [Bit 6] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t txfelvl 		:  1 ;   	/*!< [Bit 7] 		TxFIFO empty level 								*/
        volatile uint32_t ptxfelvl 		:  1 ;  	/*!< [Bit 8] 		Periodic TxFIFO empty level 					*/
        		  uint32_t reserved_1 	: 23 ;    	/*!< [Bits 31:9] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_HS_GAHBCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tocal 		:  3 ;    	/*!< [Bits 2:0] 	FS timeout calibration 							*/
        		  uint32_t reserved_0	:  3 ;     	/*!< [Bits 5:3] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t physel 		:  1 ;   	/*!< [Bit 6] 		Full-speed serial transceiver select 			*/
        		  uint32_t reserved_1	:  1 ;     	/*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t srpcap 		:  1 ;   	/*!< [Bit 8] 		SRP-capable 									*/
        volatile uint32_t hnpcap 		:  1 ;   	/*!< [Bit 9] 		HNP-capable 									*/
        volatile uint32_t trdt 			:  4 ;     	/*!< [Bits 13:10] 	USB turnaround time 							*/
        		  uint32_t reserved_4	: 15 ;      /*!< [Bits 28:14] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t fhmod 		:  1 ;    	/*!< [Bit 29] 		Force host mode 								*/
        volatile uint32_t fdmod 		:  1 ;    	/*!< [Bit 30] 		Force device mode 								*/
        volatile uint32_t ctxpkt 		:  1 ;   	/*!< [Bit 31] 		Corrupt Tx packet 								*/
    }bit;
}OTG_FS_GUSBCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t tocal 		:  3 ;    	/*!< [Bits 2:0] 	FS timeout calibration 							*/
        		  uint32_t reserved_0	:  3 ;     	/*!< [Bits 5:3] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t physel 		:  1 ;   	/*!< [Bit 6] 		Full-speed serial transceiver select 			*/
        		  uint32_t reserved_1	:  1 ;     	/*!< [Bit 7] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t srpcap 		:  1 ;   	/*!< [Bit 8] 		SRP-capable 									*/
        volatile uint32_t hnpcap 		:  1 ;   	/*!< [Bit 9] 		HNP-capable 									*/
        volatile uint32_t trdt 			:  4 ;     	/*!< [Bits 13:10] 	USB turnaround time 							*/
        		  uint32_t reserved_2	:  1 ;      /*!< [Bit 14] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t phylpcs 		:  1 ;     	/*!< [Bit 15] 		PHY Low-power clock select 						*/
        		  uint32_t reserved_3	:  1 ;      /*!< [Bit 16] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t ulpifsls 		:  1 ;     	/*!< [Bit 17] 		ULPI FS/LS select								*/
        volatile uint32_t ulpiar 		:  1 ;     	/*!< [Bit 18] 		ULPI Auto-resume 								*/
        volatile uint32_t ulpicsm 		:  1 ;     	/*!< [Bit 19] 		ULPI Clock SuspendM								*/
        volatile uint32_t ulpievbusd 	:  1 ;     	/*!< [Bit 20] 		ULPI External VBUS Drive						*/
        volatile uint32_t ulpievbusi 	:  1 ;     	/*!< [Bit 21] 		ULPI external VBUS indicator					*/
        volatile uint32_t tsdps 		:  1 ;     	/*!< [Bit 22] 		TermSel DLine pulsing selection 				*/
        volatile uint32_t pcci 			:  1 ;     	/*!< [Bit 23] 		Indicator complement							*/
        volatile uint32_t ptci 			:  1 ;     	/*!< [Bit 24] 		Indicator pass through							*/
        volatile uint32_t ulpiipd 		:  1 ;     	/*!< [Bit 25] 		ULPI interface protect disable					*/
        		  uint32_t reserved_4	:  3 ;      /*!< [Bits 28:26] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t fhmod 		:  1 ;    	/*!< [Bit 29] 		Force host mode 								*/
        volatile uint32_t fdmod 		:  1 ;    	/*!< [Bit 30] 		Force device mode 								*/
        volatile uint32_t ctxpkt 		:  1 ;   	/*!< [Bit 31] 		Corrupt Tx packet 								*/
    }bit;
}OTG_HS_GUSBCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t csrst 		:  1 ;    	/*!< [Bits 2:0] 	Core soft reset 								*/
        volatile uint32_t hsrst 		:  1 ;    	/*!< [Bit 1] 		HCLK soft reset 								*/
        volatile uint32_t fcrst 		:  1 ;    	/*!< [Bit 2] 		Host frame counter reset 						*/
        		  uint32_t reserved_0 	:  1 ;      /*!< [Bit 3] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t rxfflsh 		:  1 ;  	/*!< [Bit 4] 		RxFIFO flush 									*/
        volatile uint32_t txfflsh 		:  1 ;  	/*!< [Bit 5] 		TxFIFO flush 									*/
        volatile uint32_t txfnum 		:  5 ;   	/*!< [Bits 10:6] 	TxFIFO number 									*/
        		  uint32_t reserved_2	: 20 ;      /*!< [Bits 30:11] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t ahbidl 		:  1 ;   	/*!< [Bit 31] 		AHB master idle 								*/
    	   }bit;
}OTG_FS_GRSTCTL_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t csrst 		:  1 ;    	/*!< [Bits 2:0] 	Core soft reset 								*/
        volatile uint32_t hsrst 		:  1 ;    	/*!< [Bit 1] 		HCLK soft reset 								*/
        volatile uint32_t fcrst 		:  1 ;    	/*!< [Bit 2] 		Host frame counter reset 						*/
        		  uint32_t reserved_0 	:  1 ;      /*!< [Bit 3] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t rxfflsh 		:  1 ;  	/*!< [Bit 4] 		RxFIFO flush 									*/
        volatile uint32_t txfflsh 		:  1 ;  	/*!< [Bit 5] 		TxFIFO flush 									*/
        volatile uint32_t txfnum 		:  5 ;   	/*!< [Bits 10:6] 	TxFIFO number 									*/
        		  uint32_t reserved_2	: 19 ;      /*!< [Bits 29:11] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t dmareq 		:  1 ;   	/*!< [Bit 30] 		DMA request signal 								*/
        volatile uint32_t ahbidl 		:  1 ;   	/*!< [Bit 31] 		AHB master idle 								*/
    	   }bit;
}OTG_HS_GRSTCTL_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cmod 			: 1 ;     	/*!< [Bit 0] 		Current mode of operation 						*/
        volatile uint32_t mmis 			: 1 ;       /*!< [Bit 1] 		Mode mismatch interrupt 						*/
        volatile uint32_t otgint 		: 1 ;      	/*!< [Bit 2] 		OTG interrupt 									*/
        volatile uint32_t sof 			: 1 ;       /*!< [Bit 3] 		Start of frame 									*/
        volatile uint32_t rxflvl 		: 1 ;      	/*!< [Bit 4] 		RxFIFO non-empty 								*/
        volatile uint32_t nptxfe		: 1 ;      	/*!< [Bit 5] 		Non-periodic TxFIFO empty 						*/
        volatile uint32_t ginakeff 		: 1 ;    	/*!< [Bit 6] 		Global IN non-periodic NAK effective 			*/
        volatile uint32_t gonakeff 		: 1 ;    	/*!< [Bit 7] 		Global OUT NAK effective 						*/
        		  uint32_t reserved_0 	: 2 ;       /*!< [Bits 9:8] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t esusp 		: 1 ;       /*!< [Bit 10] 		Early suspend 									*/
        volatile uint32_t usbsusp 		: 1 ;     	/*!< [Bit 11] 		USB suspend 									*/
        volatile uint32_t usbrst 		: 1 ;      	/*!< [Bit 12] 		USB reset 										*/
        volatile uint32_t enumdne 		: 1 ;     	/*!< [Bit 13] 		Enumeration done 								*/
        volatile uint32_t isoodrp 		: 1 ;     	/*!< [Bit 14] 		Isochronous OUT packet dropped interrupt 		*/
        volatile uint32_t eopf 			: 1 ;       /*!< [Bit 15] 		End of periodic frame interrupt					*/
        		  uint32_t reserved_1	: 2 ;       /*!< [Bits 17:16] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t iepint 		: 1 ;      	/*!< [Bit 18] 		IN endpoint interrupt 							*/
        volatile uint32_t oepint 		: 1 ;      	/*!< [Bit 19] 		OUT endpoint interrupt 							*/
        volatile uint32_t iisoixfr 		: 1 ;    	/*!< [Bit 20] 		Incomplete isochronous IN transfer 				*/
        volatile uint32_t ipxfr 		: 1 ;      	/*!< [Bit 21] 		Incomplete periodic transfer 					*/
        		  uint32_t reserved_2 	: 2 ;       /*!< [Bits 23:22] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t hprtint		: 1 ;		/*!< [Bit 24] 		Host port interrupt 							*/
        volatile uint32_t hcint			: 1 ;		/*!< [Bit 25] 		Host channels interrupt 						*/
        volatile uint32_t ptxfe			: 1 ;		/*!< [Bit 26] 		Periodic TxFIFO empty 							*/
        		  uint32_t reserved_3	: 1 ;		/*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cidschg 		: 1 ;     	/*!< [Bit 28] 		Connector ID status change 						*/
        volatile uint32_t discint 		: 1 ;     	/*!< [Bit 29] 		Disconnect detected interrupt 					*/
        volatile uint32_t srqint 		: 1 ;      	/*!< [Bit 30] 		Session request/new session detected interrupt 	*/
        volatile uint32_t wkupint	 	: 1 ;     	/*!< [Bit 31] 		Resume/remote wakeup detected interrupt 		*/
    }bit;
}OTG_FS_GINTSTS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t cmod 			: 1 ;     	/*!< [Bit 0] 		Current mode of operation 						*/
        volatile uint32_t mmis 			: 1 ;       /*!< [Bit 1] 		Mode mismatch interrupt 						*/
        volatile uint32_t otgint 		: 1 ;      	/*!< [Bit 2] 		OTG interrupt 									*/
        volatile uint32_t sof 			: 1 ;       /*!< [Bit 3] 		Start of frame 									*/
        volatile uint32_t rxflvl 		: 1 ;      	/*!< [Bit 4] 		RxFIFO non-empty 								*/
        volatile uint32_t nptxfe		: 1 ;      	/*!< [Bit 5] 		Non-periodic TxFIFO empty 						*/
        volatile uint32_t ginakeff 		: 1 ;    	/*!< [Bit 6] 		Global IN non-periodic NAK effective 			*/
        volatile uint32_t gonakeff 		: 1 ;    	/*!< [Bit 7] 		Global OUT NAK effective 						*/
        		  uint32_t reserved_0 	: 2 ;       /*!< [Bits 9:8] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t esusp 		: 1 ;       /*!< [Bit 10] 		Early suspend 									*/
        volatile uint32_t usbsusp 		: 1 ;     	/*!< [Bit 11] 		USB suspend 									*/
        volatile uint32_t usbrst 		: 1 ;      	/*!< [Bit 12] 		USB reset 										*/
        volatile uint32_t enumdne 		: 1 ;     	/*!< [Bit 13] 		Enumeration done 								*/
        volatile uint32_t isoodrp 		: 1 ;     	/*!< [Bit 14] 		Isochronous OUT packet dropped interrupt 		*/
        volatile uint32_t eopf 			: 1 ;       /*!< [Bit 15] 		End of periodic frame interrupt					*/
        		  uint32_t reserved_1	: 2 ;       /*!< [Bits 17:16] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t iepint 		: 1 ;      	/*!< [Bit 18] 		IN endpoint interrupt 							*/
        volatile uint32_t oepint 		: 1 ;      	/*!< [Bit 19] 		OUT endpoint interrupt 							*/
        volatile uint32_t iisoixfr 		: 1 ;    	/*!< [Bit 20] 		Incomplete isochronous IN transfer 				*/
        volatile uint32_t ipxfr 		: 1 ;      	/*!< [Bit 21] 		Incomplete periodic transfer 					*/
        volatile uint32_t datasusp 		: 1 ;      	/*!< [Bit 22] 		Data fetch suspended							*/
        		  uint32_t reserved_2 	: 1 ;       /*!< [Bit 23] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t hprtint		: 1 ;		/*!< [Bit 24] 		Host port interrupt 							*/
        volatile uint32_t hcint			: 1 ;		/*!< [Bit 25] 		Host channels interrupt 						*/
        volatile uint32_t ptxfe			: 1 ;		/*!< [Bit 26] 		Periodic TxFIFO empty 							*/
        		  uint32_t reserved_3	: 1 ;		/*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cidschg 		: 1 ;     	/*!< [Bit 28] 		Connector ID status change 						*/
        volatile uint32_t discint 		: 1 ;     	/*!< [Bit 29] 		Disconnect detected interrupt 					*/
        volatile uint32_t srqim 		: 1 ;      	/*!< [Bit 30] 		Session request/new session detected interrupt 	*/
        volatile uint32_t wkupint	 	: 1 ;     	/*!< [Bit 31] 		Resume/remote wakeup detected interrupt 		*/
    }bit;
}OTG_HS_GINTSTS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	: 1 ;    	/*!< [Bit 0] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mmism 		: 1 ;       /*!< [Bit 1] 		Mode mismatch interrupt mask 					*/
        volatile uint32_t otgint 		: 1 ;       /*!< [Bit 2] 		OTG interrupt mask 								*/
        volatile uint32_t sofm 			: 1 ;       /*!< [Bit 3]		 Start of frame mask 							*/
        volatile uint32_t rxflvlm 		: 1 ;       /*!< [Bit 4] 		Receive FIFO non-empty mask 					*/
        volatile uint32_t nptxfem 		: 1 ;       /*!< [Bit 5] 		Non-periodic TxFIFO empty mask 					*/
        volatile uint32_t ginakeffm 	: 1 ;     	/*!< [Bit 6] 		Global non-periodic IN NAK effective mask 		*/
        volatile uint32_t gonakeffm 	: 1 ;     	/*!< [Bit 7] 		Global OUT NAK effective mask 					*/
        volatile uint32_t reserved_1 	: 2 ;    	/*!< [Bits 9:8] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t esuspm 		: 1 ;       /*!< [Bit 10] 		Early suspend mask 								*/
        volatile uint32_t usbsuspm 		: 1 ;      	/*!< [Bit 11] 		USB suspend mask 								*/
        volatile uint32_t usbrst 		: 1 ;       /*!< [Bit 12] 		USB reset mask 									*/
        volatile uint32_t enumdnem 		: 1 ;      	/*!< [Bit 13] 		Enumeration done mask 							*/
        volatile uint32_t isoodrpm 		: 1 ;      	/*!< [Bit 14] 		Isochronous OUT packet dropped interrupt mask 	*/
        volatile uint32_t eopfm		 	: 1 ;       /*!< [Bit 15] 		End of periodic frame interrupt mask 			*/
        		  uint32_t reserved_2 	: 2 ;       /*!< [Bits 17:16] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t iepint 		: 1 ;       /*!< [Bit 18] 		IN endpoints interrupt mask 					*/
        volatile uint32_t oepint 		: 1 ;       /*!< [Bit 19] 		OUT endpoints interrupt mask 					*/
        volatile uint32_t iisoixfrm 	: 1 ;     	/*!< [Bit 20] 		Incomplete isochronous IN transfer mask 		*/
        volatile uint32_t ipxfrm 		: 1 ;       /*!< [Bit 21] 		Incomplete periodic transfer mask 				*/
        		  uint32_t reserved_3 	: 2 ;       /*!< [Bits 23:22] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t hprtint 		: 1 ;       /*!< [Bit 24] 		Host port interrupt mask 						*/
        volatile uint32_t hcim 			: 1 ;       /*!< [Bit 25] 		Host channels interrupt mask 					*/
        volatile uint32_t ptxfem 		: 1 ;       /*!< [Bit 26] 		Periodic TxFIFO empty mask 						*/
        		  uint32_t reserved_4 	: 1 ;       /*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cidschgm 		: 1 ;      	/*!< [Bit 28] 		Connector ID status change mask 				*/
        volatile uint32_t discint 		: 1 ;       /*!< [Bit 29] 		Disconnect detected interrupt mask			 	*/
        volatile uint32_t srqim 		: 1 ;       /*!< [Bit 30] 		Session request/new session detected interrupt mask */
        volatile uint32_t wuim 			: 1 ;       /*!< [Bit 31] 		Resume/remote wakeup detected interrupt mask 	*/
    	   }bit;
}OTG_FS_GINTMSK_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	: 1 ;    	/*!< [Bit 0] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t mmism 		: 1 ;       /*!< [Bit 1] 		Mode mismatch interrupt mask 					*/
        volatile uint32_t otgint 		: 1 ;       /*!< [Bit 2] 		OTG interrupt mask 								*/
        volatile uint32_t sofm 			: 1 ;       /*!< [Bit 3]		 Start of frame mask 							*/
        volatile uint32_t rxflvlm 		: 1 ;       /*!< [Bit 4] 		Receive FIFO non-empty mask 					*/
        volatile uint32_t nptxfem 		: 1 ;       /*!< [Bit 5] 		Non-periodic TxFIFO empty mask 					*/
        volatile uint32_t ginakeffm 	: 1 ;     	/*!< [Bit 6] 		Global non-periodic IN NAK effective mask 		*/
        volatile uint32_t gonakeffm 	: 1 ;     	/*!< [Bit 7] 		Global OUT NAK effective mask 					*/
        volatile uint32_t reserved_1 	: 2 ;    	/*!< [Bits 9:8] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t esuspm 		: 1 ;       /*!< [Bit 10] 		Early suspend mask 								*/
        volatile uint32_t usbsuspm 		: 1 ;      	/*!< [Bit 11] 		USB suspend mask 								*/
        volatile uint32_t usbrst 		: 1 ;       /*!< [Bit 12] 		USB reset mask 									*/
        volatile uint32_t enumdnem 		: 1 ;      	/*!< [Bit 13] 		Enumeration done mask 							*/
        volatile uint32_t isoodrpm 		: 1 ;      	/*!< [Bit 14] 		Isochronous OUT packet dropped interrupt mask 	*/
        volatile uint32_t eopfm		 	: 1 ;       /*!< [Bit 15] 		End of periodic frame interrupt mask 			*/
        		  uint32_t reserved_2 	: 2 ;       /*!< [Bits 17:16] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t iepint 		: 1 ;       /*!< [Bit 18] 		IN endpoints interrupt mask 					*/
        volatile uint32_t oepint 		: 1 ;       /*!< [Bit 19] 		OUT endpoints interrupt mask 					*/
        volatile uint32_t iisoixfrm 	: 1 ;     	/*!< [Bit 20] 		Incomplete isochronous IN transfer mask 		*/
        volatile uint32_t ipxfrm 		: 1 ;       /*!< [Bit 21] 		Incomplete periodic transfer mask 				*/
        volatile uint32_t fsuspm 		: 1 ;       /*!< [Bit 22] 		Data fetch suspended mask 						*/
        		  uint32_t reserved_3 	: 1 ;       /*!< [Bit 23] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t hprtint 		: 1 ;       /*!< [Bit 24] 		Host port interrupt mask 						*/
        volatile uint32_t hcim 			: 1 ;       /*!< [Bit 25] 		Host channels interrupt mask 					*/
        volatile uint32_t ptxfem 		: 1 ;       /*!< [Bit 26] 		Periodic TxFIFO empty mask 						*/
        		  uint32_t reserved_4 	: 1 ;       /*!< [Bit 27] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t cidschgm 		: 1 ;      	/*!< [Bit 28] 		Connector ID status change mask 				*/
        volatile uint32_t discint 		: 1 ;       /*!< [Bit 29] 		Disconnect detected interrupt mask			 	*/
        volatile uint32_t srqim 		: 1 ;       /*!< [Bit 30] 		Session request/new session detected interrupt mask */
        volatile uint32_t wuim 			: 1 ;       /*!< [Bit 31] 		Resume/remote wakeup detected interrupt mask 	*/
    	   }bit;
}OTG_HS_GINTMSK_Reg_t;

typedef union {
    uint32_t reg;
    struct {
        volatile uint32_t chnum 		:  4 ;    	/*!< [Bits 3:0] 	Channel number 									*/
        volatile uint32_t bcnt 			: 11 ;      /*!< [Bits 14:4]	Byte count 										*/
        volatile uint32_t dpid 			:  2 ;      /*!< [Bits 16:15] 	Data PID 										*/
        volatile uint32_t pktsts 		:  4 ;     	/*!< [Bits 20:17] 	Packet status 									*/
        		  uint32_t reserved 	: 11 ;      /*!< [Bits 31:21] 	Reserved, must be kept at reset value 			*/
    } bit;
} OTG_FS_GRXSTSR_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t epnum 		:  4 ;     	/*!< [Bits 3:0] 	Endpoint number 								*/
        volatile uint32_t bcnt 			: 11 ;      /*!< [Bits 14:4] 	Byte count 										*/
        volatile uint32_t dpid 			:  2 ;      /*!< [Bits 16:15] 	Data PID 										*/
        volatile uint32_t pktsts 		:  4 ;     	/*!< [Bits 20:17] 	Packet status 									*/
        volatile uint32_t frmnum 		:  4 ;     	/*!< [Bits 24:21] 	Frame number 									*/
        		  uint32_t reserved 	:  7 ;     	/*!< [Bits 31:25] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GRXSTSP_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t rxfd 			: 16 ;      /*!< [Bits 15:0] 	RxFIFO depth 									*/
        		  uint32_t reserved 	: 16 ;      /*!< [Bits 31:16] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GRXFSIZ_Reg_t;

typedef union{
	uint32_t reg;
	struct{
        volatile uint32_t nptxsa 		: 16 ;  	/*!< [Bits 15:0] 	Non-periodic transmit RAM start address 		*/
        volatile uint32_t nptxfd 		: 16 ;  	/*!< [Bits 31:16] 	Non-periodic TxFIFO depth 						*/
		   }bit;
}OTG_HS_GNPTXFSIZ_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t nptxsa 		: 16 ;  	/*!< [Bits 15:0] 	Non-periodic transmit RAM start address 		*/
        volatile uint32_t nptxfd 		: 16 ;  	/*!< [Bits 31:16] 	Non-periodic TxFIFO depth 						*/
    }bit;
}OTG_FS_DIEPTXF0_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t nptxfsav 		: 16 ;    	/*!< [Bits 15:0] 	Non-periodic TxFIFO space available 			*/
        volatile uint32_t nptqxsav 		:  8 ;    	/*!< [Bits 23:16] 	Non-periodic transmit request queue space available */
        volatile uint32_t nptxqtop 		:  1 ;    	/*!< [Bits 30:24] 	Terminate (last entry for selected channel/endpoint)*/
        		  uint32_t reserved 	:  1 ;    	/*!< [Bit 31] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_HNPTXSTS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t nptxfsav 		: 16 ;    	/*!< [Bits 15:0] 	Non-periodic TxFIFO space available 			*/
        volatile uint32_t nptqxsav 		:  8 ;    	/*!< [Bits 23:16] 	Non-periodic transmit request queue space available */
        volatile uint32_t nptxqtop 		:  1 ;    	/*!< [Bits 30:24] 	Top of the nonperiodic transmit request queue	*/
        		  uint32_t reserved 	:  1 ;    	/*!< [Bit 31] 		Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_HS_GNPTXSTS_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	: 16 ;  	/*!< [Bits 15:0] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t pwrdown 		:  1 ;  	/*!< [Bit 16] 		Power down 										*/
        		  uint32_t reserved_1 	:  1 ;      /*!< [Bit 17] 		Reserved, must be kept at reset value 			*/
        volatile uint32_t vbusasen 		:  1 ; 		/*!< [Bit 18] 		Enable the VBUS sensing “A” device 				*/
        volatile uint32_t vbusbsen 		:  1 ;  	/*!< [Bit 19] 		Enable the VBUS sensing “B” device 				*/
        volatile uint32_t sofouten 		:  1 ; 		/*!< [Bit 20] 		SOF output enable 								*/
        volatile uint32_t novbussens 	:  1 ; 		/*!< [Bit 21] 		VBUS sensing disable option 					*/
        		  uint32_t reserved_2 	: 10 ;      /*!< [Bits 31:22] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_FS_GCCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        		  uint32_t reserved_0 	: 16 ;  	/*!< [Bits 15:0] 	Reserved, must be kept at reset value 			*/
        volatile uint32_t pwrdown 		:  1 ;  	/*!< [Bit 16] 		Power down 										*/
        volatile uint32_t i2cpaden 		:  1 ;  	/*!< [Bit 17] 		Enable I2C bus connection for the external I2C PHY interface*/
        volatile uint32_t vbusasen 		:  1 ; 		/*!< [Bit 18] 		Enable the VBUS sensing “A” device 				*/
        volatile uint32_t vbusbsen 		:  1 ;  	/*!< [Bit 19] 		Enable the VBUS sensing “B” device 				*/
        volatile uint32_t sofouten 		:  1 ; 		/*!< [Bit 20] 		SOF output enable 								*/
        volatile uint32_t novbussens 	:  1 ; 		/*!< [Bit 21] 		VBUS sensing disable option 					*/
        		  uint32_t reserved_2 	: 10 ;      /*!< [Bits 31:22] 	Reserved, must be kept at reset value 			*/
    	   }bit;
}OTG_HS_GCCFG_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ptxsa 		: 16 ;   	/*!< [Bits 15:0] 	Host periodic TxFIFO start address 				*/
        volatile uint32_t ptxfd 		: 16 ;   	/*!< [Bits 31:16] 	Host periodic TxFIFO depth 						*/
    	   }bit;
}OTG_FS_HPTXFSIZ_Reg_t;

typedef union{
    uint32_t reg;
    struct{
        volatile uint32_t ineptxsa 		: 16 ;   	/*!< [Bits 15:0] 	IN endpoint FIFOx transmit RAM start address 	*/
        volatile uint32_t ineptxfd 		: 16 ;   	/*!< [Bits 31:16] 	IN endpoint TxFIFO depth 						*/
    	   }bit;
}OTG_FS_DIEPTXF_Reg_t;

typedef struct
{
OTG_FS_GOTGCTL_Reg_t 	GOTGCTL;              /*!< USB_OTG Control and Status Register          000h */
OTG_FS_GOTGINT_Reg_t 	GOTGINT;              /*!< USB_OTG Interrupt Register                   004h */
OTG_FS_GAHBCFG_Reg_t 	GAHBCFG;              /*!< Core AHB Configuration Register              008h */
OTG_FS_GUSBCFG_Reg_t 	GUSBCFG;              /*!< Core USB Configuration Register              00Ch */
OTG_FS_GRSTCTL_Reg_t 	GRSTCTL;              /*!< Core Reset Register                          010h */
OTG_FS_GINTSTS_Reg_t 	GINTSTS;              /*!< Core Interrupt Register                      014h */
OTG_FS_GINTMSK_Reg_t 	GINTMSK;              /*!< Core Interrupt Mask Register                 018h */
OTG_FS_GRXSTSR_Reg_t 	GRXSTSR;              /*!< Receive Sts Q Read Register                  01Ch */
OTG_FS_GRXSTSP_Reg_t 	GRXSTSP;              /*!< Receive Sts Q Read & POP Register            020h */
OTG_FS_GRXFSIZ_Reg_t 	GRXFSIZ;              /*!< Receive FIFO Size Register                   024h */
OTG_FS_DIEPTXF0_Reg_t 	DIEPTXF0_HNPTXFSIZ;   /*!< EP0 / Non Periodic Tx FIFO Size Register     028h */
OTG_FS_HNPTXSTS_Reg_t 	HNPTXSTS;             /*!< Non Periodic Tx FIFO/Queue Sts reg           02Ch */
  uint32_t 				Reserved30[2];        /*!< Reserved                                     030h */
OTG_FS_GCCFG_Reg_t 		GCCFG;                /*!< General Purpose IO Register                  038h */
volatile uint32_t 		CID;                  /*!< User ID Register                             03Ch */
  uint32_t  			Reserved40[48];       /*!< Reserved                                0x40-0xFF */
OTG_FS_HPTXFSIZ_Reg_t 	HPTXFSIZ;             /*!< Host Periodic Tx FIFO Size Reg               100h */
OTG_FS_DIEPTXF_Reg_t 	DIEPTXF[0x0F];        /*!< dev Periodic Transmit FIFO                        */
}USB_OTG_FS_Reg_t;

typedef struct
{
OTG_FS_GOTGCTL_Reg_t 	GOTGCTL;              /*!< USB_OTG Control and Status Register          000h */
OTG_FS_GOTGINT_Reg_t 	GOTGINT;              /*!< USB_OTG Interrupt Register                   004h */
OTG_HS_GAHBCFG_Reg_t 	GAHBCFG;              /*!< Core AHB Configuration Register              008h */
OTG_HS_GUSBCFG_Reg_t 	GUSBCFG;              /*!< Core USB Configuration Register              00Ch */
OTG_HS_GRSTCTL_Reg_t 	GRSTCTL;              /*!< Core Reset Register                          010h */
OTG_HS_GINTSTS_Reg_t 	GINTSTS;              /*!< Core Interrupt Register                      014h */
OTG_HS_GINTMSK_Reg_t 	GINTMSK;              /*!< Core Interrupt Mask Register                 018h */
OTG_FS_GRXSTSR_Reg_t 	GRXSTSR;              /*!< Receive Sts Q Read Register                  01Ch */
OTG_FS_GRXSTSP_Reg_t 	GRXSTSP;              /*!< Receive Sts Q Read & POP Register            020h */
OTG_FS_GRXFSIZ_Reg_t 	GRXFSIZ;              /*!< Receive FIFO Size Register                   024h */
OTG_HS_GNPTXFSIZ_Reg_t 	OTG_HS_GNPTXFSIZ;     /*!< EP0 / Non Periodic Tx FIFO Size Register     028h */
OTG_HS_GNPTXSTS_Reg_t 	GNPTXSTS;             /*!< Non Periodic Tx FIFO/Queue Sts reg           02Ch */
  uint32_t 				Reserved30[2];        /*!< Reserved                                     030h */
OTG_HS_GCCFG_Reg_t 		GCCFG;                /*!< General Purpose IO Register                  038h */
volatile uint32_t 		CID;                  /*!< User ID Register                             03Ch */
  uint32_t  			Reserved40[48];       /*!< Reserved                                0x40-0xFF */
OTG_FS_HPTXFSIZ_Reg_t 	HPTXFSIZ;             /*!< Host Periodic Tx FIFO Size Reg               100h */
OTG_FS_DIEPTXF_Reg_t 	DIEPTXF[0x0F];        /*!< dev Periodic Transmit FIFO                        */
}USB_OTG_HS_Reg_t;

/**
  * @brief Nested Vectored Interrupt Controller (NVIC)
  */
//
//typedef union{
//    uint32_t reg;
//    struct{
//        volatile uint32_t N0 			: 8 ;   	/*!< [Bits 7:0] 	Priority, byte offset = 0 						*/
//        volatile uint32_t N1 			: 8 ;   	/*!< [Bits 15:8] 	Priority, byte offset = 1 						*/
//        volatile uint32_t N2 			: 8 ;   	/*!< [Bits 23:16] 	Priority, byte offset = 2 						*/
//        volatile uint32_t N3 			: 8 ;   	/*!< [Bits 31:24] 	Priority, byte offset = 3 						*/
//    	   }bit;
//}NVIC_IP_Reg_t;
//
//typedef struct
//{
//volatile uint32_t ISER0;			  	/*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
//volatile uint32_t ISER1;			  	/*!< Offset: 0x004 (R/W)  Interrupt Set Enable Register */
//volatile uint32_t ISER2;			  	/*!< Offset: 0x008 (R/W)  Interrupt Set Enable Register */
//uint32_t RESERVED0[29U];
//volatile uint32_t ICER0;      			/*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
//volatile uint32_t ICER1;      			/*!< Offset: 0x084 (R/W)  Interrupt Clear Enable Register */
//volatile uint32_t ICER2;      			/*!< Offset: 0x088 (R/W)  Interrupt Clear Enable Register */
//uint32_t RESERVED1[29U];
//volatile uint32_t ISPR0;      			/*!< Offset: 0x100 (R/W)  Interrupt Set-Pending Register */
//volatile uint32_t ISPR1;      			/*!< Offset: 0x104 (R/W)  Interrupt Set-Pending Register */
//volatile uint32_t ISPR2;      			/*!< Offset: 0x108 (R/W)  Interrupt Set-Pending Register */
//uint32_t RESERVED2[29U];
//volatile uint32_t ICPR0;      			/*!< Offset: 0x180 (R/W)  Interrupt Clear-Pending Register */
//volatile uint32_t ICPR1;      			/*!< Offset: 0x184 (R/W)  Interrupt Clear-Pending Register */
//volatile uint32_t ICPR2;      			/*!< Offset: 0x188 (R/W)  Interrupt Clear-Pending Register */
//uint32_t RESERVED3[29U];
//volatile uint32_t IABR0;      			/*!< Offset: 0x200 (R/W)  Interrupt Active Bit Register */
//volatile uint32_t IABR1;      			/*!< Offset: 0x204 (R/W)  Interrupt Active Bit Register */
//volatile uint32_t IABR2;      			/*!< Offset: 0x208 (R/W)  Interrupt Active Bit Register */
//uint32_t RESERVED4[61U];
//NVIC_IP_Reg_t IPR0;						/*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR1;						/*!< Offset: 0x304 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR2;						/*!< Offset: 0x308 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR3;						/*!< Offset: 0x30C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR4;						/*!< Offset: 0x310 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR5;						/*!< Offset: 0x314 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR6;						/*!< Offset: 0x318 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR7;						/*!< Offset: 0x31C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR8;						/*!< Offset: 0x320 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR9;						/*!< Offset: 0x324 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR10;						/*!< Offset: 0x328 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR11;						/*!< Offset: 0x32C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR12;						/*!< Offset: 0x330 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR13;						/*!< Offset: 0x334 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR14;						/*!< Offset: 0x338 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR15;						/*!< Offset: 0x33C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR16;						/*!< Offset: 0x340 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR17;						/*!< Offset: 0x344 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR18;						/*!< Offset: 0x348 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR19;						/*!< Offset: 0x34C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR20;						/*!< Offset: 0x350 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR21;						/*!< Offset: 0x354 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR22;						/*!< Offset: 0x358 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR23;						/*!< Offset: 0x35C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR24;						/*!< Offset: 0x360 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR25;						/*!< Offset: 0x364 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR26;						/*!< Offset: 0x368 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR27;						/*!< Offset: 0x36C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR28;						/*!< Offset: 0x370 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR29;						/*!< Offset: 0x374 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR30;						/*!< Offset: 0x378 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR31;						/*!< Offset: 0x37C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR32;						/*!< Offset: 0x380 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR33;						/*!< Offset: 0x384 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR34;						/*!< Offset: 0x388 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR35;						/*!< Offset: 0x38C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR36;						/*!< Offset: 0x390 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR37;						/*!< Offset: 0x394 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR38;						/*!< Offset: 0x398 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR39;						/*!< Offset: 0x39C (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR40;						/*!< Offset: 0x3A0 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR41;						/*!< Offset: 0x3A4 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR42;						/*!< Offset: 0x3A8 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR43;						/*!< Offset: 0x3AC (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR44;						/*!< Offset: 0x3B0 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR45;						/*!< Offset: 0x3B4 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR46;						/*!< Offset: 0x3B8 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR47;						/*!< Offset: 0x3BC (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR48;						/*!< Offset: 0x3C0 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR49;						/*!< Offset: 0x3C4 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR50;						/*!< Offset: 0x3C8 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR51;						/*!< Offset: 0x3CC (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR52;						/*!< Offset: 0x3D0 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR53;						/*!< Offset: 0x3D4 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR54;						/*!< Offset: 0x3D8 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR55;						/*!< Offset: 0x3DC (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR56;						/*!< Offset: 0x3E0 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR57;						/*!< Offset: 0x3E4 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR58;						/*!< Offset: 0x3E8 (R/W)  Interrupt Priority Register (8Bit wide) */
//NVIC_IP_Reg_t IPR59;						/*!< Offset: 0x3EC (R/W)  Interrupt Priority Register (8Bit wide) */
//}NVIC_t;

/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for RCC_CR register  ********************/
typedef enum
{
	RCC_CR_HSION = 0x1U,						/*!< HSI oscillator ON                       												*/
	RCC_CR_HSIRDY = 0x1U,						/*!< HSI oscillator ready                       											*/
	RCC_CR_HSEON = 0x1U,						/*!< HSE oscillator ON                       												*/
	RCC_CR_HSERDY = 0x1U,						/*!< HSE oscillator ready                       											*/
	RCC_CR_HSEBYP = 0x1U,						/*!< HSE oscillator bypassed with an external clock                     					*/
	RCC_CR_CSSON = 0x1U,						/*!< Clock security system ON (Clock detector ON if HSE oscillator is stable, OFF if not)	*/
	RCC_CR_PLLON = 0x1U,						/*!< PLL ON                       															*/
	RCC_CR_PLLRDY = 0x1U,						/*!< PLL locked                       														*/
	RCC_CR_PLLI2SON = 0x1U,						/*!< PLLI2S ON                       														*/
	RCC_CR_PLLI2SRDY = 0x1U						/*!< PLLI2S locked                       													*/
}RCC_CR_Defs_t;

/********************  Bit definition for RCC_PLLCFGR register  ***************/
typedef enum
{
	RCC_PLLCFGR_PLLSRC_HSI,						/*!< HSI clock selected as PLL and PLLI2S clock entry										*/
	RCC_PLLCFGR_PLLSRC_HSE						/*!< HSE oscillator clock selected as PLL and PLLI2S clock entry							*/
}RCC_PLLCFGR_Defs_t;

/********************  Bit definition for RCC_CFGR register  ******************/
typedef enum
{
	RCC_CFGR_SW_HSI = 0x0U,                   	/*!< HSI selected as system clock 													*/
	RCC_CFGR_SW_HSE = 0x1U,                   	/*!< HSE selected as system clock 													*/
	RCC_CFGR_SW_PLL = 0x2U,                    	/*!< PLL selected as system clock 													*/

	RCC_CFGR_PPRE1_DIV1 = 0x0U,					/*!< HCLK not divided   															*/
	RCC_CFGR_PPRE1_DIV2 = 0x4U, 				/*!< HCLK divided by 2  															*/
	RCC_CFGR_PPRE1_DIV4 = 0x5U,					/*!< HCLK divided by 4  															*/
	RCC_CFGR_PPRE1_DIV8 = 0x6U, 				/*!< HCLK divided by 8  															*/
	RCC_CFGR_PPRE1_DIV16 = 0x7U, 				/*!< HCLK divided by 16 															*/


	RCC_CFGR_PPRE2_DIV1 = 0x0U,					/*!< HCLK not divided   															*/
	RCC_CFGR_PPRE2_DIV2 = 0x4U, 				/*!< HCLK divided by 2  															*/
	RCC_CFGR_PPRE2_DIV4 = 0x5U,					/*!< HCLK divided by 4  															*/
	RCC_CFGR_PPRE2_DIV8 = 0x6U, 				/*!< HCLK divided by 8  															*/
	RCC_CFGR_PPRE2_DIV16 = 0x7U, 				/*!< HCLK divided by 16 															*/

	RCC_CFGR_MCO1_HSI = 0x0U,					/*!< HSI clock selected 															*/
	RCC_CFGR_MCO1_LSE = 0x1U,					/*!< LSE oscillator selected 														*/
	RCC_CFGR_MCO1_HSE = 0x2U,					/*!< HSE oscillator clock selected 													*/
	RCC_CFGR_MCO1_PLL = 0x3U,					/*!< PLL clock selected 															*/

	RCC_CFGR_I2SSRC_PLLI2S = 0x0U,				/*!< PLLI2S clock used as I2S clock source 											*/
	RCC_CFGR_I2SSRC_EXT = 0x1U,					/*!< External clock mapped on the I2S_CKIN pin used as I2S clock source				*/


	RCC_CFGR_MCO1PRE_DIV2 = 0x4U,				/*!< division by 2 																	*/
	RCC_CFGR_MCO1PRE_DIV3 = 0x5U,				/*!< division by 3 																	*/
	RCC_CFGR_MCO1PRE_DIV4 = 0x6U,				/*!< division by 4 																	*/
	RCC_CFGR_MCO1PRE_DIV5 = 0x7U,				/*!< division by 5 																	*/

	RCC_CFGR_MCO2PRE_DIV2 = 0x4U,				/*!< division by 2 																	*/
	RCC_CFGR_MCO2PRE_DIV3 = 0x5U,				/*!< division by 3 																	*/
	RCC_CFGR_MCO2PRE_DIV4 = 0x6U,				/*!< division by 4 																	*/
	RCC_CFGR_MCO2PRE_DIV5 = 0x7U,				/*!< division by 5 																	*/

	RCC_CFGR_MCO2_SYSCLK = 0x0U,				/*!< System clock (SYSCLK) selected													*/
	RCC_CFGR_MCO2_PLLI2S = 0x1U,				/*!< PLLI2S clock selected															*/
	RCC_CFGR_MCO2_HSE 	 = 0x2U,				/*!< HSE oscillator clock selected													*/
	RCC_CFGR_MCO2_PLL 	 = 0x3U					/*!< PLL clock selected																*/
}RCC_CFGR_Defs_t;

/********************  Bit definition for RCC_AHB1RSTR register  **************/
typedef enum
{
	RCC_AHB1RSTR_GPIOARST = 0x1U,				/*!< IO port A reset ON																*/
	RCC_AHB1RSTR_GPIOBRST = 0x1U,				/*!< IO port B reset ON																*/
	RCC_AHB1RSTR_GPIOCRST = 0x1U,				/*!< IO port C reset ON																*/
	RCC_AHB1RSTR_GPIODRST = 0x1U,				/*!< IO port D reset ON																*/
	RCC_AHB1RSTR_GPIOERST = 0x1U,				/*!< IO port E reset ON																*/
	RCC_AHB1RSTR_GPIOFRST = 0x1U,				/*!< IO port F reset ON																*/
	RCC_AHB1RSTR_GPIOGRST = 0x1U,				/*!< IO port G reset ON																*/
	RCC_AHB1RSTR_GPIOHRST = 0x1U,				/*!< IO port H reset ON																*/
	RCC_AHB1RSTR_GPIOIRST = 0x1U,				/*!< IO port I reset ON																*/
	RCC_AHB1RSTR_CRCRST   = 0x1U,				/*!< CRC reset ON																	*/
	RCC_AHB1RSTR_DMA1RST  = 0x1U,				/*!< DMA1 reset ON																	*/
	RCC_AHB1RSTR_DMA2RST  = 0x1U,				/*!< DMA2 reset	ON																	*/
	RCC_AHB1RSTR_ETHMACRST = 0x1U,				/*!< Ethernet MAC reset	ON															*/
	RCC_AHB1RSTR_OTGHRST  = 0x1U				/*!< USB OTG HS module reset ON														*/
}RCC_AHB1RSTR_Defs_t;

/********************  Bit definition for RCC_AHB2RSTR register  **************/
typedef enum
{
	RCC_AHB2RSTR_DCMIRST = 0x1U,				/*!< Camera interface reset	ON														*/
	RCC_AHB2RSTR_RNGRST = 0x1U,					/*!< Random number generator module reset ON										*/
	RCC_AHB2RSTR_OTGFSRST = 0x1U,				/*!< USB OTG FS module reset ON														*/
	RCC_AHB3RSTR_FSMCRST = 0x1U					/*!< USB OTG FS module reset ON														*/
}RCC_AHB2RSTR_Defs_t;

/********************  Bit definition for RCC_APB1RSTR register  **************/
typedef enum
{
	RCC_APB1RSTR_TIM2RST = 0x1U,				/*!< TIM2 reset ON																	*/
	RCC_APB1RSTR_TIM3RST= 0x1U,					/*!< TIM3 reset ON																	*/
	RCC_APB1RSTR_TIM4RST= 0x1U,					/*!< TIM4 reset ON																	*/
	RCC_APB1RSTR_TIM5RST= 0x1U,					/*!< TIM5 reset ON																	*/
	RCC_APB1RSTR_TIM6RST= 0x1U,					/*!< TIM6 reset ON 																	*/
	RCC_APB1RSTR_TIM7RST= 0x1U,					/*!< TIM7 reset ON 																	*/
	RCC_APB1RSTR_TIM12RST= 0x1U,				/*!< TIM12 reset ON 																*/
	RCC_APB1RSTR_TIM13RST= 0x1U,				/*!< TIM13 reset ON																	*/
	RCC_APB1RSTR_TIM14RST= 0x1U,				/*!< TIM14 reset ON 																*/
	RCC_APB1RSTR_WWDGRST= 0x1U,					/*!< Window watchdog reset ON														*/
	RCC_APB1RSTR_SPI2RST= 0x1U,					/*!< SPI2 reset ON			 														*/
	RCC_APB1RSTR_SPI3RST= 0x1U,					/*!< SPI3 reset ON			 														*/
	RCC_APB1RSTR_USART2RST= 0x1U,				/*!< USART2 reset ON		 														*/
	RCC_APB1RSTR_USART3RST= 0x1U,				/*!< USART3 reset ON		 														*/
	RCC_APB1RSTR_UART4RST= 0x1U,				/*!< UART4 reset ON		 															*/
	RCC_APB1RSTR_UART5RST= 0x1U,				/*!< UART5 reset ON		 															*/
	RCC_APB1RSTR_I2C1RST= 0x1U,					/*!< I2C1 reset	ON																	*/
	RCC_APB1RSTR_I2C2RST= 0x1U,					/*!< I2C2 reset	ON																	*/
	RCC_APB1RSTR_I2C3RST= 0x1U,					/*!< I2C3 reset	ON																	*/
	RCC_APB1RSTR_CAN1RST= 0x1U,					/*!< CAN1 reset ON 																	*/
	RCC_APB1RSTR_CAN2RST= 0x1U,					/*!< CAN2 reset ON 																	*/
	RCC_APB1RSTR_PWRRST= 0x1U,					/*!< Power interface reset ON 														*/
	RCC_APB1RSTR_DACRST = 0x1U					/*!< DAC reset ON																	*/
}RCC_APB1RSTR_Defs_t;

/********************  Bit definition for RCC_APB2RSTR register  **************/
typedef enum
{
	RCC_APB2RSTR_TIM1RST = 0x1U,				/*!< TIM1 reset ON			 														*/
	RCC_APB2RSTR_TIM8RST= 0x1U,					/*!< TIM8 reset ON			 														*/
	RCC_APB2RSTR_USART1RST= 0x1U,				/*!< USART1 reset ON			 													*/
	RCC_APB2RSTR_USART6RST= 0x1U,				/*!< USART6 reset ON			 													*/
	RCC_APB2RSTR_ADCRST= 0x1U,					/*!< ADC reset ON			 														*/
	RCC_APB2RSTR_SDIORST= 0x1U,					/*!< SDIO reset ON			 														*/
	RCC_APB2RSTR_SPI1RST= 0x1U,					/*!< SPI1 reset ON			 														*/
	RCC_APB2RSTR_SYSCFGRST= 0x1U,				/*!< SYSCFG reset ON			 													*/
	RCC_APB2RSTR_TIM9RST= 0x1U,					/*!< TIM9 reset ON			 														*/
	RCC_APB2RSTR_TIM10RST= 0x1U,				/*!< TIM10 reset ON			 														*/
	RCC_APB2RSTR_TIM11RST = 0x1U				/*!< TIM11 reset ON			 														*/
}RCC_APB2RSTR_Defs_t;


/********************  Bit definition for RCC_AHB1ENR register  ***************/
typedef enum
{
	RCC_AHB1ENR_GPIOAEN = 0x1U,					/*!< IO port A clock enable 														*/
	RCC_AHB1ENR_GPIOBEN = 0x1U,					/*!< IO port B clock enable 														*/
	RCC_AHB1ENR_GPIOCEN= 0x1U,					/*!< IO port C clock enable 														*/
	RCC_AHB1ENR_GPIODEN= 0x1U,					/*!< IO port D clock enable 														*/
	RCC_AHB1ENR_GPIOEEN= 0x1U,					/*!< IO port E clock enable 														*/
	RCC_AHB1ENR_GPIOFEN= 0x1U,					/*!< IO port F clock enable 														*/
	RCC_AHB1ENR_GPIOGEN= 0x1U,					/*!< IO port G clock enable 														*/
	RCC_AHB1ENR_GPIOHEN= 0x1U,					/*!< IO port H clock enable 														*/
	RCC_AHB1ENR_GPIOIEN= 0x1U,					/*!< IO port I clock enable 														*/
	RCC_AHB1ENR_CRCEN= 0x1U,					/*!< CRC clock enable		 														*/
	RCC_AHB1ENR_BKPSRAMEN= 0x1U,				/*!< Backup SRAM interface clock enable 											*/
	RCC_AHB1ENR_CCMDATARAMEN= 0x1U,				/*!< CCM data RAM clock enable 														*/
	RCC_AHB1ENR_DMA1EN = 0x1U,					/*!< DMA1 clock enable																*/
	RCC_AHB1ENR_DMA2EN= 0x1U,					/*!< DMA2 clock enable 																*/
	RCC_AHB1ENR_ETHMACEN= 0x1U,					/*!< Ethernet MAC clock enable														*/
	RCC_AHB1ENR_ETHMACTXEN= 0x1U,				/*!< Ethernet Transmission clock enable 											*/
	RCC_AHB1ENR_ETHMACRXEN= 0x1U,				/*!< Ethernet Reception clock enable 												*/
	RCC_AHB1ENR_ETHMACPTPEN= 0x1U,				/*!< Ethernet PTP clock enable 														*/
	RCC_AHB1ENR_OTGHSEN= 0x1U,					/*!< USB OTG HS clock enable 														*/
	RCC_AHB1ENR_OTGHSULPIEN = 0x1U				/*!< USB OTG HSULPI clock enable 													*/
}RCC_AHB1ENR_Defs_t;

/********************  Bit definition for RCC_AHB2ENR register  ***************/
typedef enum
{
	RCC_AHB2ENR_DCMIEN = 0x1U,					/*!< Camera interface enable 														*/
	RCC_AHB2ENR_RNGEN = 0x1U,					/*!< Random number generator clock enable 											*/
	RCC_AHB2ENR_OTGFSEN = 0x1U,					/*!< USB OTG FS clock enable 														*/
	RCC_AHB3ENR_FSMCEN = 0x1U					/*!< Flexible memory controller module clock enable 								*/
}RCC_AHB2ENR_Defs_t;

/********************  Bit definition for RCC_APB1ENR register  ***************/
typedef enum
{
	RCC_APB1ENR_TIM2EN = 0x1U,					/*!< TIM2 clock enable 																*/
	RCC_APB1ENR_TIM3EN = 0x1U,					/*!< TIM3 clock enable 																*/
	RCC_APB1ENR_TIM4EN = 0x1U,					/*!< TIM4 clock enable 																*/
	RCC_APB1ENR_TIM5EN = 0x1U,					/*!< TIM5 clock enable 																*/
	RCC_APB1ENR_TIM6EN = 0x1U,					/*!< TIM6 clock enable 																*/
	RCC_APB1ENR_TIM7EN = 0x1U,					/*!< TIM7 clock enable 																*/
	RCC_APB1ENR_TIM12EN = 0x1U,					/*!< TIM12 clock enable 															*/
	RCC_APB1ENR_TIM13EN = 0x1U,					/*!< TIM13 clock enable 															*/
	RCC_APB1ENR_TIM14EN = 0x1U,					/*!< TIM14 clock enable 															*/
	RCC_APB1ENR_WWDGEN = 0x1U,					/*!< Window watchdog clock enable 													*/
	RCC_APB1ENR_SPI2EN = 0x1U,					/*!< SPI2 clock enable																*/
	RCC_APB1ENR_SPI3EN = 0x1U,					/*!< SPI3 clock enable 																*/
	RCC_APB1ENR_USART2EN = 0x1U, 				/*!< USART2 clock enable 															*/
	RCC_APB1ENR_USART3EN = 0x1U,				/*!< USART3 clock enable															*/
	RCC_APB1ENR_UART4EN = 0x1U,					/*!< UART4 clock enable																*/
	RCC_APB1ENR_UART5EN = 0x1U,					/*!< UART5 clock enable																*/
	RCC_APB1ENR_I2C1EN = 0x1U,					/*!< I2C1 clock enable 																*/
	RCC_APB1ENR_I2C2EN = 0x1U,					/*!< I2C2 clock enable 																*/
	RCC_APB1ENR_I2C3EN = 0x1U,					/*!< I2C3 clock enable 																*/
	RCC_APB1ENR_CAN1EN = 0x1U,					/*!< CAN 1 clock enable 															*/
	RCC_APB1ENR_CAN2EN = 0x1U,					/*!< CAN 2 clock enable 															*/
	RCC_APB1ENR_PWREN = 0x1U,					/*!< I2C1 clock enable																*/
	RCC_APB1ENR_DACEN = 0x1U					/*!< DAC interface clock enable 													*/
}RCC_APB1ENR_Defs_t;

/********************  Bit definition for RCC_APB2ENR register  ***************/
typedef enum
{
	RCC_APB2ENR_TIM1EN = 0x1U,					/*!< TIM2 clock enable 																*/
	RCC_APB2ENR_TIM8EN = 0x1U,					/*!< TIM8 clock enable 																*/
	RCC_APB2ENR_USART1EN = 0x1U,				/*!< USART1 clock enable 															*/
	RCC_APB2ENR_USART6EN = 0x1U,				/*!< USART6 clock enable															*/
	RCC_APB2ENR_ADC1EN = 0x1U,					/*!< ADC1 clock enable 																*/
	RCC_APB2ENR_ADC2EN = 0x1U,					/*!< ADC2 clock enable 																*/
	RCC_APB2ENR_ADC3EN = 0x1U,					/*!< ADC3 clock enable 																*/
	RCC_APB2ENR_SDIOEN = 0x1U,					/*!< SDIO clock enable 																*/
	RCC_APB2ENR_SPI1EN = 0x1U,					/*!< SPI1 clock enable 																*/
	RCC_APB2ENR_SYSCFGEN = 0x1U,				/*!< System configuration controller clock enable									*/
	RCC_APB2ENR_TIM9EN = 0x1U,					/*!< TIM9 clock enable 																*/
	RCC_APB2ENR_TIM10EN = 0x1U,					/*!< TIM10 clock enable 															*/
	RCC_APB2ENR_TIM11EN = 0x1U					/*!< TIM11 clock enable 															*/
}RCC_APB2ENR_Defs_t;

/********************  Bit definition for RCC_BDCR register  ******************/
typedef enum
{
	RCC_BDCR_RTCSEL_NOCLOCK,					/*!< No clock 																		*/
	RCC_BDCR_RTCSEL_LSE,						/*!< LSE oscillator clock used as the RTC clock 									*/
	RCC_BDCR_RTCSEL_LSI,						/*!< LSI oscillator clock used as the RTC clock 									*/
	RCC_BDCR_RTCSEL_HSE							/*!< HSE oscillator clock divided by a programmable prescaler						*/
}RCC_BDCR_Defs_t;

/********************  Bit definition for RCC_SSCGR register  *****************/
typedef enum
{
	RCC_BDCR_SSCGR_CENTER,						/*!< Center spread. To write before to set CR[24]=PLLON bit.						*/
	RCC_BDCR_SSCGR_DOWN							/*!< Down spread. To write before to set CR[24]=PLLON bit.							*/
}RCC_SSCGR_Defs_t;

#include <core_cm4.h>



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F407xx_H */
