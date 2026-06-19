#include "Talkthrough.h"


void coefRX(uint8_t temp)
{
	switch(state) {
	    case 0:
	    	if(temp == 0xAA) {
	    		state = 1;
	    		*pPORTFIO_TOGGLE = (1<<6);
	    	    }
	    	break;
	    case 1:
	    	if(temp == 0xBB) {
	    		state = 2;
	    	}
	    	break;
	    case 2:
	    	if(temp == 0xCC) {
	    		state = 3;
	    	}
	    	break;
	    case 3:
	    	if(temp == 0xDD) {
	    		state = 4;
	    	}
	    	break;
	    case 4:
	    	rx_buffer[j] = temp;
	    	if(rx_buffer[j] == '\n') {
	    		data=1;
	    		state=0;
	    		j=0;
	    		//getText();
	    	}
	        j = (j + 1) % sizeof(rx_buffer);
	        break;

	}
}


void fillTXAndSend(void)
{

	while(!(*pUART1_LSR & THRE));
	uint32_t i = 0;

	tx_buffer[i++] = 0xAA;
	tx_buffer[i++] = 0xBB;
	tx_buffer[i++] = 0xCC;
	tx_buffer[i++] = 0xDD;
	tx_buffer[i++] = (uint8_t) 'd';
	tx_buffer[i++] = (uint8_t) 's';
	tx_buffer[i++] = (uint8_t) 'p';
	tx_buffer[i++] = (uint8_t) 'l';
	tx_buffer[i++] = (uint8_t) 'a';
	tx_buffer[i++] = (uint8_t) 'b';
	tx_buffer[i++] = (uint8_t) 's';
	tx_buffer[i++] = (uint8_t) '\n';                 // end-of-message

	tx_len = i;
	tx_index = 0;
	data = 0;

	*pDMA11_START_ADDR = (void*)tx_buffer;
	*pDMA11_X_COUNT    = 5;
	*pDMA11_X_MODIFY   = 1;
	ssync();
	*pDMA11_CONFIG = DMAEN | FLOW_STOP | WDSIZE_8 | DI_EN;
	ssync();



}

/*void sendFinishSignal(void)
{
	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0) {
			tx_buffer[0] = 0x0F;
			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
		}
}*/
