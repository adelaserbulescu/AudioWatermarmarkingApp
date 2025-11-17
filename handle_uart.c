#include "Talkthrough.h"

volatile uint8_t state;
uint8_t j;
uint8_t data;
char rx_buffer[2];
void coefRX(int8_t temp)
{
	switch(state) {
	    case 0:
	    	if(temp == -86)
	    		state = 1;
	    	break;
	    case 1:
	    	rx_buffer[j++]=temp;
	    	if(j==2) {
	    		data=1;
	    		state=0;
	    		j=0;
	    	}
	    	break;
	}
}

uint8_t step;
void fillTXAndSend(void)
{
	int i;

	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0)
	{
		step++;
		if(step==10)
		{
			tx_buffer[0]=0xAA;


			for(i=1;i<17;i++)
			{
				tx_buffer[i] = enc[i-1];
			}

			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
			step=0;
		}
	}
}

void sendFinishSignal(void)
{
	if((*pDMA9_IRQ_STATUS & DMA_RUN) == 0) {
			tx_buffer[0] = 0x0F;
			*pDMA9_CONFIG = DMAEN | FLOW_STOP;
		}
}
