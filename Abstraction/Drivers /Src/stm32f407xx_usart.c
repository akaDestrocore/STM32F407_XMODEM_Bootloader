/*										@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
										@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
										@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
										@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@             @@@         %   @@
										@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
										@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
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

#include <stm32f407xx_usart.h>




/*
 * Peripheral Clock setup
 */
/********************************************************************************************************/
/* @function name 		- USART_SetBaudRate																*/
/*																										*/
/* @brief				- This function sets needed baud rate of USART communication					*/
/*																										*/
/* @parameter[in]		- pointer to USART peripheral base address										*/
/*																										*/
/* @parameter[in]		- wanted baud rate																*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variable to hold fraction value
	uint32_t F_part;

	//Get the value of APB bus clock in to the variable PCLKx
	if(pUSARTx == USART1 || pUSARTx == USART6)
	{
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Freq();
	}else
	{
	   PCLKx = RCC_GetPCLK1Freq();
	}

	//Check for OVER8 configuration bit
	if(1 == pUSARTx->CR1.bit.over8)
	{
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	}else
	{
	   //over sampling by 16
	   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
	}

	//Calculate the Mantissa part
	pUSARTx->BRR.bit.div_mantissa = usartdiv/100;

	//Extract the fraction part
	F_part = (usartdiv - (pUSARTx->BRR.bit.div_mantissa * 100));

	//Calculate the final fractional
	if(1 == pUSARTx->CR1.bit.over8)
	{
	  //OVER8 = 1 , over sampling by 8
	  pUSARTx->BRR.bit.div_fraction = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

	}else
	{
	   //over sampling by 16
	   pUSARTx->BRR.bit.div_fraction = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

	}
}

/********************************************************************************************************/
/* @function name 		- USART_Init																	*/
/*																										*/
/* @brief				- USART initialization function													*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_Init(USART_Handle_t *pUSARTHandle)
{

	//Temporary handle
	USART_CR1_Reg_t CR1_temp;

/******************************** Configuration of CR1******************************************/

	//enable the Clock for given USART peripheral
	 USART_PeriClockControl(pUSARTHandle->pUSARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if (USART_MODE_ONLY_RX ==  pUSARTHandle->USART_Config.USART_Mode)
	{
		//enable the Receiver bit field
		CR1_temp.bit.re = ENABLE;

	}else if (USART_MODE_ONLY_TX == pUSARTHandle->USART_Config.USART_Mode)
	{
		//enable the Transmitter bit field
		CR1_temp.bit.te = ENABLE;

	}else if (USART_MODE_TXRX == pUSARTHandle->USART_Config.USART_Mode)
	{
		//enable the both Transmitter and Receiver bit fields
		CR1_temp.bit.te = ENABLE;
		CR1_temp.bit.re = ENABLE;
	}

    //configure the Word length configuration item
	CR1_temp.bit.m = pUSARTHandle->USART_Config.USART_WordLength;


    //Configuration of parity control bit fields
	if (USART_PARITY_EN_EVEN ==  pUSARTHandle->USART_Config.USART_ParityControl)
	{
		//enable the parity control
		CR1_temp.bit.pce = ENABLE;

		//by default EVEN parity will be selected once you enable the parity control

	}else if (USART_PARITY_EN_ODD == pUSARTHandle->USART_Config.USART_ParityControl)
	{
		//enable the parity control
		CR1_temp.bit.pce = ENABLE;

	    //enable ODD parity
		CR1_temp.bit.ps = ENABLE;

	}

   //Program the CR1 register
	pUSARTHandle->pUSARTx->CR1.reg = CR1_temp.reg;

/******************************** Configuration of CR2******************************************/

	//configure the number of stop bits inserted during USART frame transmission
	pUSARTHandle->pUSARTx->CR2.bit.stop = pUSARTHandle->USART_Config.USART_NoOfStopBits;

/******************************** Configuration of CR3******************************************/

	USART_CR3_Reg_t CR3_temp;

	//Configuration of USART hardware flow control
	if (USART_HW_FLOW_CTRL_CTS ==  pUSARTHandle->USART_Config.USART_HWFlowControl)
	{
		//enable CTS flow control
		CR3_temp.bit.ctse = ENABLE;

	}else if (USART_HW_FLOW_CTRL_RTS == pUSARTHandle->USART_Config.USART_HWFlowControl)
	{
		//enable RTS flow control
		CR3_temp.bit.rtse = ENABLE;

	}else if (USART_HW_FLOW_CTRL_CTS_RTS == pUSARTHandle->USART_Config.USART_HWFlowControl)
	{
		//enable both CTS and RTS Flow control
		CR3_temp.bit.ctse = ENABLE;
		CR3_temp.bit.rtse = ENABLE;
	}


	pUSARTHandle->pUSARTx->CR3.reg = CR3_temp.reg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//configure the baud rate
	USART_SetBaudRate(pUSARTHandle->pUSARTx,pUSARTHandle->USART_Config.USART_Baud);

/***************************************** Enable USART peripheral ************************************************/
	USART_PeripheralControl(pUSARTHandle->pUSARTx, ENABLE);
}



/********************************************************************************************************/
/* @function name 		- USART_DeInit																	*/
/*																										*/
/* @brief				- USART de-initialization function												*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_DeInit(USART_Handle_t *pUSARTHandle)
{
	if(pUSARTHandle->pUSARTx == USART1)
	{
		RCC->APB2RSTR.bit.usart1rst = ENABLE;
	}
	else if(pUSARTHandle->pUSARTx == USART2)
	{
		RCC->APB1RSTR.bit.usart2rst = ENABLE;
	}
	else if(pUSARTHandle->pUSARTx == USART3)
	{
		RCC->APB1RSTR.bit.usart3rst = ENABLE;
	}
	else if(pUSARTHandle->pUSARTx == UART4)
	{
		RCC->APB1RSTR.bit.uart4rst = ENABLE;
	}
	else if(pUSARTHandle->pUSARTx == UART5)
	{
		RCC->APB1RSTR.bit.uart5rst = ENABLE;
	}
	else if(pUSARTHandle->pUSARTx == USART6)
	{
		RCC->APB2RSTR.bit.usart6rst = ENABLE;
	}
}



/********************************************************************************************************/
/* @function name 		- USART_PeripheralControl														*/
/*																										*/
/* @brief				- USART enable or disable function												*/
/*																										*/
/* @parameter[in]		- pointer to USART peripheral base address										*/
/*																										*/
/* @parameter[in]		- ENABLE or DISABLE macro or 1/0												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t state)
{
	if(ENABLE == state)
	{
		pUSARTx->CR1.bit.ue = ENABLE;
	}else
	{
		pUSARTx->CR1.bit.ue = DISABLE;
	}
}



/********************************************************************************************************/
/* @function name 		- USART_PeriClockControl														*/
/*																										*/
/* @brief				- USART clock enable/disable function											*/
/*																										*/
/* @parameter[in]		- pointer to USART peripheral base address										*/
/*																										*/
/* @parameter[in]		- ENABLE or DISABLE macro or 1/0												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t state)
{
	if(ENABLE == state)
	{
		if(pUSARTx == USART1)
		{
			RCC->APB2ENR.bit.usart1en = ENABLE;
		}else if (pUSARTx == USART2)
		{
			RCC->APB1ENR.bit.usart2en = ENABLE;
		}else if (pUSARTx == USART3)
		{
			RCC->APB1ENR.bit.usart3en = ENABLE;
		}
		else if (pUSARTx == UART4)
		{
			RCC->APB1ENR.bit.uart4en = ENABLE;
		}else if (pUSARTx == UART5)
		{
			RCC->APB1ENR.bit.uart5en = ENABLE;
		}else if (pUSARTx == USART6)
		{
			RCC->APB2ENR.bit.usart6en = ENABLE;
		}
	}
	else
	{
		if(pUSARTx == USART1)
		{
			RCC->APB2ENR.bit.usart1en = DISABLE;
		}else if (pUSARTx == USART2)
		{
			RCC->APB1ENR.bit.usart2en = DISABLE;
		}else if (pUSARTx == USART3)
		{
			RCC->APB1ENR.bit.usart3en = DISABLE;
		}
		else if (pUSARTx == UART4)
		{
			RCC->APB1ENR.bit.uart4en = DISABLE;
		}else if (pUSARTx == UART5)
		{
			RCC->APB1ENR.bit.uart5en = DISABLE;
		}else if (pUSARTx == USART6)
		{
			RCC->APB2ENR.bit.usart6en = DISABLE;
		}
	}

}

/********************************************************************************************************/
/* @function name 		- USART_GetFlagStatus															*/
/*																										*/
/* @brief				- USART status register flag function											*/
/*																										*/
/* @parameter[in]		- pointer to USART peripheral base address										*/
/*																										*/
/* @parameter[in]		- flag name, refer to @USART flags												*/
/*																										*/
/* @return				- flag state																	*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, USART_SR_flag_t StatusFlagName)
{
	switch(StatusFlagName)
	{
		case USART_SR_PE:
		{
			return pUSARTx->SR.bit.pe;
		}
		case USART_SR_FE:
		{
			return pUSARTx->SR.bit.fe;
		}
		case USART_SR_NF:
		{
			return pUSARTx->SR.bit.nf;
		}
		case USART_SR_ORE:
		{
			return pUSARTx->SR.bit.ore;
		}
		case USART_SR_IDLE:
		{
			return pUSARTx->SR.bit.idle;
		}
		case USART_SR_RXNE:
		{
			return pUSARTx->SR.bit.rxne;
		}
		case USART_SR_TC:
		{
			return pUSARTx->SR.bit.tc;
		}
		case USART_SR_TXE:
		{
			return pUSARTx->SR.bit.txe;
		}
		case USART_SR_LBD:
		{
			return pUSARTx->SR.bit.lbd;
		}
		case USART_SR_CTS:
		{
			return pUSARTx->SR.bit.cts;
		}
		default:
			return 0;

	}
}

/********************************************************************************************************/
/* @function name 		- USART_SendData																*/
/*																										*/
/* @brief				- USART data transmission function												*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @parameter[in]		- pointer to TX buffer															*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- blocking function																*/
/********************************************************************************************************/
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;

   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TXE))
		{

		}

		//Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(USART_WORDLEN_9BITS == pUSARTHandle->USART_Config.USART_WordLength)
		{
			//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR.bit.dr = *pdata;

			//check for USART_ParityControl
			if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
			{
				//No parity is used in this transfer , so 9bits of user data will be sent
				//increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR.reg = (*pTxBuffer  & (uint8_t)0xFF);

			//increment the buffer address
			pTxBuffer++;
		}
	}

	//wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TC));
}

/********************************************************************************************************/
/* @function name 		- USART_ReceiveData																*/
/*																										*/
/* @brief				- USART data reception function													*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @parameter[in]		- pointer to RX buffer															*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- blocking function																*/
/********************************************************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//wait until RXNE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_RXNE))
		{

		}

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(USART_WORDLEN_9BITS == pUSARTHandle->USART_Config.USART_WordLength)
		{
			//We are going to receive 9bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
			{
				//No parity is used , so all 9bits will be of user data

				//read only first 9 bits
				*((uint16_t*) pRxBuffer) = pUSARTHandle->pUSARTx->DR.bit.dr;

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so 8bits will be of user data and 1 bit is parity
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0xFF);
				 pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				*pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0xFF);
			}
			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0x7F);

			}

			//Now , increment the pRxBuffer
			pRxBuffer++;
		}
	}

}


/********************************************************************************************************/
/* @function name 		- USART_SendDataIT																*/
/*																										*/
/* @brief				- USART data transmission non blocking function									*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @parameter[in]		- pointer to TX buffer															*/
/*																										*/
/* @return				- TX state																		*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if(USART_BUSY_IN_TX != txstate)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1.bit.txeie = ENABLE;


		//enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1.bit.tcie = ENABLE;

	}

	return txstate;
}


/********************************************************************************************************/
/* @function name 		- USART_ReceiveDataIT															*/
/*																										*/
/* @brief				- USART data reception non blocking function									*/
/*																										*/
/* @parameter[in]		- pointer to USART handle address												*/
/*																										*/
/* @parameter[in]		- pointer to КX buffer															*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(USART_BUSY_IN_RX == rxstate)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		(void)pUSARTHandle->pUSARTx->DR;

		//enable interrupt for RXNE
		pUSARTHandle->pUSARTx->CR1.bit.rxneie = ENABLE;

	}

	return rxstate;
}

/********************************************************************************************************/
/* @function name 		- USART_ClearFlag																*/
/*																										*/
/* @brief				- USART status register CTS, LBD, TC flag reset									*/
/*																										*/
/* @parameter[in]		- pointer to USART peripheral base address										*/
/*																										*/
/* @parameter[in]		- flag name																		*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
	switch(StatusFlagName)
	{
		case USART_SR_PE:
		{
			pUSARTx->SR.bit.pe = DISABLE;
		}
		case USART_SR_FE:
		{
			pUSARTx->SR.bit.fe = DISABLE;
		}
		case USART_SR_NF:
		{
			pUSARTx->SR.bit.nf = DISABLE;
		}
		case USART_SR_ORE:
		{
			pUSARTx->SR.bit.ore = DISABLE;
		}
		case USART_SR_IDLE:
		{
			pUSARTx->SR.bit.idle = DISABLE;
		}
		case USART_SR_RXNE:
		{
			pUSARTx->SR.bit.rxne = DISABLE;
		}
		case USART_SR_TC:
		{
			pUSARTx->SR.bit.tc = DISABLE;
		}
		case USART_SR_TXE:
		{
			pUSARTx->SR.bit.txe = DISABLE;
		}
		case USART_SR_LBD:
		{
			pUSARTx->SR.bit.lbd = DISABLE;
		}
		case USART_SR_CTS:
		{
			pUSARTx->SR.bit.cts = DISABLE;
		}
		default:
			return;

	}

}

/********************************************************************************************************/
/* @function name 		- USART_IRQInterruptConfig														*/
/*																										*/
/* @brief				- USART interruption configuration function										*/
/*																										*/
/* @parameter[in]		- IRQ Number																	*/
/*																										*/
/* @parameter[in]		- IRQ Priority																	*/
/*																										*/
/* @parameter[in]		- state (ENABLE or DISABLE)														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_IRQInterruptConfig(uint8_t IRQNumber,uint32_t IRQPriority,uint8_t state)
{

	if(ENABLE == state)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			NVIC->ISER[0] |= ( 1 << IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64) //32 to 63
		{
			//program ISER1 register
			NVIC->ISER[1] |= ( 1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ISER2 register //64 to 95
			NVIC->ISER[2] |= ( 1 << (IRQNumber % 64));
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			NVIC->ICER[0] |= ( 1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program ICER1 register
			NVIC->ICER[1] |= ( 1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			NVIC->ICER[2] |= ( 1 << (IRQNumber % 64));
		}
	}

	//first lets find out the ipr register
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section  = IRQNumber%4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + 4;

	*(NVIC_IPR_BASE + iprx ) |= (IRQPriority << shift_amount);
}

/********************************************************************************************************/
/* @function name 		- USART_IRQHandler																*/
/*																										*/
/* @brief				- USART interruption handler													*/
/*																										*/
/* @parameter[in]		- pointer to USART handle														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

	uint32_t temp1 , temp2, temp3;

	uint16_t *pdata;

/*************************Check for TC flag ********************************************/

    //check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR.bit.tc;

	 //check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1.bit.tcie;

	if(temp1 && temp2 )
	{
		//this interrupt occured because of TC

		//close transmission and call application callback if TxLen is zero
		if (USART_BUSY_IN_TX == pUSARTHandle->TxBusyState)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! pUSARTHandle->TxLen )
			{
				//clear the TC flag
				pUSARTHandle->pUSARTx->SR.bit.tc = DISABLE;

				//clear the TCIE control bit
				pUSARTHandle->pUSARTx->CR1.bit.tcie = DISABLE;

				//reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				//reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				//reset the length to zero
				pUSARTHandle->TxLen = 0;

				//Call the application call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	//check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR.bit.txe;

	//check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1.bit.txeie;


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE

		if(USART_BUSY_IN_TX == pUSARTHandle->TxBusyState)
		{
			//Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->TxLen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(USART_WORDLEN_9BITS == pUSARTHandle->USART_Config.USART_WordLength)
				{
					//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
					pdata = (uint16_t*) pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->DR.bit.dr = *pdata;

					//check for USART_ParityControl
					if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
					{
						//No parity is used in this transfer , so 9bits of user data will be sent
						//increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=2;
					}
					else
					{
						//Parity bit is used in this transfer . so 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=1;
					}
				}
				else
				{
					//This is 8bit data transfer
					pUSARTHandle->pUSARTx->DR.reg = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

					//increment the buffer address
					pUSARTHandle->pTxBuffer++;
					pUSARTHandle->TxLen-=1;
				}

			}
			if (0 == pUSARTHandle->TxLen)
			{
				//TxLen is zero
				//clear the TXEIE bit (disable interrupt for TXE flag )
				pUSARTHandle->pUSARTx->CR1.bit.txeie = DISABLE;
			}
		}
	}

/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR.bit.rxne;
	temp2 = pUSARTHandle->pUSARTx->CR1.bit.rxneie;

	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		if(USART_BUSY_IN_RX == pUSARTHandle->RxBusyState)
		{
			if(pUSARTHandle->RxLen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(USART_WORDLEN_9BITS == pUSARTHandle->USART_Config.USART_WordLength)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
					{
						//No parity is used , so all 9bits will be of user data

						//read only first 9 bits
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR.bit.dr);

						//Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->RxLen-=2;
					}
					else
					{
						//Parity is used, so 8bits will be of user data and 1 bit is parity
						 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0xFF);
						 pUSARTHandle->pRxBuffer++;
						 pUSARTHandle->RxLen-=1;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(USART_PARITY_DISABLE == pUSARTHandle->USART_Config.USART_ParityControl)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0xFF);

					}

					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR.reg  & (uint8_t)0x7F);

					}

					//Now , increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;
					 pUSARTHandle->RxLen-=1;
				}


			}//if of >0

			if(! pUSARTHandle->RxLen)
			{
				//disable the RXNE
				pUSARTHandle->pUSARTx->CR1.bit.rxneie = DISABLE;
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}


/*************************Check for CTS flag ********************************************/
//Note : CTS feature is not applicable for UART4 and UART5

	//check the status of CTS bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR.bit.cts;

	//check the state of CTSE bit in CR3
	temp2 = pUSARTHandle->pUSARTx->CR3.bit.ctse;

	//check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = pUSARTHandle->pUSARTx->CR3.bit.ctsie;


	if(temp1  && temp2 )
	{
		//clear the CTS flag in SR
		pUSARTHandle->pUSARTx->SR.bit.cts = DISABLE;

		//this interrupt is because of cts
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

	if(0 != temp3)
	{
		//clear the CTS flag in SR
		pUSARTHandle->pUSARTx->SR.bit.cts = DISABLE;

		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

/*************************Check for IDLE detection flag ********************************************/

	//check the status of IDLE flag bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR.bit.idle;

	//check the state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1.bit.idleie;


	if(temp1 && temp2)
	{
		//clear the IDLE flag. Refer to the RM to understand the clear sequence
		pUSARTHandle->pUSARTx->SR.bit.idle = DISABLE;

		//this interrupt is because of idle
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_IDLE);
	}

/*************************Check for Overrun detection flag ********************************************/

	//check the status of ORE flag  in the SR
	temp1 = pUSARTHandle->pUSARTx->SR.bit.ore;

	//check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->CR1.bit.rxneie;


	if(temp1  && temp2)
	{
		//Need not to clear the ORE flag here, instead give an API for the application to clear the ORE flag.

		//this interrupt is because of Overrun error
		USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
	}



/*************************Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  pUSARTHandle->pUSARTx->CR3.bit.eie;

	USART_SR_Reg_t SR_temp;

	if(temp2 )
	{
		SR_temp.reg = pUSARTHandle->pUSARTx->SR.reg;
		if(SET == SR_temp.bit.fe)
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_FE);
		}

		if(SET == SR_temp.bit.nf)
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is cleared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_NF);
		}

		if(SET == SR_temp.bit.ore)
		{
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
		}
	}


}

__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event)
{
	return;
}

/****************************************************** End of file *************************************************************/
