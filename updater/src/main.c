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
#include "xmodem.h"
#include "flash.h"
#include "image.h"
#include "uart_transport.h"
#include "crc.h"
#include "ring_buffer.h"
#include "delta_update.h"

/* Private typedef -----------------------------------------------------------*/
// State for XMODEM recovery after transfer complete
typedef enum {
    POST_XMODEM_INITIAL,
    POST_XMODEM_RECOVERING,
    POST_XMODEM_COMPLETE
} PostXmodemState_t;

/* Private define ------------------------------------------------------------*/
// Time to block Enter key after XMODEM transfer (for ExtraPuTTY compatibility)
#define ENTER_BLOCK_AFTER_UPDATE_MS  3000

/* Private variables ---------------------------------------------------------*/
__attribute__((section(".image_hdr"))) const ImageHeader_t IMAGE_HEADER = {
  .image_magic = IMAGE_MAGIC_UPDATER,
  .image_hdr_version = IMAGE_VERSION_CURRENT,
  .image_type = IMAGE_TYPE_UPDATER,
  .is_patch = 0,
  .version_major = 1,
  .version_minor = 0,
  .version_patch = 0,
  ._padding = 0,
  .vector_addr = 0x08010200,
  .crc = 0,
  .data_size = 0
};

/* Global variables ----------------------------------------------------------*/
static uint32_t ENTER_BLOCKED_UNTIL = 0;
static UARTTransport_Config_t uart_config;
static Transport_t uart_transport;
static XmodemConfig_t xmodem_config;
static XmodemManager_t xmodem_manager;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void clear_screen(void);
static void display_menu(void);
static void clear_rx_buffer(void);
static uint32_t recover_from_xmodem(void);
static int is_enter_blocked(uint32_t current_time);
static void block_enter_temporarily(uint32_t current_time);
static void send_cancel_sequence(void);

// Updater banner - orange colored
const char* BOOT_BANNER = "\r\n\
\x1B[94m                       \r\n\
00000000  \x1B[92m\x1B[42m57\x1B[0m \x1B[94m45 \x1B[92m\x1B[42m4C\x1B[0m \x1B[94m43 \x1B[92m\x1B[42m4F 4D\x1B[0m \x1B[94m45 \x1B[92m\x1B[42m20\x1B[0m  \x1B[94m54 4F \x1B[92m\x1B[42m20\x1B[0m \x1B[94m44 45 \x1B[92m\x1B[42m53 54 52\x1B[0m  \x1B[94m\r\n\
00000010  \x1B[92m\x1B[42m4F\x1B[0m \x1B[94m43 \x1B[92m\x1B[42m4F\x1B[0m \x1B[94m52 \x1B[92m\x1B[42m45\x1B[0m \x1B[94m27 53 \x1B[92m\x1B[42m20\x1B[0m  \x1B[94m43 55 \x1B[92m\x1B[42m53\x1B[0m \x1B[94m54 4F \x1B[92m\x1B[42m4D\x1B[0m \x1B[94m20 \x1B[92m\x1B[42m58\x1B[0m  \x1B[94m\r\n\
00000020  \x1B[92m\x1B[42m4D 4F 44\x1B[0m \x1B[94m45 \x1B[92m\x1B[42m4D 20\x1B[0m \x1B[94m42 \x1B[92m\x1B[42m4F\x1B[0m  \x1B[94m4F 54 \x1B[92m\x1B[42m4C\x1B[0m \x1B[94m4F 41 \x1B[92m\x1B[42m44\x1B[0m \x1B[94m45 \x1B[92m\x1B[42m52\x1B[0m  \x1B[94m\r\n\
00000030  \x1B[92m\x1B[42m50\x1B[0m \x1B[94m4C \x1B[92m\x1B[42m45\x1B[0m \x1B[94m41 \x1B[92m\x1B[42m53\x1B[0m \x1B[94m45 20 \x1B[92m\x1B[42m53\x1B[0m  \x1B[94m45 4C \x1B[92m\x1B[42m45\x1B[0m \x1B[94m43 54 \x1B[92m\x1B[42m20\x1B[0m \x1B[94m41 \x1B[92m\x1B[42m20\x1B[0m  \x1B[94m\r\n\
00000040  \x1B[92m\x1B[42m42\x1B[0m \x1B[94m4F \x1B[92m\x1B[42m4F\x1B[0m \x1B[94m54 \x1B[92m\x1B[42m20 4F\x1B[0m \x1B[94m50 \x1B[92m\x1B[42m54  49\x1B[0m \x1B[94m4F \x1B[92m\x1B[42m4E 20\x1B[0m \x1B[94m46 \x1B[92m\x1B[42m4F 52 21\x1B[0m  \r\n\x1B[0m\r\n";

const char* UPDATER_OPTIONS_STR = "\x1B[96mPress \x1B[31m'Spacebar'\x1B[0m\x1B[96m to update firmware using XMODEM(CRC)\r\n\
Press \x1B[31m'I'\x1B[0m\x1B[96m to get information about system state\r\n\
Press \x1B[31m'Q'\x1B[0m\x1B[96m to return to loader\x1B[0m\r\n";

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

    // Send version info - set color to orange (33) to represent updater
    transport_send(&uart_transport, (const uint8_t*)"\x1B[91m-- Updater v ", 16);
    sprintf(version_str, "%d.%d.%d", IMAGE_HEADER.version_major, 
            IMAGE_HEADER.version_minor, IMAGE_HEADER.version_patch);
    transport_send(&uart_transport, (const uint8_t*)version_str, strlen(version_str));
    transport_send(&uart_transport, (const uint8_t*)" --\x1B[0m\r\n\r\n", 11);
    
    // Send menu options (removed auto-boot option)
    transport_send(&uart_transport, (const uint8_t*)UPDATER_OPTIONS_STR, strlen(UPDATER_OPTIONS_STR));
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
  * @brief Check if Enter key is temporarily blocked (for ExtraPuTTY)
  * @param current_time Current system time in ms
  * @return 1 if blocked, 0 if not blocked
  */
static int is_enter_blocked(uint32_t current_time) {
    return ENTER_BLOCKED_UNTIL > current_time;
}

/**
  * @brief Block Enter key for a period of time (for ExtraPuTTY)
  * @param current_time Current system time in ms
  */
static void block_enter_temporarily(uint32_t current_time) {
    ENTER_BLOCKED_UNTIL = current_time + ENTER_BLOCK_AFTER_UPDATE_MS;
}

/**
  * @brief Send XMODEM cancel sequence
  */
static void send_cancel_sequence(void) {
    // XMODEM CAN three times
    transport_send(&uart_transport, (const uint8_t*)&(uint8_t){XMODEM_CAN}, 1);
    transport_send(&uart_transport, (const uint8_t*)&(uint8_t){XMODEM_CAN}, 1);
    transport_send(&uart_transport, (const uint8_t*)&(uint8_t){XMODEM_CAN}, 1);

    HAL_Delay(1000);
}

/**
  * @brief Recover from XMODEM transfer (cleanup and show menu)
  * @return New time reference
  */
static uint32_t recover_from_xmodem(void) {
    clear_rx_buffer();
    HAL_Delay(3000);
    
    // Clear screen
    clear_screen();
    HAL_Delay(500);
    
    // Block enter key temporarily
    block_enter_temporarily(HAL_GetTick());
    
    // Reset time reference
    uint32_t new_time = HAL_GetTick();
    
    // Display main menu
    display_menu();
    
    return new_time;
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
  * @brief Set LED state
  * @param led_pin LED pin number (0-3)
  * @param state 1 to turn on, 0 to turn off
  */
static void set_led(uint8_t led_pin, uint8_t state) {
    uint32_t pin;
    switch (led_pin) {
        case 0: {
            pin = LL_GPIO_PIN_12; 
            break;
        }
        case 1: {
            pin = LL_GPIO_PIN_13; 
            break;
        }
        case 2: {
            pin = LL_GPIO_PIN_14; 
            break;
        }
        case 3: {
            pin = LL_GPIO_PIN_15; 
            break;
        }
        default: {
            return;
        }
    }
    
    if (state) {
        LL_GPIO_SetOutputPin(GPIOD, pin);
    } else {
        LL_GPIO_ResetOutputPin(GPIOD, pin);
    }
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
    
    /* Create bootloader configuration */
    BootConfig_t boot_config = {
        .app_addr = APP_ADDR,
        .updater_addr = UPDATER_ADDR,
        .loader_addr = LOADER_ADDR,
        .image_hdr_size = IMAGE_HDR_SIZE
    };
    
    // Configure XMODEM
    xmodem_config.app_addr = APP_ADDR;
    xmodem_config.updater_addr = UPDATER_ADDR;
    xmodem_config.loader_addr = LOADER_ADDR;
    xmodem_config.image_hdr_size = IMAGE_HDR_SIZE;
#ifdef FIRMWARE_ENCRYPTED
    xmodem_config.use_encryption = 1;
#endif
    
    // Configure UART transport
    uart_config.usart = USART2;
    uart_config.baudrate = 115200;
    uart_config.timeout = 1000;
    uart_config.use_xmodem = 1;
    uart_config.app_addr = APP_ADDR;
    uart_config.updater_addr = UPDATER_ADDR;
    uart_config.loader_addr = LOADER_ADDR;
    uart_config.image_hdr_size = IMAGE_HDR_SIZE;
    
    // Initialize transport
    transport_init(&uart_transport, TRANSPORT_UART, &uart_config);
    
    // Initialize XMODEM
    xmodem_init(&xmodem_manager, &xmodem_config);
    
    // Initialize CRC module
    crc_init();
    
    // Initial clear for RX buffer
    clear_rx_buffer();
    
    // Display menu
    display_menu();
    
    // Enable USART2 IRQ
    NVIC_EnableIRQ(USART2_IRQn);
    
    // Main variables
    bool update_in_progress = false;
    uint32_t firmware_target = APP_ADDR;
    uint32_t led_toggle_time = HAL_GetTick();
    PostXmodemState_t post_xmodem_state = POST_XMODEM_COMPLETE;
    bool xmodem_error_occurred = false;
    bool return_to_loader = false;
    
    while (1) {
        // Process UART data
        transport_process(&uart_transport);
        
        // Blink orange LED every 500ms to indicate we're in updater mode
        uint32_t current_time = HAL_GetTick();
        if (current_time - led_toggle_time >= 500) {
            toggle_led(1); // LED1 - orange for updater
            led_toggle_time = current_time;
        }
        
        // Handle post XMODEM recovery
        if (post_xmodem_state == POST_XMODEM_RECOVERING) {
            recover_from_xmodem();
            post_xmodem_state = POST_XMODEM_COMPLETE;
            update_in_progress = false;
            continue;
        }
        
        // Handle return to loader if requested
        if (return_to_loader) {
            // Wait for UART to finish
            while (!uart_transport_is_tx_complete()) {
                transport_process(&uart_transport);
            }
            clear_rx_buffer();
            
            // Boot to loader
            boot_loader(&boot_config);
            
            // If boot failed, reset the flag and continue
            return_to_loader = false;
            display_menu();
            continue;
        }
        
        // Read and process user input if not updating
        if (!update_in_progress) {
            uint8_t byte;
            if (transport_receive(&uart_transport, &byte, 1) > 0) {
                
                switch (byte) {
                    case 'Q':
                    case 'q': {
                        // Return to loader
                        if (is_firmware_valid(LOADER_ADDR, &boot_config)) {
                            transport_send(&uart_transport, (const uint8_t*)"\x1B[36m\r\n Returning to loader...\x1B[0m\r\n", 35);
                            return_to_loader = true;
                        } else {
                            transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\nValid loader not found!\x1B[0m\r\n", 36);
                            HAL_Delay(1500);
                            display_menu();
                        }
                        break;
                    }
                        
                    case 0x0D:
                    case 0x20: {
                        // Start firmware update
                        clear_screen();
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[96mUpdate firmware using XMODEM - select target:\x1B[0m\r\n", 58);
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[92m[\x1B[33m1\x1B[92m] \x1B[32m- Loader\x1B[0m", 34);
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[92m[\x1B[33m2\x1B[92m] \x1B[32m- Application\x1B[0m", 39);
                        break;
                    }
                        
                    case '1': {
                        // Start loader update
                        clear_screen();
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[92mUpdating loader...\x1B[0m\r\n", 30);
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[91mSend file using XMODEM protocol with CRC-16. \x1B[0m\r\n\x1B[31mIf menu doesn't load after update is over, please press \x1B[91m'Esc'\x1B[0m\r\n", 131);
                        firmware_target = LOADER_ADDR;
                        xmodem_start(&xmodem_manager, firmware_target);
                        update_in_progress = true;
                        
                        set_led(0, 1);  // Green - system alive
                        set_led(1, 1);  // Orange - XMODEM active
                        set_led(2, 0);  // Red - no error
                        set_led(3, 0);  // Blue - no data received yet
                        
                        // Spam initial 'C'
                        if (xmodem_should_send_byte(&xmodem_manager)) {
                            uint8_t response = xmodem_get_response(&xmodem_manager);
                            transport_send(&uart_transport, &response, 1);
                        }
                        break;
                    }
                        
                    case '2': {
                        // Start application update
                        clear_screen();
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[92mUpdating application...\x1B[0m\r\n", 34);
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[96mSend file using XMODEM protocol with CRC-16.\x1B[0m\r\n\x1B[91mIf menu doesn't load after update is over, please press \x1B[31m'Esc'\x1B[0m\r\n", 131);
                        firmware_target = APP_ADDR;
                        xmodem_start(&xmodem_manager, firmware_target);
                        update_in_progress = true;
                        
                        set_led(0, 1);  // Green - system alive
                        set_led(1, 1);  // Orange - XMODEM active
                        set_led(2, 0);  // Red - no error
                        set_led(3, 0);  // Blue - no data received yet
                        
                        // Spam initial 'C'
                        if (xmodem_should_send_byte(&xmodem_manager)) {
                            uint8_t response = xmodem_get_response(&xmodem_manager);
                            transport_send(&uart_transport, &response, 1);
                        }
                        break;
                    }
                        
                    case 'I':
                    case 'i': {
                        // Show system information
                        clear_screen();
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[31m\r\n--- System Information ---\x1B[0m\r\n\r\n", 43);
                        
                        // Updater info first (this image)
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[96mUpdater (this image): \x1B[0m", 31);
                        
                        ImageHeader_t header;
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
                        
                        // Loader info
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[96mLoader: \x1B[0m", 21);

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
                        
                        // System info
                        char buffer[256];
                        transport_send(&uart_transport, (const uint8_t*)"\x1B[96mSystem Info:\x1B[0m\r\n", 22);
                        
                        sprintf(buffer, "\x1B[92m  System uptime: \x1B[93m%u seconds\x1B[0m\r\n", HAL_GetTick() / 1000);
                        transport_send(&uart_transport, (const uint8_t*)buffer, strlen(buffer));
                        
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[91mPress \x1B[31m'Esc'\x1B[0m \x1B[91mto return to menu...\x1B[0m\r\n", 59);
                        
                        uint8_t key;
                        while(1) {
                            transport_process(&uart_transport);
                            if (transport_receive(&uart_transport, &key, 1) > 0) {
                                if (key == 0x1B) { // ESC key
                                    break;
                                }
                            }
                        }
                        
                        display_menu();
                        break;
                    }
                        
                    default: {
                        if (byte == 0x1B) { // ESC key
                            clear_screen();
                            display_menu();
                        } 
                        break;
                    }
                }
            }
        } else {
            // handle XMODEM update
            uint8_t byte;
            if (transport_receive(&uart_transport, &byte, 1) > 0) {
                
                XmodemError_t result = xmodem_process_byte(&xmodem_manager, byte);
                
                // Send response if needed
                if (xmodem_should_send_byte(&xmodem_manager)) {
                    uint8_t response = xmodem_get_response(&xmodem_manager);
                    transport_send(&uart_transport, &response, 1);
                }
                
                // Handle XMODEM result
                switch (result) {
                    case XMODEM_ERROR_NONE: {
                        // Normal processing, nothing to do
                        break;
                    }
                        
                    case XMODEM_ERROR_TRANSFER_COMPLETE: {
                        // Send ACK for EOT if needed
                        if (xmodem_should_send_byte(&xmodem_manager)) {
                            uint8_t response = xmodem_get_response(&xmodem_manager);
                            transport_send(&uart_transport, &response, 1);
                        }
                        
                        update_in_progress = false;
                        xmodem_error_occurred = false;
                        // Wait for flash operations
                        HAL_Delay(100);

                        transport_send(&uart_transport, (const uint8_t*)"\r\nDumping raw header bytes from PATCH_ADDR:\r\n", 44);
                        uint8_t* raw_header = (uint8_t*)PATCH_ADDR;
                        char debug_bytes[100];
                        for (int i = 0; i < 32; i += 4) {
                            sprintf(debug_bytes, "%02X %02X %02X %02X\r\n", 
                                    raw_header[i], raw_header[i+1], raw_header[i+2], raw_header[i+3]);
                            transport_send(&uart_transport, (const uint8_t*)debug_bytes, strlen(debug_bytes));
                        }
                        
                        // Read the header from the staging area to determine what we received
                        ImageHeader_t received_header;
                        memcpy(&received_header, (void*)PATCH_ADDR, sizeof(ImageHeader_t));
                        
                        // Calculate the total size of the firmware we received
                        uint32_t received_size = received_header.data_size + IMAGE_HDR_SIZE;
                        
                        // Debug output
                        char debug[120];
                        sprintf(debug, "\r\n\x1B[93mReceived firmware: type=%d, is_patch=%d, size=%lu bytes\x1B[0m\r\n",
                                received_header.image_type, received_header.is_patch, received_size);
                        transport_send(&uart_transport, (const uint8_t*)debug, strlen(debug));
                        
                        // Check if it's a patch
                        if (received_header.is_patch) {
                            // Determine the target address based on image type
                            uint32_t target_addr;
                            
                            if (received_header.image_type == IMAGE_TYPE_APP) {
                                target_addr = APP_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived application patch\x1B[0m\r\n", 42);
                            } else if (received_header.image_type == IMAGE_TYPE_LOADER) {
                                target_addr = LOADER_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived loader patch\x1B[0m\r\n", 39);
                            } else if (received_header.image_type == IMAGE_TYPE_UPDATER) {
                                target_addr = UPDATER_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived updater patch\x1B[0m\r\n", 40);
                            } else {
                                // Unknown image type
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mUnknown image type in patch!\x1B[0m\r\n", 41);
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                                post_xmodem_state = POST_XMODEM_RECOVERING;
                                break;
                            }
                            
                            // This is a patch - apply it
                            transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mApplying patch to firmware...\x1B[0m\r\n", 41);

                            // Output debug info
                            sprintf(debug, "\r\nDebug: Target=0x%08lX, PATCH_ADDR=0x%08lX, BACKUP_ADDR=0x%08lX\r\n", 
                                    target_addr, PATCH_ADDR, BACKUP_ADDR);
                            transport_send(&uart_transport, (const uint8_t*)debug, strlen(debug));

                            // Apply the patch using our handle_firmware_patch function
                            int result = handle_firmware_patch(
                                target_addr,    // Source address (current firmware)
                                PATCH_ADDR,     // Patch address (staging area)
                                target_addr,    // Target address (same as source)
                                BACKUP_ADDR,    // Backup address
                                IMAGE_HDR_SIZE  // Header size
                            );

                            if (result != 0) {
                                char error_str[64];
                                sprintf(error_str, "\r\n\x1B[31mPatch application failed! Error code: %d\x1B[0m\r\n", result);
                                transport_send(&uart_transport, (const uint8_t*)error_str, strlen(error_str));
                                
                                // Detailed error messages based on error code
                                switch(result) {
                                    case 1:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mNo valid source firmware found!\x1B[0m\r\n", 47);
                                        break;
                                    case 2:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mNot a valid patch file!\x1B[0m\r\n", 38);
                                        break;
                                    case 3:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mInvalid backup sector!\x1B[0m\r\n", 39);
                                        break;
                                    case 4:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to erase backup sectors!\x1B[0m\r\n", 48);
                                        break;
                                    case 5:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to write backup!\x1B[0m\r\n", 41);
                                        break;
                                    case 6:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to erase target sectors!\x1B[0m\r\n", 48);
                                        break;
                                    case 7:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to write header!\x1B[0m\r\n", 39);
                                        break;
                                    case 8:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to apply delta patch!\x1B[0m\r\n", 44);
                                        break;
                                    case 9:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mCRC verification failed!\x1B[0m\r\n", 39);
                                        break;
                                    default:
                                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mUnknown error during patching!\x1B[0m\r\n", 45);
                                        break;
                                }
                                
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mFirmware invalidated.\x1B[0m\r\n", 38);
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mRestored from backup.\x1B[0m\r\n", 40);
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                            } else {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[32mPatch applied successfully!\x1B[0m\r\n", 43);
                                
                                // Clean up patch sectors
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mCleaning up patch data...\x1B[0m\r\n", 39);
                                
                                // Erase patch sectors
                                for (uint32_t addr = PATCH_ADDR; addr < PATCH_ADDR + PATCH_SIZE; addr += 0x20000) {
                                    flash_erase_sector(addr);
                                }
                                
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[32mCleanup complete.\x1B[0m\r\n", 30);
                            }
                        } else {
                            // Regular firmware (not a patch) - copy from staging to destination
                            uint32_t destination_addr;
                            
                            // Determine destination address based on image type
                            if (received_header.image_type == IMAGE_TYPE_APP) {
                                destination_addr = APP_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived application firmware\x1B[0m\r\n", 45);
                            } else if (received_header.image_type == IMAGE_TYPE_LOADER) {
                                destination_addr = LOADER_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived loader firmware\x1B[0m\r\n", 42);
                            } else if (received_header.image_type == IMAGE_TYPE_UPDATER) {
                                destination_addr = UPDATER_ADDR;
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mReceived updater firmware\x1B[0m\r\n", 43);
                            } else {
                                // Unknown image type
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mUnknown image type!\x1B[0m\r\n", 33);
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                                post_xmodem_state = POST_XMODEM_RECOVERING;
                                break;
                            }
                            
                            // Prepare to copy from staging area to destination
                            transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mVerifying firmware CRC...\x1B[0m\r\n", 39);
                            
                            // Verify the CRC first
                            if (!verify_firmware_crc(PATCH_ADDR, IMAGE_HDR_SIZE)) {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mCRC verification failed! Aborting.\x1B[0m\r\n", 47);
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                                post_xmodem_state = POST_XMODEM_RECOVERING;
                                break;
                            }
                            
                            transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[32mCRC verification successful.\x1B[0m\r\n", 42);
                            
                            // Now copy to the destination
                            transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mCopying firmware to destination...\x1B[0m\r\n", 47);
                            
                            // Erase destination sectors
                            uint8_t sector = flash_get_sector(destination_addr);
                            if (sector == 0xFF) {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mInvalid destination sector!\x1B[0m\r\n", 44);
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                                post_xmodem_state = POST_XMODEM_RECOVERING;
                                break;
                            }
                            
                            // Erase sectors at destination
                            flash_erase_sector(destination_addr);
                            if (received_size > 0x20000) { // If larger than one sector
                                flash_erase_sector(destination_addr + 0x20000);
                            }
                            
                            // Copy firmware from staging to destination
                            if (!flash_write(destination_addr, (uint8_t*)PATCH_ADDR, received_size)) {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFailed to copy firmware to destination!\x1B[0m\r\n", 54);
                                
                                // Invalidate destination
                                invalidate_firmware(destination_addr);
                                
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                                post_xmodem_state = POST_XMODEM_RECOVERING;
                                break;
                            }
                            
                            // Verify the copy
                            transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[93mVerifying destination firmware...\x1B[0m\r\n", 48);
                            if (!verify_firmware_crc(destination_addr, IMAGE_HDR_SIZE)) {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mDestination CRC verification failed!\x1B[0m\r\n", 52);
                                
                                // Invalidate destination
                                invalidate_firmware(destination_addr);
                                
                                xmodem_error_occurred = true;
                                set_led(2, 1);  // Red LED
                            } else {
                                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[32mDestination firmware verified successfully.\x1B[0m\r\n", 57);
                                
                                // Clean up staging area
                                for (uint32_t addr = PATCH_ADDR; addr < PATCH_ADDR + PATCH_SIZE; addr += 0x20000) {
                                    flash_erase_sector(addr);
                                }
                            }
                        }
                        
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                    }
                        
                    // Handle other XMODEM errors
                    case XMODEM_ERROR_CANCELLED:
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mTransfer cancelled.\x1B[0m\r\n", 33);
                        xmodem_error_occurred = true;
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                        
                    case XMODEM_ERROR_TIMEOUT:
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mTransfer timed out.\x1B[0m\r\n", 33);
                        xmodem_error_occurred = true;
                        set_led(2, 1);  // Red LED
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                        
                    case XMODEM_ERROR_SEQUENCE_ERROR:
                    case XMODEM_ERROR_CRC_ERROR:
                    case XMODEM_ERROR_INVALID_PACKET:
                        // XMODEM protocol will handle these internally
                        if (xmodem_should_send_byte(&xmodem_manager)) {
                            uint8_t response = xmodem_get_response(&xmodem_manager);
                            transport_send(&uart_transport, &response, 1);
                        }
                        break;
                        
                    case XMODEM_ERROR_FLASH_WRITE_ERROR:
                        xmodem_cancel_transfer(&xmodem_manager);
                        send_cancel_sequence();
                        
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mError writing to flash memory.\x1B[0m\r\n", 46);
                        
                        xmodem_error_occurred = true;
                        set_led(2, 1);  // Red LED
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                        
                    case XMODEM_ERROR_INVALID_MAGIC:
                        xmodem_cancel_transfer(&xmodem_manager);
                        send_cancel_sequence();
                        
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mInvalid firmware image magic number.\x1B[0m\r\n", 51);
                        
                        xmodem_error_occurred = true;
                        set_led(2, 1);  // Red LED
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                        
                    case XMODEM_ERROR_OLDER_VERSION:
                        xmodem_cancel_transfer(&xmodem_manager);
                        send_cancel_sequence();
                        
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFirmware version is older than current version.\x1B[0m\r\n", 63);
                        
                        xmodem_error_occurred = true;
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                        
                    case XMODEM_ERROR_AUTHENTICATION_FAILED:
                        xmodem_cancel_transfer(&xmodem_manager);
                        send_cancel_sequence();
                        
                        transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mFirmware authentication failed.\x1B[0m\r\n", 46);
                        
                        xmodem_error_occurred = true;
                        set_led(2, 1);  // Red LED
                        post_xmodem_state = POST_XMODEM_RECOVERING;
                        break;
                }
            }
            
            // Check if need to send 'C'
            if (xmodem_should_send_byte(&xmodem_manager)) {
                uint8_t response = xmodem_get_response(&xmodem_manager);
                transport_send(&uart_transport, &response, 1);
            }
            
            // Also check for errors
            if (xmodem_get_state(&xmodem_manager) == XMODEM_STATE_ERROR) {
                transport_send(&uart_transport, (const uint8_t*)"\r\n\x1B[31mXMODEM transfer error. Aborting.\x1B[0m\r\n", 46);
                
                xmodem_error_occurred = true;
                post_xmodem_state = POST_XMODEM_RECOVERING;
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