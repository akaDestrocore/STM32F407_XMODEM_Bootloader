/* USER CODE BEGIN Header */
/*@file      LOADER.c								    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @brief  	Loader should verify application image  	@@@@@@@@@@@@@@@@@@@@#             *@@@@@@@@@@@@@@@@@
  			and load it									@@@@@@@@@@@@@@@@@%     *@@@@@@@@@@   *@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@     .#@@@@@@@@@@@@@% *@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@.    .@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@
   														@@@@@@@@@@@@@@       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @version  1.2											@@@@@@@@@@@@@%      @@@@@    @@@@@@@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@#      @@@@@    **#@@@@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@      @@@@@        *@@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@      @@@@@%        #@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@     @@@@@@=           %@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@    :@@@@@             @@@@@@@@@@@@@
 	 	 	 	 	 	 	 	 	 	 	 	 		@@@@@@@@@@@@@@@@@@   .#@@@@@@@@@#:    @@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@@@@@     @@@@@*     @@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@@@@@@@@@@_____@@@@@@@@@@@@@@@@@@@@@@
														@  __ *@@@  @@  @@  @@@  @@@  @@  @@*   _ )@@@  @@
														  @@@@@@@@  @@  @@   @   @@@  @@  @@   (@@@@@@  @@
														  @@   @@@  @@  @@       @@@  @@  @@@@   @@@@@  __
														  @@@  @@@  @@  @@  @_@  @@@  @@  @@@@@@   @@@  @@
														@  @@  @@@  @@  @@  @@@  @@@  @@  @@  @@@  @@@  @@
														@@____@@@@______@@__@@@__@@@@_____@@@______@@@__@@__
														@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@					*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart;

/* USER CODE BEGIN PV */
// initialize ring buffers for RX and TX
Ring_Buffer_t rxBuff, txBuff;

// variable to read one byte from RX ring buffer
uint8_t rx_byte = 0;

// flag to indicate that transmission is in progress
uint8_t TxInProgress = RESET;

// set this flag to jump to SLOT2 application
uint8_t LoadApplication = RESET;

// needed for automated jump after time is out
uint32_t boot_timeout = 10000;
uint32_t start_tick = 0;

// holder of the user's selection
uint8_t LoadingOptionSelected = RESET;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ShowMessage(MESSAGE_t msg);
void JumpToUserApplication(void);
void Read_Key(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	// Initialize circular buffers for TX and RX
	RingBuffer_Init(&rxBuff);
	RingBuffer_Init(&txBuff);

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  // print launch messages over UART
  ShowMessage(MSG_WELCOME);

  ShowMessage(MSG_SELECT_OPTION);

  start_tick = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // constantly read user input
	  Read_Key();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
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

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
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
    /* USER CODE BEGIN USART2_Init 2 */
    __HAL_UART_ENABLE_IT(&huart, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

      /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GREEN_Pin | ORANGE_Pin | RED_Pin | BLUE_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : USER_BTN_Pin */
    GPIO_InitStruct.Pin = USER_BTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : GREEN_Pin ORANGE_Pin RED_Pin BLUE_Pin */
    GPIO_InitStruct.Pin = GREEN_Pin | ORANGE_Pin | RED_Pin | BLUE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
 * message expressions for terminal
 */
void ShowMessage(MESSAGE_t msg)
{
    switch (msg)
    {
        case MSG_WELCOME:
        {
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@@@#             *@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@%     *@@@@@@@@@@   *@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@     .#@@@@@@@@@@@@@% *@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@.    .@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@%      @@@@@    @@@@@@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@#      @@@@@    **#@@@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@      @@@@@        *@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@      @@@@@%        #@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@     @@@@@@=           %@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@    :@@@@@             @@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@   .#@@@@@@@@@#:    @@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@@@     @@@@@*     @@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@@@@@@@@_____@@@@@@@@@@@@@@@@@@@@@@\n\r");
            USART_SendString((uint8_t*)" @  __ *@@@  @@  @@  @@@  @@@  @@  @@*   _ )@@@  @@  \n\r");
            USART_SendString((uint8_t*)"   @@@@@@@@  @@  @@   @   @@@  @@  @@   (@@@@@@  @@  \n\r");
            USART_SendString((uint8_t*)"   @@   @@@  @@  @@       @@@  @@  @@@@   @@@@@  __  \n\r");
            USART_SendString((uint8_t*)"   @@@  @@@  @@  @@  @_@  @@@  @@  @@@@@@   @@@  @@  \n\r");
            USART_SendString((uint8_t*)" @  @@  @@@  @@  @@  @@@  @@@  @@  @@  @@@  @@@  @@  \n\r");
            USART_SendString((uint8_t*)" @@____@@@@______@@__@@@__@@@@_____@@@______@@@__@@__\n\r");
            USART_SendString((uint8_t*)" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r");

            USART_SendString((uint8_t*)" CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n\r");
            USART_SendString((uint8_t*)" }           XMODEM BOOTLOADER [CRC 16 BIT]         {\n\r");
            USART_SendString((uint8_t*)" }                                                  {\n\r");
            USART_SendString((uint8_t*)" }                 by destrocore                    {\n\r");
            USART_SendString((uint8_t*)" CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n\r\n\r");
            break;
        }
        case MSG_SELECT_OPTION:
        {
        	USART_SendString((uint8_t*)"Loading application in 10 seconds.\n\r\n\r");
        	USART_SendString((uint8_t*)"Please select an option:\n\r\n\r");
        	USART_SendString((uint8_t*)" > 'SHIFT+U' --> Hold 'SHIFT' and then press 'U' key to go to updater\n\r\n\r");
        	USART_SendString((uint8_t*)" > 'ENTER'   --> Press 'ENTER' for regular boot\n\r");
        	break;
        }
        case MSG_NO_SUCH_KEY:
        {
        	USART_SendString((uint8_t*)"No such option available. Please select from available options.\n\r");
        	break;
        }
        case MSG_GOTO_APP:
        {
        	USART_SendString((uint8_t*)"\r\n Jumping to user application...\n\r");
        	break;
        }
        case MSG_GOTO_UPDATER:
        {
        	USART_SendString((uint8_t*)"\r\n Jumping to updater...\n\r");
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
	USART_SendString((uint8_t*)"\n\rLoading user application now.\n\r\n\r");

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

	USART_SendString((uint8_t*)"\033[2J");
	USART_SendString((uint8_t*)"\033[0;0H");

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
				ShowMessage(MSG_GOTO_UPDATER);
				JumpToUpdater();
				break;
			}
			case '\r':
			{
				if(0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
				{
					USART_SendString((uint8_t*)"There is no user application!\n\r");
					rx_byte = 0;
					break;
				}
				ShowMessage(MSG_GOTO_APP);
				LoadApplication = SET;
				break;
			}
			default:
			{
				if(0 != rx_byte)
				{
					ShowMessage(MSG_NO_SUCH_KEY);
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
