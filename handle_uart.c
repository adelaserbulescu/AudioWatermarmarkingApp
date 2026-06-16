#include "Talkthrough.h"

volatile uint8_t state;
volatile uint8_t j;
volatile uint8_t data;
uint8_t rx_buffer[50];
void coefRX(uint8_t temp)
{
	switch(state) {
	    case 0:
	    	if(temp == 0xAA) {
	    		state = 1;
	    		*pPORTFIO_TOGGLE = (1<<5);
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
	    		getText();
	    	}
	        j = (j + 1) % sizeof(rx_buffer);
	        break;

	}
}

uint8_t tx_buffer[401];
volatile uint8_t tx_index;
volatile int tx_len;
void fillTXAndSend(void)
{
	uint32_t i = 0;

	tx_buffer[i++] = 0xAA;
	tx_buffer[i++] = 0xBB;
	tx_buffer[i++] = 0xCC;
	tx_buffer[i++] = 0xDD;

	for(uint32_t k = 0; k < size_enc && i < sizeof(tx_buffer) - 1; k++)
	    tx_buffer[i++] = enc[k];

	tx_buffer[i++] = '\n';                 // end-of-message

	tx_len = i;
	tx_index = 0;
	data = 0;

	    // Enable UART1 TX interrupt
	*pUART1_IER |= ETBEI;


}

/*void sendFinishSignal(void)
{
	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0) {
			tx_buffer[0] = 0x0F;
			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
		}
}*/
