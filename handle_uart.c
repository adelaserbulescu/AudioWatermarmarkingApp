#include "Talkthrough.h"

volatile uint8_t state;
volatile uint8_t j;
volatile uint8_t data;
uint8_t rx_buffer[50];
void coefRX(uint8_t temp)
{
	switch(state) {
	    case 0:

	    	if(temp) {
	    		state = 1;
	    		*pPORTFIO_SET = (1<<5);
	    	    }
	    	break;
	    case 1:
	    	rx_buffer[j]=temp;
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

	tx_buffer[i++] = 0xAA;                 // SYNC byte

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
