/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_cortex.h"
#include "image.h"
#include "ring_buffer.h"
#include "uart_transport.h"

/* Private variables ---------------------------------------------------------*/
// Image header definition
__attribute__((section(".image_hdr"))) ImageHeader_t IMAGE_HEADER = {
  .image_magic = IMAGE_MAGIC_APP,
  .image_hdr_version = IMAGE_VERSION_CURRENT,
  .image_type = IMAGE_TYPE_APP,
  .version_major = 1,
  .version_minor = 0,
  .version_patch = 0,
  ._padding = 0,
  .vector_addr = 0x08020200,
  .crc = 0,
  .data_size = 0
};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void setup_leds(void);
static void clear_screen(void);
static void delay_ms(uint32_t ms);
static char* itoa(uint32_t value);

/* Animation frames for serial terminal */
static const char* FRAMES[] = {
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  | o              |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |    o           |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |       o        |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |          o     |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |             o  |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |               o|  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |            o   |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |        o       |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|  |     o          |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",
    
    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  | o              |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n",

    "+----------------------+\r\n"
    "|                      |\r\n"
    "|  |                |  |\r\n"
    "|  |o               |  |\r\n"
    "|  |                |  |\r\n"
    "|  |                |  |\r\n"
    "|                      |\r\n"
    "+----------------------+\r\n"
};

#define FRAMES_COUNT (sizeof(FRAMES) / sizeof(FRAMES[0]))

/* Animation banner */
static const char* BOOT_BANNER = "\r\n"
"\x1B[36mxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n"
"xxxxxxxx  xxxxxxxxxxxxxxxxxxxx  xxxxxxxxx\r\n"
"xxxxxxxxxx  xxxxxxxxxxxxxxxxx  xxxxxxxxxx\r\n"
"xxxxxx  xxx  xxxxxxxxxxxxxxx  xx   xxxxxx\r\n"
"xxxxxxxx  xx  xxxxxxxxxxxxx  xx  xxxxxxxx\r\n"
"xxxx  xxx   xxxxxxxxxxxxxxxxx  xxx  xxxxx\r\n"
"xxxxxx    xxxx  xxxxxxxx  xxx     xxxxxxx\r\n"
"xxxxxxxx xxxxx xx      xx xxxx  xxxxxxxxx\r\n"
"xxxx     xxxxx   xx  xx   xxxxx     xxxxx\r\n"
"xxxxxxxx xxxxxxxxxx  xxxxxxxxxx  xxxxxxxx\r\n"
"xxxxx    xxxxxx  xx  xx  xxxxxx    xxxxxx\r\n"
"xxxxxxxx  xxxx xxxx  xxxx xxxxx xxxxxxxxx\r\n"
"xxxxxxx    xxx  xxx  xxx  xxx    xxxxxxxx\r\n"
"xxxxxxxxxx   xxxxxx  xxxxxx   xxxxxxxxxxx\r\n"
"xxxxxxxxxxxxxx             xxxxxxxxxxxxxx\r\n"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n\x1B[0m\r\n"
"\x1B[33m-- Firmware v1.0.0 --\x1B[0m\r\n"
"\r\n";

/* Transport variables */
static UARTTransport_Config_t uart_config;
static Transport_t uart_transport;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* External symbol from the linker script for firmware size */
    extern uint32_t __firmware_size;
    uint32_t firmware_size = (uint32_t)&__firmware_size;
    
    /* Update firmware size in header */
    IMAGE_HEADER.data_size = firmware_size;
    
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();
    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    setup_leds();
    
    /* Configure UART */
    uart_config.usart = USART2;
    uart_config.baudrate = 115200;
    uart_config.timeout = 1000;
    uart_config.use_xmodem = 0;
    
    /* Initialize transport */
    transport_init(&uart_transport, TRANSPORT_UART, &uart_config);
    
    /* Enable USART2 interrupt in NVIC */
    NVIC_EnableIRQ(USART2_IRQn);
    
    /* Clear the screen */
    clear_screen();
    
    /* Print banner */
    transport_send(&uart_transport, (const uint8_t*)BOOT_BANNER, strlen(BOOT_BANNER));
    delay_ms(2000);
    
    /* Animation loop variables */
    uint8_t frame_index = 0;
    uint8_t led_index = 0;
    uint32_t last_update = HAL_GetTick();
    uint32_t last_led_toggle = HAL_GetTick();
    char buffer[256];
    
    /* Main loop */
    while (1) {
        /* Process UART data */
        transport_process(&uart_transport);
        
        /* Get current time */
        uint32_t current_time = HAL_GetTick();
        
        /* Update animation every 200 ms */
        if (current_time - last_update >= 200) {
            /* Clear screen and reset cursor position */
            transport_send(&uart_transport, (const uint8_t*)"\x1B[2J\x1B[1;1H", 10);
            
            /* Send colored title */
            transport_send(&uart_transport, (const uint8_t*)"\x1B[96mPONG ANIMATION\x1B[0m\r\n\r\n", 25);
            
            /* Send current frame in blue color */
            transport_send(&uart_transport, (const uint8_t*)"\x1B[34m", 5);
            transport_send(&uart_transport, (const uint8_t*)FRAMES[frame_index], strlen(FRAMES[frame_index]));
            transport_send(&uart_transport, (const uint8_t*)"\x1B[0m", 4);
            
            /* Additional info */
            sprintf(buffer, "\r\n\x1B[91mFirmware v1.0.0 - System uptime: %u sec\x1B[0m\r\n", current_time / 1000);
            transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
            
            /* Move to next frame */
            frame_index = (frame_index + 1) % FRAMES_COUNT;
            last_update = current_time;
        }
        
        /* Blink LEDs for 100 ms each */
        if (current_time - last_led_toggle >= 100) {
            /* Turn off all LEDs */
            for (int i = 0; i < 4; i++) {
                LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12 << i);
            }
            
            /* Enable next LED in sequence */
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12 << led_index);
            
            /* Move to next LED */
            led_index = (led_index + 1) % 4;
            last_led_toggle = current_time;
        }
    }
}

/**
  * @brief  Clears the screen using ANSI escape codes
  * @retval None
  */
static void clear_screen(void) {
    transport_send(&uart_transport, (const uint8_t*)"\x1B[2J\x1B[1;1H", 10);
    HAL_Delay(10);
}

/**
  * @brief  Delays execution for a number of milliseconds
  * @param  ms: delay in milliseconds
  * @retval None
  */
static void delay_ms(uint32_t ms) {
    uint32_t start = HAL_GetTick();
    while (HAL_GetTick() - start < ms) {
        __NOP();
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators according to the specified parameters */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 90;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* Initialize the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief Initialize GPIO pins for UART
  * @retval None
  */
static void MX_GPIO_Init(void) {
    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    
    /* Configure PA2 (TX) and PA3 (RX) for USART2 */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief Initialize LED pins
  * @retval None
  */
static void setup_leds(void) {
    /* Configure PD12-PD15 as outputs for LEDs */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* Initialize all LEDs to off state */
    LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
        /* Toggle red LED (PD14) to indicate error */
        LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_14);
        HAL_Delay(200);
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* User can add his own implementation to report the file name and line number */
    while(1);
}
#endif /* USE_FULL_ASSERT */