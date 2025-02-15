#include "main.h"
#include <uart_log.h>

/* Private define ------------------------------------------------------------*/
typedef void (*fnc_ptr)(void);

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart;
// ring buffers for RX and TX
Ring_Buffer_t rxBuff, txBuff;
uint8_t rx_byte = 0;
uint8_t TxInProgress = RESET;
uint8_t LoadApplication = RESET;
uint32_t boot_timeout = 10000;
uint32_t start_tick = 0;
uint8_t LoadingOptionSelected = RESET;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void ShowMessage(MESSAGE_t msg);
void JumpToUserApplication(void);
void Read_Key(void);


int main(void)
{

	// Initialize circular buffers for TX and RX
	RingBuffer_Init(&rxBuff);
	RingBuffer_Init(&txBuff);
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  // print launch messages over UART
    ShowMessage(PRINT_HELLO);
    ShowMessage(PRINT_OPTIONS);
    start_tick = HAL_GetTick();

  while (1)
  {
	  // constantly read user input
	  Read_Key();
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
  RCC_OscInitStruct.PLL.PLLN = 90;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{

  huart.Instance = USART2;
  huart.Init.BaudRate = 115200;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_NONE;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_UART_ENABLE_IT(&huart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(&huart,UART_IT_RXNE);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = USER_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GREEN_Pin ORANGE_Pin RED_Pin BLUE_Pin */
  GPIO_InitStruct.Pin = GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/*
 * message expressions for terminal
 */
void ShowMessage(MESSAGE_t msg)
{
    switch (msg)
    {
        case PRINT_HELLO:
        {
            DBG(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@@@@@@@@@@@=====@@@@@@@@@@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@@@@@@===============@@@@@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@@@====================@@@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@========================@@@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@===========================@@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@=============================@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@===============================@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@================================@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@====@=@=====@@==@=@@====@=@@===@@@@@@@@@@\n\r");
            DBG(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");
            DBG(" ----------------------------------------------------\n\r");
            DBG(" @@@@@/@@@@@@@/@@@@@/@@@@@@|@@@@\\@@@@@@\\@@@@\\@@@@@@@@\n\r");
            DBG(" @@@/@@@@@@@@/@@@@@@/@@@@@@|@@@@@\\@@@@@@\\@@@@@\\@@@@@@\n\r");
            DBG(" @/@@@@@@@@/@@@@@@#/@@@@@@@|@@@@@@\\@@@@@@@\\@@@@@@\\@@@\n\r");

            DBG(" CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n\r");
            DBG(" }             WELCOME TO BOOTLOADER                {\n\r");
            DBG(" }                                                  {\n\r");
            DBG(" }                 by destrocore                    {\n\r");
            DBG(" CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n\r\n\r");
            break;
        }
        case PRINT_OPTIONS:
        {
        	DBG("Booting into application in 10 seconds.\n\r\n\r");
        	DBG("Select an option:\n\r\n\r");
        	DBG(" > 'SHIFT+U' --> Go to UPDATER\n\r\n\r");
        	DBG(" > 'ENTER'   --> Regular boot\n\r");
        	break;
        }
        case PRINT_SELECTION_ERR:
        {
        	DBG("Please select from available options.\n\r");
        	break;
        }
        case PRINT_BOOT_FIRMWARE:
        {
        	DBG("\r\n Booting into user application...\n\r");
        	break;
        }
        case PRINT_BOOT_UPDATER:
        {
        	DBG("\r\n Loading updater...\n\r");
        	break;
        }
        default:
            break;
    }
}

/*
 * @function name    - JumpToUserApplication
 *
 * @brief            - Function to jump to the application that is in slot2rom
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - Does regular jump to the user application reset handler
 */
void JumpToUserApplication(void)
{
	DBG("\n\rLoading user application now.\n\r\n\r");

	fnc_ptr jump_to_app;
	jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (SLOT_2_APP_ADDR+4U));

	HAL_RCC_DeInit();
	HAL_DeInit();

	SYSCFG->MEMRMP = 0x01;

	SysTick->CTRL = 0; //disable SysTick

	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk ;

	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
					 SCB_SHCSR_BUSFAULTENA_Msk | \
					 SCB_SHCSR_MEMFAULTENA_Msk ) ;

	SCB->VTOR = (uint32_t)SLOT_2_APP_ADDR;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)SLOT_2_APP_ADDR);
	__set_PSP(*(volatile uint32_t*)SLOT_2_APP_ADDR);

	jump_to_app();
}

/*
 * @function name    - JumpToUpdater
 *
 * @brief            - Function to jump to UPDATER instead of user application
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - Jump to UPDATER applications's reset handler
 */
void JumpToUpdater(void)
{
	fnc_ptr jump_to_updt;
	jump_to_updt = (fnc_ptr)(*(volatile uint32_t*) (UPDATER_ADDR+4U));

	HAL_RCC_DeInit();
	HAL_DeInit();

	SYSCFG->MEMRMP = 0x01;

	SysTick->CTRL = 0; //disable SysTick

	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk ;

	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
					 SCB_SHCSR_BUSFAULTENA_Msk | \
					 SCB_SHCSR_MEMFAULTENA_Msk ) ;

	SCB->VTOR = (uint32_t)UPDATER_ADDR;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)UPDATER_ADDR);
	__set_PSP(*(volatile uint32_t*)UPDATER_ADDR);

	jump_to_updt();
}

/*
 * @function name    - Read_Key
 *
 * @brief            - Read user input from serial terminal
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This is the main routine of the LOADER and it allows user
 * 						to jump to UPDATER if needed. After timeout it does regular
 * 						boot into present user application
 */
void Read_Key(void)
{
	while(0 == rx_byte)
	{
		RingBuffer_Read(&rx_byte, &rxBuff);

		switch(rx_byte)
		{
			case 'U':
			{
				// Show 'update in progress' message
				ShowMessage(PRINT_BOOT_UPDATER);
				JumpToUpdater();
				break;
			}
			case '\r':
			{
				if(0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
				{
					DBG("There is no user application!\n\r");
					rx_byte = 0;
					break;
				}
				ShowMessage(PRINT_BOOT_FIRMWARE);
				LoadApplication = SET;
				break;
			}
			default:
			{
				if(0 != rx_byte)
				{
					ShowMessage(PRINT_SELECTION_ERR);
					rx_byte = 0;
				}
				break;
			}
		}
		if((SET == LoadApplication) || ((HAL_GetTick() - start_tick) >= boot_timeout))
		{
			JumpToUserApplication();
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
