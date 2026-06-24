#include "Talkthrough.h"



void fillTX()
{
	/*int timeout = 0;
	while(!(*pUART1_LSR & TEMT) && timeout < 0xfff) timeout++;*/

    if((*pDMA11_IRQ_STATUS & DMA_RUN) == 0) {
    	int i = 0;
    	for(int idx = 0; i < 3; idx ++){
            tx_buffer[i++] = 0xAA;

            tx_buffer[i++] = 0xBB;

            tx_buffer[i++] = 0xCC;

            tx_buffer[i++] = 0xDD;

            }

        tx_buffer[i++] = 't';

        tx_buffer[i++] = 'i';

        tx_buffer[i++] = 'n';

        tx_buffer[i++] = 'n';

        tx_buffer[i++] = 'i';

        tx_buffer[i++] = 't';

        tx_buffer[i++] = 'u';

        tx_buffer[i++] = 's';



        *pDMA11_START_ADDR = (void*)tx_buffer;
        *pDMA11_X_COUNT = i;  // 19 total bytes
        *pDMA11_X_MODIFY = 1;
        ssync();

        *pDMA11_CONFIG = DMAEN | FLOW_STOP | WDSIZE_8 | DI_EN;
        ssync();

        /*timeout = 0;
        while(!(*pUART1_LSR & TEMT) && timeout < 0xfff) timeout++;*/
        //delayTIM0();




    }

}


volatile int rx_index;
volatile int frame_state;

void readRX(char n)
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
		    	rx_buffer[rx_index++] = n;
		    	return;
	}
}
