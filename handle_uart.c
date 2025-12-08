#include "Talkthrough.h"

volatile uint8_t state;
volatile uint8_t j;
volatile uint8_t data;
char rx_buffer[50];
void coefRX(int8_t temp)
{
	switch(state) {
	    case 0:

	    	if(temp == -86)
	    		state = 1;
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

uint8_t step;
void fillTXAndSend(void)
{
	int i;

	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0 && data)
	{
		step++;
		if(step==10)
		{
			tx_buffer[0]=0xAA;


			for(i=1;i<textByteSize;i++)
			{
				tx_buffer[i] = enc[i-1];
			}

			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
			step=0;
			data=0;
		}
	}
}

/*void sendFinishSignal(void)
{
	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0) {
			tx_buffer[0] = 0x0F;
			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
		}
}*/
