/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
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
    .version_minor 	= 0,
    .version_patch 	= 0
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart;
Ring_Buffer_t rxBuff, txBuff;
uint8_t rx_byte = 0;
uint8_t TxInProgress = RESET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void ShowMessage(ANIMATION_t frame);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// serial terminal related API
void clear_screen(void)
{
    USART_SendString((uint8_t*)"\033[2J");
}

void set_cursor_position(int x, int y)
{
    char buffer[32];
    sprintf(buffer, "\033[%d;%dH", y, x);
    USART_SendString((uint8_t*)buffer);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int app_main(void)
{

  /* USER CODE BEGIN 1 */
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
  memcpy(&huart, &huart2, sizeof(huart));
  __HAL_UART_ENABLE_IT(&huart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(&huart,UART_IT_RXNE);
  char ver[80];
  sprintf(ver, "Welcome to version %d.%d.%d\r\n",image_ver.version_major, image_ver.version_minor, image_ver.version_patch);
  USART_SendString((uint8_t*)ver);

  clear_screen();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  set_cursor_position(0, 0);
//	  HAL_GPIO_TogglePin(GPIOD, RED_Pin);
//	  ShowMessage(FRAME0);
//	  clear_screen();
//	  set_cursor_position(0, 0);
//	  HAL_GPIO_TogglePin(GPIOD, ORANGE_Pin);
//	  ShowMessage(FRAME1);
//	  clear_screen();
//	  set_cursor_position(0, 0);
//	  HAL_GPIO_TogglePin(GPIOD, GREEN_Pin);
//	  ShowMessage(FRAME2);
//	  clear_screen();
//	  set_cursor_position(0, 0);
	  HAL_GPIO_TogglePin(GPIOD, BLUE_Pin);
	  HAL_Delay(1000);
//	  ShowMessage(FRAME3);
//	  clear_screen();
//	  set_cursor_position(0, 0);
//	  ShowMessage(FRAME4);
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
  RCC_OscInitStruct.PLL.PLLN = 168;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  /* USER CODE BEGIN USART2_Init 2 */
  __HAL_UART_ENABLE_IT(&huart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(&huart,UART_IT_RXNE);
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GREEN_Pin ORANGE_Pin RED_Pin BLUE_Pin */
  GPIO_InitStruct.Pin = GREEN_Pin|ORANGE_Pin|RED_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void ShowMessage(ANIMATION_t frame)
{
    switch (frame)
    {
        case FRAME0:
        {
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
        	USART_SendString((uint8_t*)"                                                                                           \n\r");
            USART_SendString((uint8_t*)"       -.\\_.--._.______.-------.___.---------.___                                         \n\r");
            USART_SendString((uint8_t*)"       )`.                                       `-._                                     \n\r");
            USART_SendString((uint8_t*)"      (                                              `---.                                 \n\r");
            USART_SendString((uint8_t*)"      /o                                                  `.                              \n\r");
            USART_SendString((uint8_t*)"     (                                                      \\                             \n\r");
            USART_SendString((uint8_t*)"   _.'`.  _                                                  L                            \n\r");
            USART_SendString((uint8_t*)"   .'/| \"\" \"\"\"\"._                                            |                            \n\r");
            USART_SendString((uint8_t*)"      |          \\             |                             J                            \n\r");
            USART_SendString((uint8_t*)"                  \\-._          \\                             L                           \n\r");
            USART_SendString((uint8_t*)"                  /   `-.        \\                            J                           \n\r");
            USART_SendString((uint8_t*)"                 /      /`-.      )_                           `                          \n\r");
            USART_SendString((uint8_t*)"                /    .-'    `    J  \"\"\"\"-----.`-._             |\\                         \n\r");
            USART_SendString((uint8_t*)"              .'   .'        L   F            `-. `-.___        \\`.                       \n\r");
            USART_SendString((uint8_t*)"           ._/   .'          )  )                `-    .'""""`.  \\)                       \n\r");
            USART_SendString((uint8_t*)"__________((  _.'__       .-'  J              _.-'   .'        `. \\                       \n\r");
            USART_SendString((uint8_t*)"                   \"\"\"\"\"\"((  .'--.__________(   _.-'___________)..|----------------._____\n\r");
            USART_SendString((uint8_t*)"                            \"\"                \"\"\"               ``U'                      \n\r");
            break;
        }
        case FRAME1:
		{
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                               `.  \\                                     \n\r");
			USART_SendString((uint8_t*)"                                                 \\  \\                                     \n\r");
			USART_SendString((uint8_t*)"                                                  .  \\                                    \n\r");
			USART_SendString((uint8_t*)"                                                  :   .                                   \n\r");
			USART_SendString((uint8_t*)"                                                  |    .                                  \n\r");
			USART_SendString((uint8_t*)"                                                  |    :                                  \n\r");
			USART_SendString((uint8_t*)"                                                  |    |                                  \n\r");
			USART_SendString((uint8_t*)"  ..._  ___                                       |    |                                  \n\r");
			USART_SendString((uint8_t*)" `.\"\".`''''\"\"--..___                              |    |                                  \n\r");
			USART_SendString((uint8_t*)" ,-\\  \\             \"\"-...__         _____________/    |                                  \n\r");
			USART_SendString((uint8_t*)" / ` \" '                    `\"\"\"\"\"\"\"\"                  .                                  \n\r");
			USART_SendString((uint8_t*)" \\                                                      L                                 \n\r");
			USART_SendString((uint8_t*)" (>                                                      \\                                \n\r");
			USART_SendString((uint8_t*)"/                                                         \\                                \n\r");
			USART_SendString((uint8_t*)"\\_    ___..---.                                            L                               \n\r");
			USART_SendString((uint8_t*)"  `--'         '.                                           \\                              \n\r");
			USART_SendString((uint8_t*)"                 .                                           \\_                            \n\r");
			USART_SendString((uint8_t*)"                _/`.                                           `.._                        \n\r");
			USART_SendString((uint8_t*)"             .'     -.                                             `.                      \n\r");
			USART_SendString((uint8_t*)"            /     __.-Y     /''''''-...___,...--------.._            |                     \n\r");
			USART_SendString((uint8_t*)"           /   _.\"    |    /                ' .      \\   '---..._    |                     \n\r");
			USART_SendString((uint8_t*)"          /   /      /    /                _,. '    ,/           |   |                     \n\r");
			USART_SendString((uint8_t*)"          \\_,'     _.'   /              /''     _,-'            _|   |                     \n\r");
			USART_SendString((uint8_t*)"                  '     /               `-----''               /     |                     \n\r");
			USART_SendString((uint8_t*)"                  `...-'                                       !_____))                    \n\r");
			break;
		}
        case FRAME2:
        {
        	USART_SendString((uint8_t*)"                                                                                J    L     \n\r");
			USART_SendString((uint8_t*)"                                                                                |    |     \n\r");
			USART_SendString((uint8_t*)"                                                                                F    F     \n\r");
			USART_SendString((uint8_t*)"                                                                               J    J      \n\r");
			USART_SendString((uint8_t*)"                                                                              /    /       \n\r");
			USART_SendString((uint8_t*)"                                                                             /    /        \n\r");
			USART_SendString((uint8_t*)"                                                                           .'    /         \n\r");
			USART_SendString((uint8_t*)"                                                   .--\"\"--._              /     /          \n\r");
			USART_SendString((uint8_t*)"                                               _.-'         `-.          /     /           \n\r");
			USART_SendString((uint8_t*)"                                    __       .'                `.       /     /            \n\r");
			USART_SendString((uint8_t*)"                                 _-'-.\"`-.  J                    \\     /     /             \n\r");
			USART_SendString((uint8_t*)"                            .---(  `, _   `'|                     `.  J     /              \n\r");
			USART_SendString((uint8_t*)"                              `. )                                  \"\"     /               \n\r");
			USART_SendString((uint8_t*)"                               F                                          J                \n\r");
			USART_SendString((uint8_t*)"                               L                  |                      J                 \n\r");
			USART_SendString((uint8_t*)"                               ` (/     /         |                      F                 \n\r");
			USART_SendString((uint8_t*)"                                |    ._'          |                      |                 \n\r");
			USART_SendString((uint8_t*)"                               /'`--'`.           |                      J                 \n\r");
			USART_SendString((uint8_t*)"                               '||\\   |-._        `.  ___.               |                 \n\r");
			USART_SendString((uint8_t*)"                                 `     \\  `.        |/    `-            J                  \n\r");
			USART_SendString((uint8_t*)"                                        F   L       /       J           /                  \n\r");
			USART_SendString((uint8_t*)"                                        |   J      J         F         J                   \n\r");
			USART_SendString((uint8_t*)"                                        |    \\     J         |        J                    \n\r");
			USART_SendString((uint8_t*)"                                        |    |L    J         J        J                    \n\r");
			USART_SendString((uint8_t*)"                                        |    FJ    |          L       |                    \n\r");
			USART_SendString((uint8_t*)"                                        |   J  L   |          L\\      F                    \n\r");
			USART_SendString((uint8_t*)"                                        |   F  |   |           L\\    J                     \n\r");
			USART_SendString((uint8_t*)"                                        F  F   |   |           | L   |                     \n\r");
			USART_SendString((uint8_t*)"                                       J  J    |   |           | |   F                     \n\r");
			USART_SendString((uint8_t*)"                                       /  )    F  J            F F  J                      \n\r");
			USART_SendString((uint8_t*)"                                      ( .'    )   F           J J  F                       \n\r");
			USART_SendString((uint8_t*)"                                      `\"     (   J           /.'  J                        \n\r");
			USART_SendString((uint8_t*)"                                              `-'           ||-' |)                        \n\r");
			USART_SendString((uint8_t*)"                                                              '-')                         \n\r");
			break;
        }
        case FRAME3:
		{
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"					                         .-""-.__                                     \n\r");
			USART_SendString((uint8_t*)"				             _.--\"\"\"--._____.-'        `-._                            \n\r");
			USART_SendString((uint8_t*)"				           .-'                              `.                            \n\r");
			USART_SendString((uint8_t*)"                                         .'                        _.._       `.           \n\r");
			USART_SendString((uint8_t*)"                                        /                       J\"\"    `-.      \\          \n\r");
			USART_SendString((uint8_t*)"                                       /                         \\        `-.    `.        \n\r");
			USART_SendString((uint8_t*)"                                     .'          |               F           \\     \\       \n\r");
			USART_SendString((uint8_t*)"                                     F           |              /             `.    \\      \n\r");
			USART_SendString((uint8_t*)"                               _.---<_           J             J                `-._/      \n\r");
			USART_SendString((uint8_t*)"                             ./`.     `.          \\          J F                           \n\r");
			USART_SendString((uint8_t*)"                          __/ F  )                 \\          \\                            \n\r");
			USART_SendString((uint8_t*)"                        <     |                \\    >.         L                           \n\r");
			USART_SendString((uint8_t*)"                         `-.                    |  J  `-.      )                           \n\r");
			USART_SendString((uint8_t*)"                           J         |          | /      `-    F                           \n\r");
			USART_SendString((uint8_t*)"                            \\  o     /         (|/        //  /                           \n\r");
			USART_SendString((uint8_t*)"                             `.    )'.          |        /'  /                            \n\r");
			USART_SendString((uint8_t*)"                              `---'`\\ `.        |        /  /                             \n\r");
			USART_SendString((uint8_t*)"                              '|\\\\ \\ `. `.      |       /( /                               \n\r");
			USART_SendString((uint8_t*)"                                 `` `  \\  \\     |       \\_'                                \n\r");
			USART_SendString((uint8_t*)"                                        L  L    |     .' /                                 \n\r");
			USART_SendString((uint8_t*)"                                        |  |    |    (_.'                                  \n\r");
			USART_SendString((uint8_t*)"                                        J  J    F                                          \n\r");
			USART_SendString((uint8_t*)"                                        J  J   J                                           \n\r");
			USART_SendString((uint8_t*)"                                        J  J   J                                           \n\r");
			USART_SendString((uint8_t*)"                                        J  J   F                                           \n\r");
			USART_SendString((uint8_t*)"                                        |  |  J                                            \n\r");
			USART_SendString((uint8_t*)"                                        |  F  F                                            \n\r");
			USART_SendString((uint8_t*)"                                        F /L  |                                            \n\r");
			USART_SendString((uint8_t*)"                                        `' \\_)'                                            \n\r");
			break;
		}
        case FRAME4:
		{
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"                                                                                           \n\r");
			USART_SendString((uint8_t*)"				           ______  .----.___                                               \n\r");
			USART_SendString((uint8_t*)"                                       .--'      `' `-      `-.                            \n\r");
			USART_SendString((uint8_t*)"                                    .-'                        `.                          \n\r");
			USART_SendString((uint8_t*)"                                __.'-                            `.                        \n\r");
			USART_SendString((uint8_t*)"                              .'                                   `.                       \n\r");
			USART_SendString((uint8_t*)"                   _.---._   /                                       `.                     \n\r");
			USART_SendString((uint8_t*)"                 .'       \"-'                                          `.                   \n\r");
			USART_SendString((uint8_t*)"             .--'                                              .         `.                 \n\r");
			USART_SendString((uint8_t*)"             `._.-\"                                             \\._        `.               \n\r");
			USART_SendString((uint8_t*)"               <_`-.                            .'              |  `.        `.             \n\r");
			USART_SendString((uint8_t*)"                |`                   .'     .--'._            .'     `-.       `.           \n\r");
			USART_SendString((uint8_t*)"                |         |/         |   .-'      `---.__.---'          `.       `.         \n\r");
			USART_SendString((uint8_t*)"                 \\``    _.-          | .'                                 `.       `.      \n\r");
			USART_SendString((uint8_t*)"                  L   //F `.         |/                                     `-.      \\     \n\r");
			USART_SendString((uint8_t*)"                  |\"\"\"\\J    \\        |                                         `-.    \\    \n\r");
			USART_SendString((uint8_t*)"                   |||\\F   .'\\       |                                            `-._/   \n\r");
			USART_SendString((uint8_t*)"                    |`J  .'   \\|     F                                                     \n\r");
			USART_SendString((uint8_t*)"                    ` F |      )    J                                                      \n\r");
			USART_SendString((uint8_t*)"                     J  F      |    J                                                      \n\r");
			USART_SendString((uint8_t*)"                     |  F      |    F                                                      \n\r");
			USART_SendString((uint8_t*)"                     \\_/       |   J                                                       \n\r");
			USART_SendString((uint8_t*)"                               |   )                                                       \n\r");
			USART_SendString((uint8_t*)"                               F  J                                                        \n\r");
			USART_SendString((uint8_t*)"                              J   |                                                        \n\r");
			USART_SendString((uint8_t*)"                             ')   F                                                        \n\r");
			USART_SendString((uint8_t*)"                             /.   |                                                        \n\r");
			USART_SendString((uint8_t*)"                             ', J'                                                         \n\r");
			USART_SendString((uint8_t*)"                             `-''                                                          \n\r");
			break;
		}
        default:
            break;
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
