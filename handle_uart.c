#include "Talkthrough.h"

void fillTX()
{
	while(!(*pUART1_LSR & THRE));
    if((*pDMA11_IRQ_STATUS & DMA_RUN) == 0) {
        tx_buffer[0] = 0xAA;
        tx_buffer[1] = 0xBB;
        tx_buffer[2] = 0xCC;
        tx_buffer[3] = 0xDD;
        tx_buffer[4] = (uint8_t) 'd';
        tx_buffer[5] = (uint8_t) 's';
        tx_buffer[6] = (uint8_t) 'p';
        tx_buffer[7] = (uint8_t) 'l';
        tx_buffer[8] = (uint8_t) 'a';
        tx_buffer[9] = (uint8_t) 'b';
        tx_buffer[10] = (uint8_t) 's';

        // Reload descriptor registers before re-enabling
        *pDMA11_START_ADDR = (void*)tx_buffer;
        *pDMA11_X_COUNT    = 12;
        *pDMA11_X_MODIFY   = 1;
        ssync();
        *pDMA11_CONFIG = DMAEN | FLOW_STOP | WDSIZE_8 | DI_EN;
        ssync();
    }
}

volatile uint8_t frame_state;
volatile char rx_buffer[8];
volatile uint8_t rx_index;

void readRX(uint8_t n)
{
	switch(frame_state) {
		    case 0:
		    	if(n == 0xAA) {
		    		frame_state = 1;
		    	}
		    	break;
		    case 1:
		    	if(n == 0xBB) {
		    		frame_state = 2;
		    	}
		    	break;
		    case 2:
		    	if(n == 0xCC) {
		    		frame_state = 3;
		    	}
		    	break;
		    case 3:
		    	if(n == 0xDD) {
		    		frame_state = 4;
		    	}
		    	break;
		    case 4:
		    	frame_state = 0;
		    	rx_buffer[rx_index] = n;
		    	rx_index++;
		    	break;
	}
}
