/*****************************************************************************
 * uart_loopback_test.c
 *****************************************************************************/

//#include <sys/platform.h>
//#include "adi_initialize.h"
#include "uart_loopback_test.h"
#include <ccblkfn.h>



void main(void)
{
	void init();
	void initUART();
	void initTIM0();
	void initInterrupts();
	sti(EVT_IVG10 | EVT_IVG12);
	fillTX(1);
	
	while(1);
}

