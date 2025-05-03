#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_cortex.h"
#include "bootloader.h"
#include "flash.h"
#include "image.h"
#include "uart_transport.h"
#include "crc.h"
#include "ring_buffer.h"

/* Private define ------------------------------------------------------------*/
#define BOOT_TIMEOUT_MS         10000

/* Private variables ---------------------------------------------------------*/
__attribute__((section(".image_hdr"))) const ImageHeader_t IMAGE_HEADER = {
  .image_magic = IMAGE_MAGIC_LOADER,
  .image_hdr_version = IMAGE_VERSION_CURRENT,
  .image_type = IMAGE_TYPE_LOADER,
  .is_patch = 0,
  .version_major = 1,
  .version_minor = 0,
  .version_patch = 0,
  ._padding = 0,
  .vector_addr = 0x08004200,
  .crc = 0,
  .data_size = 0
};

/* Global variables ----------------------------------------------------------*/
static UARTTransport_Config_t uart_config;
static Transport_t uart_transport;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void clear_screen(void);
static void display_menu(void);
static void clear_rx_buffer(void);

// Loader banner - kept green
const char* BOOT_BANNER = "\r\n\
\x1B[37m\
00000000  \x1B[92m\x1B[42m57\x1B[0m \x1B[37m45 \x1B[92m\x1B[42m4C\x1B[0m \x1B[37m43 \x1B[92m\x1B[42m4F 4D\x1B[0m \x1B[37m45 \x1B[92m\x1B[42m20\x1B[0m  \x1B[37m54 4F \x1B[92m\x1B[42m20\x1B[0m \x1B[37m44 45 \x1B[92m\x1B[42m53 54 52\x1B[0m  \x1B[37m\r\n\
00000010  \x1B[92m\x1B[42m4F\x1B[0m \x1B[37m43 \x1B[92m\x1B[42m4F\x1B[0m \x1B[37m52 \x1B[92m\x1B[42m45\x1B[0m \x1B[37m27 53 \x1B[92m\x1B[42m20\x1B[0m  \x1B[37m43 55 \x1B[92m\x1B[42m53\x1B[0m \x1B[37m54 4F \x1B[92m\x1B[42m4D\x1B[0m \x1B[37m20 \x1B[92m\x1B[42m58\x1B[0m  \x1B[37m\r\n\
00000020  \x1B[92m\x1B[42m4D 4F 44\x1B[0m \x1B[37m45 \x1B[92m\x1B[42m4D 20\x1B[0m \x1B[37m42 \x1B[92m\x1B[42m4F\x1B[0m  \x1B[37m4F 54 \x1B[92m\x1B[42m4C\x1B[0m \x1B[37m4F 41 \x1B[92m\x1B[42m44\x1B[0m \x1B[37m45 \x1B[92m\x1B[42m52\x1B[0m  \x1B[37m\r\n\
00000030  \x1B[92m\x1B[42m50\x1B[0m \x1B[37m4C \x1B[92m\x1B[42m45\x1B[0m \x1B[37m41 \x1B[92m\x1B[42m53\x1B[0m \x1B[37m45 20 \x1B[92m\x1B[42m53\x1B[0m  \x1B[37m45 4C \x1B[92m\x1B[42m45\x1B[0m \x1B[37m43 54 \x1B[92m\x1B[42m20\x1B[0m \x1B[37m41 \x1B[92m\x1B[42m20\x1B[0m  \x1B[37m\r\n\
00000040  \x1B[92m\x1B[42m42\x1B[0m \x1B[37m4F \x1B[92m\x1B[42m4F\x1B[0m \x1B[37m54 \x1B[92m\x1B[42m20 4F\x1B[0m \x1B[37m50 \x1B[92m\x1B[42m54  49\x1B[0m \x1B[37m4F \x1B[92m\x1B[42m4E 20\x1B[0m \x1B[37m46 \x1B[92m\x1B[42m4F 52 21\x1B[0m  \r\n\x1B[0m\r\n";

const char* BOOT_OPTIONS_STR = "\x1B[92mPress \x1B[93m'Enter'\x1B[0m\x1B[92m to boot application\r\n\
Press \x1B[93m'I'\x1B[0m\x1B[92m to get information about system state\r\n\
Press \x1B[93m'U'\x1B[0m\x1B[92m to enter updater\x1B[0m\r\n";

/**
  * @brief Clears the terminal screen using ANSI escape codes
  */
static void clear_screen(void) {
    transport_send(&uart_transport, (const uint8_t*)"\x1B[2J\x1B[1;1H", 10);
    HAL_Delay(10);
}

/**
  * @brief Display the main menu on the terminal
  */
static void display_menu(void) {
    char version_str[20];
    clear_screen();
    
    // Send banner
    transport_send(&uart_transport, (const uint8_t*)BOOT_BANNER, strlen(BOOT_BANNER));

    // Send version info
    transport_send(&uart_transport, (const uint8_t*)"\x1B[96m-- Loader v ", 15);
    sprintf(version_str, "%d.%d.%d", IMAGE_HEADER.version_major, 
            IMAGE_HEADER.version_minor, IMAGE_HEADER.version_patch);
    transport_send(&uart_transport, (const uint8_t*)version_str, strlen(version_str));
    transport_send(&uart_transport, (const uint8_t*)" --\x1B[0m\r\n\r\n", 11);
    
    // Send menu options
    transport_send(&uart_transport, (const uint8_t*)BOOT_OPTIONS_STR, strlen(BOOT_OPTIONS_STR));
    transport_send(&uart_transport, (const uint8_t*)"\x1B[96mWill boot automatically in 10 seconds\x1B[0m\r\n", 52);
}

/**
  * @brief UART RX buffer dummy read function
  */
static void clear_rx_buffer(void) {
    uint8_t dummy;
    while (transport_receive(&uart_transport, &dummy, 1) > 0) {
        // Read until buffer is empty
    }
}

/**
  * @brief Toggle LED
  * @param led_pin LED pin number (0-3)
  */
static void toggle_led(uint8_t led_pin) {
    uint32_t pin;
    switch (led_pin) {
        case 0: { // Green
            pin = LL_GPIO_PIN_12; 
            break;
        } 
        case 1: { // Orange
            pin = LL_GPIO_PIN_13; 
            break;
        }
        case 2: { // Red
            pin = LL_GPIO_PIN_14; 
            break;
        }
        case 3: { // Blue
            pin = LL_GPIO_PIN_15; 
            break;
        }
        default: {
            return;
        }
    }
    
    LL_GPIO_TogglePin(GPIOD, pin);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    
    // Create bootloader configuration
    BootConfig_t boot_config = {
        .app_addr = APP_ADDR,
        .updater_addr = UPDATER_ADDR,
        .loader_addr = LOADER_ADDR,
        .image_hdr_size = IMAGE_HDR_SIZE
    };
    
    // Configure UART transport
    uart_config.usart = USART2;
    uart_config.baudrate = 115200;
    uart_config.timeout = 1000;
    uart_config.app_addr = APP_ADDR;
    uart_config.updater_addr = UPDATER_ADDR;
    uart_config.loader_addr = LOADER_ADDR;
    uart_config.image_hdr_size = IMAGE_HDR_SIZE;
    
    // Initialize transport
    transport_init(&uart_transport, TRANSPORT_UART, &uart_config);
    
    // Initialize CRC module
    crc_init();
    
    // Initial clear for RX buffer
    clear_rx_buffer();
    
    // Display menu
    display_menu();
    
    // Enable USART2 irq
    NVIC_EnableIRQ(USART2_IRQn);
    
    // Main variables
    BootOption_t boot_option = BOOT_OPTION_NONE;
    uint32_t led_toggle_time = HAL_GetTick();
    uint32_t autoboot_timer = HAL_GetTick();
    
    while (1) {
        // Process UART data
        transport_process(&uart_transport);
        
        // Blink green LED to indicate system is alive
        uint32_t current_time = HAL_GetTick();
        if (current_time - led_toggle_time >= 500) {
            toggle_led(0);
            led_toggle_time = current_time;
        }
        
        // Process user input
        uint8_t byte;
        if (transport_receive(&uart_transport, &byte, 1) > 0) {
            // Reset autoboot timer on any key press
            autoboot_timer = current_time;
            
            switch (byte) {
                case 'U':
                case 'u': {
                    // Boot to updater
                    if (is_firmware_valid(UPDATER_ADDR, &boot_config)) {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[36m\r\n Booting updater...\x1B[0m\r\n", 33);
                        boot_option = BOOT_OPTION_UPDATER;
                    } else {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\nValid updater not found!\x1B[0m\r\n", 38);
                        HAL_Delay(1500);
                        display_menu();
                    }
                    break;
                }
                    
                case 'I':
                case 'i': {
                    // Show system info
                    clear_screen();
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\n--- System Information ---\x1B[0m\r\n\r\n", 43);
                    
                    // Loader info
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[96mLoader (this image) : \x1B[0m", 32);
                    
                    ImageHeader_t header;
                    if (get_firmware_header(LOADER_ADDR, &boot_config, &header)) {
                        char buffer[256];
                        
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[32mValid\x1B[0m\r\n", 16);
                        
                        sprintf(buffer, "\x1B[92m  Version: \x1B[93m%d.%d.%d\x1B[0m\r\n", 
                                header.version_major, header.version_minor, header.version_patch);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Vector table: \x1B[93m0x%08lX\x1B[0m\r\n", header.vector_addr);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Data size: \x1B[93m%u bytes\x1B[0m\r\n", header.data_size);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  CRC: \x1B[93m0x%08lX\x1B[0m\r\n", header.crc);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Is Patch: \x1B[93m%s\x1B[0m\r\n", header.is_patch ? "Yes" : "No");
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Address: \x1B[93m0x%08lX\x1B[0m\r\n", LOADER_ADDR);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                    } else {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31mInvalid or Not Found\x1B[0m\r\n", 33);
                    }
                    
                    transport_send(&uart_transport, (const uint8_t*)"\r\n", 2);
                    
                    // Application info
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[96mApplication: \x1B[0m", 24);
                    
                    if (get_firmware_header(APP_ADDR, &boot_config, &header)) {
                        char buffer[256];
                        
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[32mValid\x1B[0m\r\n", 16);
                        
                        sprintf(buffer, "\x1B[92m  Version: \x1B[93m%d.%d.%d\x1B[0m\r\n", 
                                header.version_major, header.version_minor, header.version_patch);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Vector table: \x1B[93m0x%08lX\x1B[0m\r\n", header.vector_addr);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Data size: \x1B[93m%u bytes\x1B[0m\r\n", header.data_size);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  CRC: \x1B[93m0x%08lX\x1B[0m\r\n", header.crc);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Is Patch: \x1B[93m%s\x1B[0m\r\n", header.is_patch ? "Yes" : "No");
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Address: \x1B[93m0x%08lX\x1B[0m\r\n", APP_ADDR);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                    } else {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31mInvalid or Not Found\x1B[0m\r\n", 33);
                    }
                    
                    transport_send(&uart_transport, (const uint8_t*)"\r\n", 2);
                    
                    // Updater info
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[96mUpdater: \x1B[0m", 21);
                    
                    if (get_firmware_header(UPDATER_ADDR, &boot_config, &header)) {
                        char buffer[256];
                        
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[32mValid\x1B[0m\r\n", 16);
                        
                        sprintf(buffer, "\x1B[92m  Version: \x1B[93m%d.%d.%d\x1B[0m\r\n", 
                                header.version_major, header.version_minor, header.version_patch);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Vector table: \x1B[93m0x%08lX\x1B[0m\r\n", header.vector_addr);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Data size: \x1B[93m%u bytes\x1B[0m\r\n", header.data_size);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  CRC: \x1B[93m0x%08lX\x1B[0m\r\n", header.crc);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Is Patch: \x1B[93m%s\x1B[0m\r\n", header.is_patch ? "Yes" : "No");
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        sprintf(buffer, "\x1B[92m  Address: \x1B[93m0x%08lX\x1B[0m\r\n", UPDATER_ADDR);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                    } else {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31mInvalid or Not Found\x1B[0m\r\n", 33);
                    }
                    
                    transport_send(&uart_transport, (const uint8_t*)"\r\n", 2);
                    
                    // System info
                    char buffer[256];
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[96mSystem Info:\x1B[0m\r\n", 22);
                    
                    sprintf(buffer, "\x1B[92m  Boot timeout: \x1B[93m%u seconds\r\n", BOOT_TIMEOUT_MS / 1000);
                    transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                    
                    sprintf(buffer, "\x1B[92m  System uptime: \x1B[93m%u seconds\x1B[0m\r\n", HAL_GetTick() / 1000);
                    transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                    
                    transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[91mPress \x1B[31m'Esc'\x1B[0m \x1B[91mto return to menu...\x1B[0m\r\n", 59);
                    
                    uint8_t key;
                    while(1) {
                        transport_process(&uart_transport);
                        if (transport_receive(&uart_transport, &key, 1) > 0) {
                            if (key == 0x1B) { // ESC key
                                autoboot_timer = HAL_GetTick();
                                break;
                            }
                            autoboot_timer = HAL_GetTick();
                        }
                    }
                    
                    display_menu();
                    break;
                }
                    
                case '\r':
                case '\n': {
                    if (is_firmware_valid(APP_ADDR, &boot_config)) {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[92m\r\n Booting application...\x1B[0m\r\n", 36);
                        boot_option = BOOT_OPTION_APPLICATION;
                    } else {
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\nValid application not found!\x1B[0m\r\n", 41);
                        HAL_Delay(1500);
                        display_menu();
                    }
                    break;
                }
                    
                default: {
                    // ESC key
                    if (byte == 0x1B) {
                        clear_screen();
                        display_menu();
                    }
                    break;
                }
            }
        }
        
        // Handle boot options
        switch (boot_option) {
            case BOOT_OPTION_APPLICATION: {
                if (is_firmware_valid(APP_ADDR, &boot_config)) {
                    // Wait for UART to finish
                    while (!uart_transport_is_tx_complete()) {
                        transport_process(&uart_transport);
                    }
                    clear_rx_buffer();
                    
                    // Boot to app
                    boot_application(&boot_config);
                } else {
                    clear_screen();
                    transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mApplication validation failed just before boot\x1B[0m\r\n", 62);
                    boot_option = BOOT_OPTION_NONE;
                    HAL_Delay(1500);
                    display_menu();
                }
                break;
            }
                
            case BOOT_OPTION_UPDATER: {
                // Wait for UART to finish
                while (!uart_transport_is_tx_complete()) {
                    transport_process(&uart_transport);
                }
                clear_rx_buffer();
                
                // Boot to updater
                boot_updater(&boot_config);
                break;
            }
                
            default: {
                // Nothing to do here
                break;
            }
        }
        
        // Check for timeout
        if (boot_option == BOOT_OPTION_NONE) {
            uint32_t check_time = HAL_GetTick();
            if (check_time - autoboot_timer >= BOOT_TIMEOUT_MS) {
                if (is_firmware_valid(APP_ADDR, &boot_config)) {
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[93m\r\n Auto-boot timeout reached. Booting application...\x1B[0m\r\n", 69);
                    
                    // Wait for UART to finish
                    while (!uart_transport_is_tx_complete()) {
                        transport_process(&uart_transport);
                    }
                    clear_rx_buffer();
                    
                    boot_option = BOOT_OPTION_APPLICATION;
                } else {
                    transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\n Auto-boot timeout reached but valid application not found!\x1B[0m\r\n", 74);
                    
                    // Reset autoboot timer
                    autoboot_timer = current_time;
                    HAL_Delay(1500);
                    display_menu();
                }
            }
        }
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

    /* Initialize the CPU, AHB and APB bus clocks */
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    
    // on-board LEDs
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // Turn off all LEDs initially
    LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
        /* Toggle red LED to indicate error */
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
    /* For example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    while(1);
}
#endif /* USE_FULL_ASSERT */