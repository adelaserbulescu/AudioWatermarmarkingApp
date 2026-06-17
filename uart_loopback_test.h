/*****************************************************************************
 * uart_loopback_test.h
 *****************************************************************************/

#ifndef __UART_LOOPBACK_TEST_H__
#define __UART_LOOPBACK_TEST_H__

#include <sys\exception.h>
#include <cdefBF537.h>

/* Add your custom header content here */

extern volatile uint8_t state;
//extern uint8_t rx_buffer[5];
extern uint8_t tx_buffer[5];
extern volatile uint8_t frame_state;

EX_INTERRUPT_HANDLER(UART1_ISR);
EX_INTERRUPT_HANDLER(TIM0_ISR);

void init(void);
void initUART(void);
void initTIM0(void);
void initInterrupts(void);
void fillTX(uint8_t n);
void readRX(uint8_t n);

#endif /* __UART_LOOPBACK_TEST_H__ */
