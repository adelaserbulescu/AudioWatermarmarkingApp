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

volatile uint8_t temp;
EX_INTERRUPT_HANDLER(UART1_ISR)
{

	if(*pUART1_LSR & DR)
	{
		temp = *pUART1_RBR;
		*pPORTFIO_SET = (1<<5);
		*pUART1_IER |= ETBEI;

		// //debug led
	}
	if ((*pUART1_LSR & THRE) && (*pUART1_IER & ETBEI)) {

	        *pUART1_THR = temp;
	        *pUART1_IER &= ~ETBEI;
	    }


}

static volatile uint32_t  period;
static volatile uint8_t   phase;
static volatile uint8_t   done;
EX_INTERRUPT_HANDLER(TMR6_ISR)
{
	*pPORTFIO_SET = (1<<5);
	if(!(*pTIMER_STATUS & TIMIL6))
		return;

	*pTIMER_DISABLE = 1 << 6;
	*pTIMER_STATUS = TRUN6 | TOVF_ERR6 | TIMIL6;

	if(!phase) {
		period = *pTIMER6_PERIOD;
		*pTIMER6_CONFIG  = (uint16_t)(OUT_DIS | IRQ_ENA | PERIOD_CNT | PWM_OUT);
		*pTIMER_ENABLE = TIMEN6;
		phase = 1;
	}
	else {
		phase = 0;
		done = 1;
	}

}

	


