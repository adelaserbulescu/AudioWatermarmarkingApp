#include "uart_loopback_test.h"

EX_INTERRUPT_HANDLER(UART1_ISR)
{
	if(*pUART1_LSR & (OE | PE |FE | BI)) {
		volatile uint8_t dummy = *pUART1_RBR;
		frame_state = 0;  // reset state machine
		return;
	}
	if(*pUART1_LSR & DR)
	{
		uint8_t rx = *pUART1_RBR;
	    readRX(rx);
	}
}

volatile uint8_t state;


EX_INTERRUPT_HANDLER(TIM0_ISR)
{
	*pTIMER_STATUS = TIMIL0;
	state ^= 1;
	fillTX(state);

}
