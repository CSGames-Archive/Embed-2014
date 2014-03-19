/*
 * uart.h
 *
 *  Created on: 2014-03-19
 *      Author: fmorin
 */

#ifndef UART_CSGAMES_H_
#define UART_CSGAMES_H_

#include <stdint.h>

/** Size of the circular receive buffer, must be power of 2 */
#define UART_RX_BUFFER_SIZE 128
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)

#define UART_STATUS_MASK	  0xff00
#define UART_DATA_MASK		  0x00ff

#define UART_PARITY_ERROR     0x1000              /* Parity error by UART        */
#define UART_FRAME_ERROR      0x0800              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0400              /* Overrun condition by UART   */
#define UART_BUFFER_OVERFLOW  0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /* no receive data available   */
#define UART_NO_ERROR		  0x0000

#define EOS (-1)

int uart_init();
	
int uart_put(char data);
	
uint16_t uart_get(char* c);

#endif // UART_CSGAMES_H_
