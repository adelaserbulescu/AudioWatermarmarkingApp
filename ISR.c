#include "Talkthrough.h"

//--------------------------------------------------------------------------//
// Function:	Sport0_RX_ISR												//
//																			//
// Description: This ISR is executed after a complete frame of input data 	//
//				has been received. The new samples are stored in 			//
//				iChannel0LeftIn and iChannel0RightIn.  Then the function 	//
//				Process_Data() is called in which user code can be executed.//
//				After that the processed values are copied from the 		//
//				variables iChannel0LeftOut and iChannel0RightOut into the  	//
//				DMA transmit buffer.										//
//--------------------------------------------------------------------------//
EX_INTERRUPT_HANDLER(Sport0_RX_ISR)
{
	// confirm interrupt handling
	*pDMA3_IRQ_STATUS = 0x0001;

	// copy input data from dma input buffer into variables
	iChannel0LeftIn = iRxBuffer1[INTERNAL_ADC_L0];
	iChannel0RightIn = iRxBuffer1[INTERNAL_ADC_R0];
	
	// call function that contains user code
	Process_Data();				

	// copy processed data from variables into dma output buffer
	iTxBuffer1[INTERNAL_DAC_L0] = iChannel0LeftOut;
	iTxBuffer1[INTERNAL_DAC_R0] = iChannel0RightOut;

}

EX_INTERRUPT_HANDLER(TIM0_ISR)
{
	*pTIMER_STATUS = TIMIL0;
		state ^= 1;
		fillTX(state);
}

volatile uint8_t state;
EX_INTERRUPT_HANDLER(UART1_RX_ISR)
{
	if(*pUART1_LSR & (OE | PE |FE | BI)) {
			volatile uint8_t dummy = *pUART1_RBR;
			frame_state = 0;  // reset state machine
			return;
		}
		if(*pUART1_LSR & DR)
		{
			uint8_t rx = *pUART1_RBR;
		    readRX(rx);
		}
}

	


