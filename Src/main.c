
/* Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "xmodem.h"
#include "stm32f4xx_hal.h"
#include "simple_delay.h"
#include "circularBuffer.h"
#include "uart_init.h"
#include "led_init.h"
#include "basic_flash_lib.h"
#include "button_init.h"
#include "string.h"


/* Private structures ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct CircularBuffer rxCB,txCB;

xmodem_state_t state;

/* Flags ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern uint8_t TxInProgress;

uint8_t first_byte_already_received;
uint8_t copy_data;


uint8_t rx_byte = 0, prev_index1, prev_index2 = 0xFF, dataCounter, crc;
uint8_t a_readRxData[132] = "\0";
uint8_t a_actual_data[128] = "\0";
uint8_t sum;

/* Private function prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SystemClock_Config(void);
uint8_t sumofarray(uint8_t array[], uint16_t length);


//uint8_t a_Write_Test[54] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
//		,'\0','\0','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
//uint8_t a_Read_Test[54];
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
int main(void)
{


  /* Configure the system clock */
  SystemClock_Config();
  initializeCircularBuffer(&rxCB);
  initializeCircularBuffer(&txCB);
  UART4_Config(APBx_freq,BAUD_RATE);
  LED_GPIO_Config();
  Button_GPIO_Config();
  BFL_FLASH_Init(DATA_TYPE_8b);

  //welcome message
  UART4_SendString((uint8_t*)">>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<\n\r\n\r");
  UART4_SendString((uint8_t*)"Welcome to XMODEM 8-bit CRC bootloader!\n\r");
  UART4_SendString((uint8_t*)">>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<\n\r\n\r");



  //if the button is pressed, then jump to the user application, otherwise stay in the bootloader
 if(1 != ReadButton())
 {
	 UART4_SendString((uint8_t*)"Jumping to user application...\n\r");
	 BFL_Jump();
 }

 first_byte_already_received = 0;
 copy_data = 1;
	/* Ask for new data and start the Xmodem protocol. */
	UART4_SendString((uint8_t*)"Please send a new binary file with Xmodem protocol to update the firmware.\n\r");
	state = WAIT_SOH;
  while (1)
  {
	  if(first_byte_already_received == 0)
	  {
		  UART4_SendByte(X_NAK);
		  delay_ms(200);
	  }

  if(state == WAIT_SOH && first_byte_already_received == 1)
	{
	  UART4_SendByte(X_ACK);												//send ACK after flashing is also over
	}
	readCircularBuffer( &rxCB, &rx_byte );									//get data from host
	if(rxCB.data[0] == X_EOT){UART4_SendByte(X_ACK); first_byte_already_received = 1; rxCB.head = 0; rxCB.tail = 0; rxCB.dataCount = 0;}
	if(rxCB.data[0] == X_ETB){UART4_SendByte(X_ACK); LED_TurnOff(GREEN);BFL_Jump();}
	if(copy_data == 1)
	{
		for(dataCounter = 0; dataCounter < 132; dataCounter++){ a_readRxData[dataCounter] = rxCB.data[dataCounter];}
		first_byte_already_received = 0;
		rxCB.head = 0;
		rxCB.tail = 0;														//empty the ring buffer
		rxCB.dataCount = 0;
	}
		  switch(state){
		  	  case WAIT_SOH:
		  		   if(a_readRxData[0] == X_SOH){ state = WAIT_INDEX1; first_byte_already_received = 1; copy_data = 0;}
		  		   else{first_byte_already_received = 0;}
		  		   break;
		  	  case WAIT_INDEX1:
		  		   if(a_readRxData[1] == prev_index1+1){ state = WAIT_INDEX2; prev_index1++;}
		  		  else{ /*error*/ }
		  		  break;
		  	  case WAIT_INDEX2:
		  		   if(a_readRxData[2] == prev_index2-1){ state = READ_DATA; prev_index2--;}
		  		 else{ /*error*/ }
		  		  break;
		  	  case READ_DATA:
		  			  for(dataCounter = 0; dataCounter < 129; dataCounter++){ a_actual_data[dataCounter] = a_readRxData[dataCounter+3];}
		  			  state = WAIT_CHKSM;
		  		 break;
		  	  case WAIT_CHKSM:
		  		  	  sum = sumofarray(a_actual_data,128);
		  		  	  crc = sum%256;
		  		  	  if(crc == a_readRxData[131])
		  		  	  {
		  		  		//turn on the green LED to indicate that we are in bootloader mode
		  		  			BFL_FLASH_Erase(FLASH_APP_START_ADDRESS);
		  		  			LED_TurnOff(ORANGE);
		  		  			LED_LightUp(GREEN);
		  		  		  if(a_readRxData[1] == X_SOH)
		  		  		  {
		  		  			BFL_FLASH_Write(a_actual_data, 128, FLASH_APP_START_ADDRESS);
		  		  			memset(rxCB.data, 0, sizeof rxCB.data);
		  		  			memset(a_readRxData, 0, sizeof rxCB.data);
		  		  			first_byte_already_received = 1; copy_data = 1; state = WAIT_SOH;
		  		  		  }
		  		  		  if(a_readRxData[1] > X_SOH)
		  		  		  {
		  		  			BFL_FLASH_Write(a_actual_data, 128, FLASH_APP_START_ADDRESS+((rxCB.data[1]-1)*128));
		  		  			memset(rxCB.data, 0, sizeof rxCB.data);
		  		  			memset(a_readRxData, 0, sizeof rxCB.data);
		  		  			first_byte_already_received = 1;
		  		  			copy_data = 1;
		  		  			state = WAIT_SOH;
		  		  		  }
		  		  	  }
		  		 break;
		  	  	  }

  }
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SUM OF ARRAY FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint8_t sumofarray(uint8_t array[], uint16_t length)
{
    int sum = 0;
    // Iterate through all elements and add them to sum
    for (int i = 0; i < length; i++)
        sum += array[i];
    return sum;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART INTERRUPT HANDLER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_IRQHandler(void)
{
	if ((UART4->SR & USART_SR_RXNE) && (UART4->CR1 & USART_CR1_RXNEIE)) 	//if RX is not empty and RX interrupt is enabled
	{
//		uint32_t read_SR = UART4->SR;										//read status register first

		writeCircularBuffer(&rxCB, UART4->DR);								//save from RX to RX ring buffer so data is not lost
	}
	if((UART4->SR & USART_SR_TXE) && (UART4->CR1 & USART_CR1_TXEIE))		//if TX is empty and TX interrupt is enabled
	{
		TxInProgress = 0;
		initiateTransmission(&txCB);					//check if TX has any data and if it does save it to temp, then print temp and turn on TXEIE
//		UART4->CR1 &= ~USART_CR1_TXEIE;

	}
 }
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */






/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SYSTEM CLOCK CONFIGURATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
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
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
