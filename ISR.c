#include "Talkthrough.h"
#include <string.h>

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
	if(received_bytes[received_bytes_index+1] == '\n') {
		*pUART1_IER = 0;
		ssync();
		return;
	}

}

volatile char received_bytes[25];
volatile int received_bytes_index = 0;
volatile int uart_isr_count = 0;
volatile int error_condition_hit;
volatile int stop_wait;
volatile int oe;
volatile int pe;
volatile int fe;
volatile int bi;
EX_INTERRUPT_HANDLER(UART1_RX_ISR)
{

	if(!uart_isr_count) for(int i = 0; i < 1000000; i++);
	if(uart_isr_count == sizeof(rx_buffer))sti(EVT_IVG12);
	volatile int lsr = *pUART1_LSR;


	if(lsr & (OE | PE |FE | BI)) {
		oe = lsr & OE;
		    pe = lsr & PE;
		    fe = lsr & FE;
		    bi = lsr & BI;
		    error_condition_hit++;
			volatile char dummy = *pUART1_RBR;
			//frame_state = 0;  // reset state machine
			uart_isr_count++;
			return;
		}

	while(!(*pUART1_LSR & DR));
    char rx = *pUART1_RBR;
    //rx += 94;
    if(rx == 0xff) return;
    readRX(rx);
	received_bytes[received_bytes_index++] = rx;
	uart_isr_count++;

}

	


