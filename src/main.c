/*
===============================================================================
 Name        : usbgpio.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include <cr_section_macros.h>
#include "initializer.h"
#include "uart.h"
#include "shell.h"
#include "gpio.h"

#define FIRMWARE_VERESION "0.01"

static int command_version(int argc, const char** argv) {
	uart_put_line(FIRMWARE_VERESION);
	uart_put_char('\n');
	return 0;
}

int main(void) {
#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#endif

	SwitchMatrix_Init();
	IOCON_Init();
	uart_init();
	uart_setup();
	shell_init(uart_get_char, uart_put_char);
	gpio_init();
	if (!shell_add_command("version", command_version)) {
		uart_put_line("shell_add_command(version) error\n");
	}

	shell_set_prompt("\n$>");
	shell_start(); /* no return */

    return 0 ;
}
