/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

#include "uartrx.h"

#include <compiler.h>

#include <board.h>

#include <dip204.h>

#include <power_clocks_lib.h>
#include <gpio.h>
#include <usart.h>
#include <led.h>
#include <delay.h>
#include <adc.h>
#include <spi.h>

#include <print_funcs.h>

#include <avr32/io.h>

#include <string.h> // memcpy

uint8_t rxBuf[UART_RX_BUFFER_SIZE];
volatile uint16_t rxHead;
volatile uint16_t rxTail;
volatile uint16_t lastRxError;

void dbg_usart_int_handler(void);

int uart_init()
{
	rxHead		= 0;
	rxTail		= 0;
	lastRxError = 0;
	memset(rxBuf, 0 , UART_RX_BUFFER_SIZE * sizeof(uint8_t));

	static const gpio_map_t DBG_USART_GPIO_MAP =
	{
		{DBG_USART_RX_PIN, DBG_USART_RX_FUNCTION},
		{DBG_USART_TX_PIN, DBG_USART_TX_FUNCTION}
	};

	// Options for debug USART.
	usart_options_t dbg_usart_options =
	{
		.baudrate = DBG_USART_BAUDRATE,
		.charlength = 8,
		.paritytype = USART_NO_PARITY,
		.stopbits = USART_1_STOPBIT,
		.channelmode = USART_NORMAL_CHMODE
	};

	// Setup GPIO for debug USART.
	gpio_enable_module(DBG_USART_GPIO_MAP,
					 sizeof(DBG_USART_GPIO_MAP) / sizeof(DBG_USART_GPIO_MAP[0]));
	
	// Initialize it in RS232 mode.
	usart_init_rs232(DBG_USART, &dbg_usart_options, FOSC0);

	INTC_register_interrupt(&dbg_usart_int_handler, DBG_USART_IRQ, AVR32_INTC_INT1);

	// Enable the receive-complete interrupt
	DBG_USART->ier = AVR32_USART_IER_RXRDY_MASK;
}

uint16_t uart_get(char* c) {
	uint16_t tmptail = 0;
	uint8_t data	 = 0;

	if (rxHead == rxTail) {
		return UART_NO_DATA; /* no data available */
	}

	AVR32_ENTER_CRITICAL_REGION(); // don't want RX interrupt happening in this critical section:

	/* calculate /store buffer index */
	tmptail = (rxTail + 1) & UART_RX_BUFFER_MASK;
	rxTail = tmptail;

	AVR32_LEAVE_CRITICAL_REGION();

	/* get data from receive buffer */
	data = rxBuf[tmptail];
	*c = data;

	return lastRxError;
}

__attribute__((__interrupt__))
void dbg_usart_int_handler(void) {
	unsigned int csr = DBG_USART->csr & DBG_USART->imr;

	// Is this a RX interrupt?
	if (csr & AVR32_USART_CSR_RXRDY_MASK)
	{
		uint16_t tmphead	 = 0;
		unsigned char data	 = 0;
		unsigned int csr	 = 0; // channel status register
		uint16_t lastRxError = 0;

		/* read UART status register and UART data register */
		csr = DBG_USART->csr;
		data = DBG_USART->rhr & AVR32_USART_RHR_RXCHR_MASK;

		/* check for overflow (OVRE) or framing error (FRAME) */
		/* Note: AVR32_USART_CSR_xxx are stored in usart_400.h */
		if (csr & AVR32_USART_CSR_OVRE_MASK)
			lastRxError = UART_OVERRUN_ERROR;
		if (csr & AVR32_USART_CSR_FRAME_MASK)
			lastRxError |= UART_FRAME_ERROR;
		if (csr & AVR32_USART_CSR_PARE_MASK)
			lastRxError |= UART_PARITY_ERROR;

		// clear the error, if there is one.
		if (csr & (AVR32_USART_CSR_FRAME_MASK | AVR32_USART_CSR_OVRE_MASK | AVR32_USART_CSR_PARE_MASK))
			DBG_USART->cr = AVR32_USART_CR_RSTSTA_MASK;

		/* calculate buffer index */
		tmphead = (rxHead + 1) & UART_RX_BUFFER_MASK;

		if (tmphead == rxTail) {
			/* error: receive buffer overflow */
			lastRxError |= UART_BUFFER_OVERFLOW;
		} else {
			/* store new index */
			rxHead = tmphead;
			/* store received data in buffer */
			rxBuf[tmphead] = data;
		}
		lastRxError = lastRxError;
	}
}
