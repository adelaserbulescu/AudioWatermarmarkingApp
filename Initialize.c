#include "Talkthrough.h"
#include <string.h>

/*****************************************************************************
 Function:	Init_Flags													
																		
 Description:	Configure PORTF flags to control ADC and DAC RESETs
 													
******************************************************************************/
void Init_Flags(void)
{
	int temp;
	// configure programmable flags
	// set PORTF function enable register (need workaround)
	*pPORT_MUX &= ~PFTE;
	temp = *pPORTF_FER;
	temp++;
    *pPORTF_FER = 0x0000;
    *pPORTF_FER = 0x0000;
    *pPORTF_FER &= ~(1<<5); //debug led
    *pPORTF_FER |= (1 << 2) | (1 << 3); // PF2 & PF3 for UART1 RX/TX

    // set PORTF direction register
    *pPORTFIO_DIR = 0x1FC0;
    *pPORTFIO_DIR |= (1<<5); //debug led
        
   	// set PORTF input enable register
    *pPORTFIO_INEN = 0x005C;
         
	// set PORTF clear register
    *pPORTFIO_CLEAR = 0x0FC0;
}


/*****************************************************************************
 Function:	Audio_Reset
					
 Description:	This function Resets the ADC and DAC. 
		
******************************************************************************/   
void Audio_Reset(void)
{
	int i;
	// give some time for reset to take affect
    for(i = 0; i< delay;i++){};
 	
    // set port f set register
    *pPORTFIO_SET = PF12;

}

//--------------------------------------------------------------------------//
// Function:	Init_Sport0													//
//																			//
// Description:	Configure Sport0 for I2S mode, to transmit/receive data 	//
//				to/from the ADC/DAC.Configure Sport for al clocks and //
//				frame syncs.												//
//--------------------------------------------------------------------------//
void Init_Sport0(void)
{
	// Sport0 receive configuration
	// al CLK, al Frame sync, MSB first, Active Low
	// 24-bit data, Secondary side enable, Stereo frame sync enable
// Users of ADSP-BF537 EZ-KIT Board Rev 1.0 must enable the internal clock and frame sync	
//	*pSPORT0_RCR1 = RFSR | LRFS | RCKFE | IRFS | IRCLK;
	*pSPORT0_RCR1 = RFSR | RCKFE;
	*pSPORT0_RCR2 = SLEN_24 | RSFSE;
//	*pSPORT0_RCLKDIV = 0x0013;
//	*pSPORT0_RFSDIV = 0x001F;
	
	// Sport0 transmit configuration
	// al CLK, al Frame sync, MSB first, Active Low
	// 24-bit data, Secondary side enable, Stereo frame sync enable
// Users of ADSP-BF537 EZ-KIT Board Rev 1.0 must enable the internal clock and frame sync
//	*pSPORT0_TCR1 = TFSR | LTFS | TCKFE | ITFS | ITCLK;
	*pSPORT0_TCR1 = TFSR | TCKFE;
	*pSPORT0_TCR2 = SLEN_24 | TSFSE;

//	*pSPORT0_TCLKDIV = 0x0013;
//	*pSPORT0_TFSDIV = 0x001F;
	
}

//--------------------------------------------------------------------------//
// Function:	Init_DMA													//
//																			//
// Description:	Initialize DMA3 in autobuffer mode to receive and DMA4 in	//
//				autobuffer mode to transmit									//
//--------------------------------------------------------------------------//
void Init_DMA(void)
{
	// Configure DMA3
	// 32-bit transfers, Interrupt on completion, Autobuffer mode
	*pDMA3_CONFIG = WNR | WDSIZE_32 | DI_EN | FLOW_1;
	// Start address of data buffer
	*pDMA3_START_ADDR = iRxBuffer1;
	// DMA loop count
	*pDMA3_X_COUNT = 2;
	// DMA loop address increment
	*pDMA3_X_MODIFY = 4;
	

	// Configure DMA4
	// 32-bit transfers, Autobuffer mode
	*pDMA4_CONFIG = WDSIZE_32 | FLOW_1;
	// Start address of data buffer
	*pDMA4_START_ADDR = iTxBuffer1;
	// DMA loop count
	*pDMA4_X_COUNT = 2;
	// DMA loop address increment
	*pDMA4_X_MODIFY = 4;

}

//--------------------------------------------------------------------------//
// Function:	Enable_DMA_Sport											//
//																			//
// Description:	Enable DMA3, DMA4, Sport0 TX and Sport0 RX					//
//--------------------------------------------------------------------------//
void Enable_DMA_Sport0(void)
{
	// enable DMAs
	*pDMA4_CONFIG	= (*pDMA4_CONFIG | DMAEN);
	*pDMA3_CONFIG	= (*pDMA3_CONFIG | DMAEN);
	
	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 | TSPEN);
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 | RSPEN);
}

//--------------------------------------------------------------------------//
// Function:	Init_Interrupts												//
//																			//
// Description:	Initialize Interrupt for Sport0 RX							//
//--------------------------------------------------------------------------//
void Init_Interrupts(void)
{
	// Set Sport0 RX (DMA3) interrupt priority to 2 = IVG9 
	*pSIC_IAR0 = 0xff2fffff;
	*pSIC_IAR1 = 0xff3ffff; // Set UART1 RX interrupt priority to 3
	*pSIC_IAR2 = 0xffffffff;
	*pSIC_IAR3 = 0xffffffff;

	// assign ISRs to interrupt vectors
	// Sport0 RX ISR -> IVG 9
	//UART1 ISR -> IVG10
	register_handler(ik_ivg9, Sport0_RX_ISR);
	register_handler(ik_ivg10, UART1_ISR);

	// enable Sport0 RX and UART1 interrupts
	*pSIC_IMASK = 0x00002020;
}

char text[50];
int len;


volatile int size_enc;
void getText(void)
{
	memcpy(text, rx_buffer, sizeof(rx_buffer));
	len = strlen(text);
	size_enc = 0;


}


void procFirstTwoChars(void)
{
	strncpy(proc_string, text, 2);
	encodeMessage();
	fsk(50, 2000);

}


void initUART(void)
{
    // Enable UART1
    *pUART1_GCTL = UCEN;

    // Set baud rate
    *pUART1_LCR = DLAB;
    *pUART1_DLH = 0x02;
    *pUART1_DLL = 0x8B;
    *pUART1_LCR = 0x0003;   // 8 data bits, no parity, 1 stop bit
    *pUART1_LCR &= ~DLAB;

    // Enable RX interrupt ONLY
    *pUART1_IER = ERBFI | ETBEI;


    // DO NOT enable ETBEI here
}


/*void startRpiScript(void)
{
	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0) {
		tx_buffer[0] = 1;
		*pDMA9_CONFIG = DMAEN | FLOW_STOP;
	}
}*/






	

