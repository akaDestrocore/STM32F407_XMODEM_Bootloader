/* USER CODE BEGIN Header */
/*@file     BOOT.c										@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @brief  	this is a simple program whose job			@@@@@@@@@@@@@@@@##@@@@@@@@`%@@@@@@@@@@@@@@@@@@@@
  			is to load the Loader(slot1rom), and		@@@@@@@@@@@@@@@@‾‾* `        ` *@@@@@@@@@@@@@@@@@
			fall back to updater if the loader is		@@@@@@@@@@@@@@#                   #@@@@@@@@@@@@@
			invalid.									@@@@@@@@@@@@                        @@@@@@@@@@@@
   														@@@@@@@@@@@          _ @@@@@@@\     ``\@@@@@@@@@
  @version  1.0											@@@@@@@@%       %@@@@ ``*@@@@@@\_      \@@@@@@@@
														@@@@@@@*      +@@@@@  /@@#  `*@@@@\_    \@@@@@@@
														@@@@@@/      /@@@@@   /@@  @@@@@@@@@|    !@@@@@@
														@@@@/       /@@@@@@@%  *  /` ___*@@@|    |@@@@@@
														@@@#       /@@@@@@@@@       ###}@@@@|    |@@@@@@
														@@@@@|     |@@@@@@@@@         __*@@@      @@@@@@
 	 	 	 	 	 	 	 	 	 	 	 	 		@@@@@*     |@@@@@@@@@        /@@@@@@@/     '@@@@
														@@@@@@|    |@@ \@@          @@@@@@@@@      /@@@@
														@@@@@@|     |@@ _____     @@@@@@@@*       @@@@@@
														@@@@@@*     \@@@@@@@@@    @@@@@@@/         @@@@@
														@@@@@@@\     \@@@@@@@@@  @@@@@@@%        @@@@@@@
														@@@@@@@@\     \@@@@@@@@  @\  ‾‾‾           @@@@@@
														@@@@@@@@@@\    \@@@@@@@  @@/ _==> $     @@@@@@@@
														@@@@@@@@@@@@*    \@@@@@@@@@@@##‾‾   ``  @@@@@@@@@
														@@@@@@@@@@@@@@@@\     ___*@@`*    /@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@@@@@@--`@@@@__@@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
														@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@												*/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SLOT_1_APP_LOADER_ADDR 			((uint32_t)0x08004000U)			//FLASH_SECTOR_1_ADDR
#define UPDATER_ADDR					((uint32_t)0x08008000U)			//FLASH_SECTOR_2_ADDR
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * @function name    - JumpToUpdater
 *
 * @brief            - Function to jump to the updater (UPDATER_ADDR)
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This function is only called if application loader is invalid or not present
 */
void JumpToUpdater(void)
{
	fnc_ptr jump_to_updater;
	jump_to_updater = (fnc_ptr)(*(volatile uint32_t*) (UPDATER_ADDR+4U));

	HAL_RCC_DeInit();
	HAL_DeInit();

	SYSCFG->MEMRMP = 0x01;	//indicate that system memory is remapped to start at a different address

	SysTick->CTRL = 0; 		//disable SysTick

	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk ;	// PendSV clear

	// disable fault handlers

	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
					 SCB_SHCSR_BUSFAULTENA_Msk | \
					 SCB_SHCSR_MEMFAULTENA_Msk ) ;

	// move vector table to the new address
	SCB->VTOR = (uint32_t)UPDATER_ADDR;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)UPDATER_ADDR);
	__set_PSP(*(volatile uint32_t*)UPDATER_ADDR);

	jump_to_updater();
}

/*
 * @function name    - JumpToAppLoader
 *
 * @brief            - Function to jump to the application loader
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This function should be called by default at boot after system reset
 */
void JumpToAppLoader(void)
{
	fnc_ptr jump_to_app_loader;
	jump_to_app_loader = (fnc_ptr)(*(volatile uint32_t*) (SLOT_1_APP_LOADER_ADDR+4U));

	HAL_RCC_DeInit();
	HAL_DeInit();

	SYSCFG->MEMRMP = 0x01; 	//indicate that system memory is remapped to start at a different address

	SysTick->CTRL = 0; 		//disable SysTick

	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk ;

	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
					 SCB_SHCSR_BUSFAULTENA_Msk | \
					 SCB_SHCSR_MEMFAULTENA_Msk ) ;

	// move vector table to the new address
	SCB->VTOR = (uint32_t)SLOT_1_APP_LOADER_ADDR;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)SLOT_1_APP_LOADER_ADDR);
	__set_PSP(*(volatile uint32_t*)SLOT_1_APP_LOADER_ADDR);

	jump_to_app_loader();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int bl_main(void)
{

  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
	if(0xFFFFFFFF == *((uint32_t*)SLOT_1_APP_LOADER_ADDR))
	{
		// jump to UPDATER if LOADER is invalid or not present
		JumpToUpdater();
	}

	// jump to LOADER after boot
	JumpToAppLoader();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* SHOULD NOT REACH HERE*/
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
