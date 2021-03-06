/*
 * uart.c
 *
 *  Created on: Aug 24, 2015
 *      Author: ryochack
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "pinassign.h"
#include "uart.h"
#include <string.h>

#define UART_HANDLE_SIZE    0x10

typedef struct uart_mgr {
	/* UART handle and memory for ROM API */
	UART_HANDLE_T *handle;
	/* Use a buffer size larger than the expected return value of
	   uart_get_mem_size() for the static UART handle type */
	uint32_t handle_mem[UART_HANDLE_SIZE];
	volatile bool got_callback;
	uint32_t n;
	bool is_setuped;
} uart_mgr_t;
static uart_mgr_t uart_instance = {
	.handle       = NULL,
	.handle_mem   = { 0 },
	.got_callback = false,
	.n            = 0,
	.is_setuped   = false,
};

void uart_init(void) {
	Chip_Clock_SetUARTClockDiv(1);	/* divided by 1 */
	Chip_UART_Init(LPC_USART0);
}

void uart_setup(void) {
	uart_mgr_t *uart = &uart_instance;
	uint32_t frg_mult;

	/* 115.2KBPS, 8N1, ASYNC mode, no errors, clock filled in later */
	UART_CONFIG_T cfg = {
		.sys_clk_in_hz  = 0,        /* U_PCLK frequency in Hz */
		.baudrate_in_hz = 115200,   /* Baud Rate in Hz */
		.config         = 0x01,     /* 8bit length, no parity, 1 stop bit */
		.sync_mod       = 0,        /* Asynchronous Mode */
		.error_en       = NO_ERR_EN /* Enable No Errors */
	};

	/* Perform a sanity check on the storage allocation */
	if (LPC_UARTD_API->uart_get_mem_size() > sizeof(uart->handle_mem)) {
		/* Example only: this should never happen and probably isn't needed for
		   most UART code. */
		return;
	}

	/* Setup the UART handle */
	uart->handle = LPC_UARTD_API->uart_setup((uint32_t)LPC_USART0, (uint8_t *)&uart->handle_mem);
	if (uart->handle == NULL) {
		return;
	}

	/* Need to tell UART ROM API function the current UART peripheral clock
	     speed */
	cfg.sys_clk_in_hz = Chip_Clock_GetSystemClockRate();

	/* Initialize the UART with the configuration parameters */
	frg_mult = LPC_UARTD_API->uart_init(uart->handle, &cfg);
	if (frg_mult) {
		Chip_SYSCTL_SetUSARTFRGDivider(0xFF);	/* value 0xFF should be always used */
		Chip_SYSCTL_SetUSARTFRGMultiplier(frg_mult);
	}

	/* Enable the IRQ for the UART */
	NVIC_EnableIRQ(UART0_IRQn);

	uart->is_setuped = true;
}

static void uart_callback(uint32_t ercd,  uint32_t n) {
	uart_mgr_t *uart = &uart_instance;
	uart->got_callback = true;
	uart->n = n;
}

/* async */
void uart_get_line_async(char *buf, size_t len) {
	uart_mgr_t *uart = &uart_instance;
	UART_PARAM_T param;

	if (!uart->is_setuped) return;

	param.buffer = (uint8_t *)buf;
	param.size = len;

	/* Receive data up to the CR/LF character in polling mode. Will
	   truncate at length if too long.	*/
	param.transfer_mode = RX_MODE_LF_RECVD;
	param.driver_mode = DRIVER_MODE_INTERRUPT;

	/* Setup the receive callback, this will get called when the
	   transfer is complete */
	param.callback_func_pt = (UART_CALLBK_T)uart_callback;

	/* Receive the data */
	uart->got_callback = false;
	LPC_UARTD_API->uart_get_line(uart->handle, &param);
}

void uart_put_line_async(const char *msg) {
	uart_mgr_t *uart = &uart_instance;
	UART_PARAM_T param;

	if (!uart->is_setuped) return;

	param.buffer = (uint8_t *)msg;
	param.size = strlen(msg);

	/* Interrupt mode, do not append CR/LF to sent data */
	param.transfer_mode = TX_MODE_SZERO;
	param.driver_mode = DRIVER_MODE_INTERRUPT;

	/* Setup the transmit callback, this will get called when the
	   transfer is complete */
	param.callback_func_pt = (UART_CALLBK_T)uart_callback;

	/* Transmit the data using interrupt mode, the function will return */
	uart->got_callback = false;
	LPC_UARTD_API->uart_put_line(uart->handle, &param);
}

uint32_t uart_wait_callback(uint32_t tmout) {
	uart_mgr_t *uart = &uart_instance;
	bool is_foever = !tmout;
	if (!uart->is_setuped)
		return 0;

	while (is_foever || (tmout-- > 0)) {
		if (uart->got_callback)
			return uart->n;
		__WFI();
	}
	return 0;
}

/* sync */
char uart_get_char(void) {
	uart_mgr_t *uart = &uart_instance;
	if (!uart->is_setuped) return '\0';
	return LPC_UARTD_API->uart_get_char(uart->handle);
}

void uart_put_char(char c) {
	uart_mgr_t *uart = &uart_instance;
	if (!uart->is_setuped) return;
	LPC_UARTD_API->uart_put_char(uart->handle, c);
}

size_t uart_get_line(char *buf, size_t len) {
	uart_mgr_t *uart = &uart_instance;
	if (!uart->is_setuped) return 0;

	uart_get_line_async(buf, len);
	/* Wait until the transmit callback occurs. When it hits, the transfer is complete. */
	return uart_wait_callback(UART_TMOUT_FOEVER);
}

size_t uart_put_line(const char *msg) {
	uart_mgr_t *uart = &uart_instance;
	if (!uart->is_setuped) return 0;

	uart_put_line_async(msg);
	/* Wait until the transmit callback occurs. When it hits, the
	   transfer is complete. */
	return uart_wait_callback(UART_TMOUT_FOEVER);
}

/**
 * @brief	Handle UART interrupt
 * @return	Nothing
 */
void UART0_IRQHandler(void) {
	uart_mgr_t *uart = &uart_instance;
	LPC_UARTD_API->uart_isr(uart->handle);
}

