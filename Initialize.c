#include "uart_loopback_test.h"

#define SCLK_HZ 100000000UL
#define PERIOD0 2
#define TIMER_PERIOD0 (SCLK_HZ * PERIOD0)
#define TIMER_WIDTH0 (TIMER_PERIOD0 / 2)



void init()
{
	*pPORT_MUX &= ~PFTE;
	*pPORTF_FER &= ~((1<<6) | (1<<7));
	*pPORTF_FER |= (1 << 2) | (1 << 3);
	*pPORTFIO_DIR |= (1<<6) | (1<<7);
	*pPORTFIO_INEN = 0x0000;
}


uint8_t tx_buffer[5];
void initUART(void)
{
    // Enable UART1
    *pUART1_GCTL = UCEN;

    // Set baud rate
    *pUART1_LCR = DLAB;
    *pUART1_DLH = 0x02;
    *pUART1_DLL = 0x8B;
    //ssync();
    *pUART1_LCR = 0x0003;   // 8 data bits, no parity, 1 stop bit
    //ssync();
    *pUART1_LCR &= ~DLAB;
    *pUART1_MCR = LOOP_ENA;
    //ssync();
    *pUART1_IER = ERBFI | ETBEI;
    //ssync();

    *pDMA9_START_ADDR = (void*)tx_buffer;
    *pDMA9_X_COUNT = 5;
    *pDMA9_X_MODIFY = 1;
    *pDMA9_CONFIG = 0;

}

void initTIM0()
{
	*pTIMER_DISABLE = TIMDIS0;
	*pTIMER0_CONFIG = PERIOD_CNT | PWM_OUT | IRQ_ENA;
	*pTIMER0_PERIOD = TIMER_PERIOD0;
	*pTIMER0_WIDTH = TIMER_WIDTH0;


}

void initInterrupts()
{
	*pSIC_IAR0 = 0xffffffff;
	*pSIC_IAR1 = 0xff3fffff; //UART1 RX interrupt priority 3
	*pSIC_IAR2 = 0xffff5fff; //TIM0 interrupt priority 5
	*pSIC_IAR3 = 0xffffffff;
	//ssync();

	register_handler(ik_ivg10, UART1_ISR);
	register_handler(ik_ivg12, TIM0_ISR);

	*pIMASK |= EVT_IVG10 | EVT_IVG12;
	//ssync();
	*pSIC_IMASK = 0x00082000; //enable UART1 RX and TIM0 interrupts
	//ssync();
}
