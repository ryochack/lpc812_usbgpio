/*
 * uart.h
 *
 *  Created on: Aug 24, 2015
 *      Author: ryochack
 *
 * @note
 * MIT License
 */

#ifndef UART_H_
#define UART_H_

#define UART_TMOUT_FOEVER 0

void uart_init(void);
void uart_setup(void);
/* sync */
char uart_get_char(void);
void uart_put_char(char c);
size_t uart_get_line(char *buf, size_t len);
size_t uart_put_line(const char *msg);
/* async */
void uart_get_line_async(char *buf, size_t len);
void uart_put_line_async(const char *msg);
size_t uart_wait_callback(uint32_t tmout);

#endif /* UART_H_ */
