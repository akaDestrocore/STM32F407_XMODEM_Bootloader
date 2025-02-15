/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <uart_log.h>

typedef struct __attribute__((packed))
{
	uint32_t signature0;
	uint8_t signature1;
	uint8_t version_major;
	uint8_t version_minor;
	uint8_t version_patch;
}image_version_t;

// version of this application
image_version_t image_ver __attribute__((section(".image_ver"))) = {
	.signature0		= 0xDABA,
	.signature1 	= 0xF,
    .version_major 	= 1,
    .version_minor 	= 2,
    .version_patch 	= 3
};

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart;
Ring_Buffer_t rxBuff, txBuff;
uint8_t rx_byte = 0;
uint8_t TxInProgress = RESET;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void ShowMessage(ANIMATION_t frame);

// serial terminal related API
void clear_screen(void)
{
    DBG("\033[2J");
}

void set_cursor_position(int x, int y)
{
    char buffer[32];
    sprintf(buffer, "\033[%d;%dH", y, x);
    DBG(buffer);
}

int main(void)
{

  RingBuffer_Init(&rxBuff);
  RingBuffer_Init(&txBuff);
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  memcpy(&huart, &huart2, sizeof(huart));
  __HAL_UART_ENABLE_IT(&huart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(&huart,UART_IT_RXNE);
  char ver[80];
  sprintf(ver, "Welcome to version %d.%d.%d\r\n",image_ver.version_major, image_ver.version_minor, image_ver.version_patch);
  DBG(ver);
  clear_screen();

  while (1)
  {
	  set_cursor_position(0, 0);
	  HAL_GPIO_TogglePin(GPIOD, RED_Pin);
	  ShowMessage(FRAME0);
	  clear_screen();
	  set_cursor_position(0, 0);
	  HAL_GPIO_TogglePin(GPIOD, ORANGE_Pin);
	  ShowMessage(FRAME1);
	  clear_screen();
	  set_cursor_position(0, 0);
	  HAL_GPIO_TogglePin(GPIOD, GREEN_Pin);
	  ShowMessage(FRAME2);
	  clear_screen();
	  set_cursor_position(0, 0);
	  HAL_GPIO_TogglePin(GPIOD, BLUE_Pin);
	  ShowMessage(FRAME3);
	  clear_screen();
	  set_cursor_position(0, 0);
	  ShowMessage(FRAME4);

  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_UART_ENABLE_IT(&huart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(&huart,UART_IT_RXNE);


}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void ShowMessage(ANIMATION_t frame)
{
    switch (frame)
    {
        case FRAME0:  // T-Rex running frame 1
        {
            DBG("            ______                                                                         \n\r");
            DBG("           /      \\__                                                                      \n\r");
            DBG("          /   []     |                                                                     \n\r");
            DBG("          |          |                                                                     \n\r");
            DBG("          |    ______|                                                                     \n\r");
            DBG("         /    |____                                                                        \n\r");
            DBG("        /     ____|                                                                        \n\r");
            DBG("|\\     /     |_                                                                           \n\r");
            DBG("| \\   /        /\\                                                                         \n\r");
            DBG("|  \\_/        |  []                                                                       \n\r");
            DBG("|             |                                                                           \n\r");
            DBG(" \\           /                                                                            \n\r");
            DBG("  \\         /                                                                             \n\r");
            DBG("    \\  ___ |                                                                             \n\r");
            DBG("     | |  \\|_                                                                            \n\r");
            DBG("     |/    [_]                                                                           \n\r");
            DBG("     |_                                                                                   \n\r");
            DBG("     [_]                                                                                  \n\r");
            break;
        }
        case FRAME1:  // T-Rex running frame 2
        {
            DBG("            ______                                                                         \n\r");
            DBG("           /      \\__                                                                      \n\r");
            DBG("          /   []     |                                                                     \n\r");
            DBG("          |          |                                                                     \n\r");
            DBG("          |    ______|                                                                     \n\r");
            DBG("         /    |____                                                                        \n\r");
            DBG("        /     ____|                                                                        \n\r");
            DBG("|\\     /     |_                                                                           \n\r");
            DBG("| \\   /        /\\                                                                         \n\r");
            DBG("|  \\_/        |  []                                                                       \n\r");
            DBG("|             |                                                                           \n\r");
            DBG(" \\           /                                                                            \n\r");
            DBG("  \\         /                                                                             \n\r");
            DBG("    \\  __  |                                                                             \n\r");
            DBG("     |/  | |                                                                             \n\r");
            DBG("     [_]  \\|                                                                             \n\r");
            DBG("           |_                                                                             \n\r");
            DBG("           [_]                                                                            \n\r");
            break;
        }
        case FRAME2:  // Jumping T-Rex
        {
            DBG("                                                                                           \n\r");
            DBG("            ______                                                                         \n\r");
            DBG("           /      \\__                                                                      \n\r");
            DBG("          /   []     |                                                                     \n\r");
            DBG("          |          |                                                                     \n\r");
            DBG("          |    ______|                                                                     \n\r");
            DBG("         /    |____                                                                        \n\r");
            DBG("        /     ____|                                                                        \n\r");
            DBG("|\\     /     |_                                                                           \n\r");
            DBG("| \\   /        /\\                                                                         \n\r");
            DBG("|  \\_/        |  []                                                                       \n\r");
            DBG("|             |                                                                           \n\r");
            DBG(" \\           /                                                                            \n\r");
            DBG("  \\         /                                                                             \n\r");
            DBG("    \\  ___ |                                                                             \n\r");
            DBG("     | |  \\|                                                                             \n\r");
            DBG("     |/    |                                                                             \n\r");
            DBG("     |_    |_                                                                            \n\r");
            DBG("     [_]   [_]                                                                           \n\r");
            break;
        }
        case FRAME3:  // T-Rex running frame 3 (variation of frame 1)
        {
            DBG("            ______                                                                         \n\r");
            DBG("           /      \\__                                                                      \n\r");
            DBG("          /   []     |                                                                     \n\r");
            DBG("          |          |                                                                     \n\r");
            DBG("          |    ______|                                                                     \n\r");
            DBG("         /    |____                                                                        \n\r");
            DBG("        /     ____|                                                                        \n\r");
            DBG("|\\     /     |_                                                                           \n\r");
            DBG("| \\   /        /\\                                                                         \n\r");
            DBG("|  \\_/        |  []                                                                       \n\r");
            DBG("|             |                                                                           \n\r");
            DBG(" \\           /                                                                            \n\r");
            DBG("  \\         /                                                                             \n\r");
            DBG("    \\  ___ |                                                                             \n\r");
            DBG("     | |  \\|_                                                                            \n\r");
            DBG("     |/    [_]                                                                           \n\r");
            DBG("     |_                                                                                   \n\r");
            DBG("     [_]                                                                                  \n\r");
            break;
        }
        case FRAME4:  // T-Rex running frame 4 (variation of frame 2)
        {
            DBG("            ______                                                                         \n\r");
            DBG("           /      \\__                                                                      \n\r");
            DBG("          /   []     |                                                                     \n\r");
            DBG("          |          |                                                                     \n\r");
            DBG("          |    ______|                                                                     \n\r");
            DBG("         /    |____                                                                        \n\r");
            DBG("        /     ____|                                                                        \n\r");
            DBG("|\\     /     |_                                                                           \n\r");
            DBG("| \\   /        /\\                                                                         \n\r");
            DBG("|  \\_/        |  []                                                                       \n\r");
            DBG("|             |                                                                           \n\r");
            DBG(" \\           /                                                                            \n\r");
            DBG("  \\         /                                                                             \n\r");
            DBG("    \\  __  |                                                                             \n\r");
            DBG("     |/  | |                                                                             \n\r");
            DBG("     [_]  \\|                                                                             \n\r");
            DBG("           |_                                                                             \n\r");
            DBG("           [_]                                                                            \n\r");
            break;
        }
        default:
            break;
    }
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
