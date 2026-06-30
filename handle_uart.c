#include "Talkthrough.h"
#include <math.h>
#include <string.h>


volatile int fill_tx_called = 0;
void fillTX()
{
	if((*pDMA11_IRQ_STATUS & DMA_RUN) == 0) {
		fill_tx_called = (fill_tx_called + 1) % 512;
		for(int i = 0; i < 4; i++) {
			tx_buffer[i].sof_value = tx_buffer[i].original_right = tx_buffer[i].original_left = tx_buffer[i].watermark = 0xAA + i + (i << 4);

		}

		        // Fill samples directly in tx_buffer (no local array!)
		for(int i = 0; i < 512; i++) {
			tx_buffer[i + 4].original_left = procInLeft[i];
		    tx_buffer[i + 4].original_right = procInRight[i];
		    tx_buffer[i + 4].watermark = (int)floor((double)(procOutLeft[i] - (1 - alpha) * procInLeft[i]));
		    tx_buffer[i + 4].sof_value = 0;
		}

	    // Send via DMA11
	    *pDMA11_START_ADDR = (void*)tx_buffer;
	    ssync();
	    *pDMA11_X_COUNT = 516 * sizeof(struct SignalComparator);  // Total bytes to send
	    ssync();
	    *pDMA11_X_MODIFY = 1;
	    ssync();
	    *pDMA11_CONFIG = DMAEN | FLOW_STOP | WDSIZE_8 | DI_EN;
	    ssync();
	}

}


volatile int rx_index = 0;
volatile int frame_state;

void readRX(char n)
{
	switch(frame_state) {
		    case 0:
		    	if(n == -86) {
		    		frame_state = 1;
		    	}
		    		break;
		    case 1:
		    	if(n == -69) {
		    		frame_state = 2;
		    	}
		    	else frame_state = 0;

	    		break;
		    case 2:
		    	if(n == -52) {
		    		frame_state = 3;
		    	}
		    	else frame_state = 0;

	    	    break;
		    case 3:
		    	if(n == -35) {
		    		frame_state = 4;
		    	}
		    	else frame_state = 0;

	    		break;
		    case 4:
		    	rx_buffer[rx_index++] = n;
		    	//frame_state = 0;
		    	break;
	}
}
