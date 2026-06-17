#include "uart_loopback_test.h"


void fillTX(uint8_t n)
{
    uint8_t bytes[] = {0xAA, 0xBB, 0xCC, 0xDD, n};
    for (int i = 0; i < sizeof(bytes); i++) {
        while (!(*pUART1_LSR & THRE));
        *pUART1_THR = bytes[i];
    }
}

volatile uint8_t frame_state;

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
		    	*pPORTFIO_TOGGLE = (1 << (6 + (n ^ 1))) | ~(1 << (6 + n));
		    	*pTIMER_ENABLE = TIMEN0;
		    	break;
	}
}
