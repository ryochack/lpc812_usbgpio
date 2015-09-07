/*
 * gpio.c
 *
 *  Created on: Sep 7, 2015
 *      Author: ryochack
 *
 * @note
 * MIT License
 */

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "shell.h"
#include "uart.h"
#include "ascii.h"
#include <stdlib.h>
#include <string.h>

#define IOCON_INVALID 0xFF
#define MAX_PIN_NUMBER 18

typedef struct subcmd {
	const char *name;
	shell_command_t func;
	const char *description;
} subcmd_t;


static uint8_t pin2iocon(uint8_t pin);
static void usage(const subcmd_t *subcmd, int num);
static int subcommand_gpio_list(int argc, const char **argv);
static int subcommand_gpio_mode(int argc, const char **argv);
static int subcommand_gpio_dir(int argc, const char **argv);
static int subcommand_gpio_read(int argc, const char **argv);
static int subcommand_gpio_write(int argc, const char **argv);
static int command_gpio(int argc, const char **argv);


static uint8_t pin2iocon(uint8_t pin) {
	static const uint8_t map[] = {
		IOCON_PIO0,     /* PIN 0 */
		IOCON_PIO1,     /* PIN 1 */
		IOCON_PIO2,     /* PIN 2 */
		IOCON_PIO3,     /* PIN 3 */
		IOCON_PIO4,     /* PIN 4 */
		IOCON_PIO5,     /* PIN 5 */
		IOCON_PIO6,     /* PIN 6 */
		IOCON_PIO7,     /* PIN 7 */
		IOCON_PIO8,     /* PIN 8 */
		IOCON_PIO9,     /* PIN 9 */
		IOCON_PIO10,    /* PIN 10 */
		IOCON_PIO11,    /* PIN 11 */
		IOCON_PIO12,    /* PIN 12 */
		IOCON_PIO13,    /* PIN 13 */
		IOCON_PIO14,    /* PIN 14 */
		IOCON_PIO15,    /* PIN 15 */
		IOCON_PIO16,    /* PIN 16 */
		IOCON_PIO17,    /* PIN 17 */
		IOCON_PIO18,    /* PIN 18 */
		IOCON_PIO19,    /* PIN 19 */
		IOCON_PIO20,    /* PIN 20 */
		IOCON_PIO21,    /* PIN 21 */
		IOCON_PIO22,    /* PIN 22 */
		IOCON_PIO23,    /* PIN 23 */
		IOCON_PIO24,    /* PIN 24 */
		IOCON_PIO25,    /* PIN 25 */
		IOCON_PIO26,    /* PIN 26 */
		IOCON_PIO27,    /* PIN 27 */
		IOCON_PIO28,    /* PIN 28 */
		IOCON_PIO_NUL0, /* PIN NULL */
		IOCON_PIO_NUL1, /* PIN NULL */
	};
	static const uint8_t map_size = sizeof(map) / sizeof(map[0]);
	if (pin > map_size) {
		return IOCON_INVALID;
	}
	return map[pin];
}

void gpio_init(void) {
	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);
	shell_add_command("gpio", command_gpio);
}

void gpio_set_mode(uint8_t pin, CHIP_PIN_MODE_T mode) {
	uint8_t iocon = pin2iocon(pin);
	if (iocon == IOCON_INVALID) {
		return;
	}
	Chip_IOCON_PinSetMode(LPC_IOCON, iocon, mode);
}

void gpio_set_dir(uint8_t pin, bool output) {
	Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, 0, pin, output);
}

bool gpio_get_dir(uint8_t pin) {
	return Chip_GPIO_GetPinDIR(LPC_GPIO_PORT, 0, pin);
}

void gpio_write(uint8_t pin, bool level) {
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 0, pin, level);
}

bool gpio_read(uint8_t pin) {
	return Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, pin);
}


static void usage(const subcmd_t *subcmd, int num) {
	int i;
	uart_put_line("gpio sub-command is following\n");
	for (i=0; i<num; i++) {
		uart_put_line(subcmd[i].name);
		uart_put_line(": ");
		uart_put_line(subcmd[i].description);
		uart_put_char('\n');
	}
}

static int subcommand_gpio_list(int argc, const char **argv) {
	/* > gpio list */
	uint8_t i;
	for (i=0; i<MAX_PIN_NUMBER; i++) {
		uart_put_char(ascii_num_to_char(i/10));
		uart_put_char(ascii_num_to_char(i%10));
		uart_put_line(": dir=");
		uart_put_line(gpio_get_dir(i) ? "out" : "in ");
		uart_put_line(" value=");
		uart_put_char(ascii_num_to_char(gpio_read(i)));
		uart_put_char('\n');
	}

	return 0;
}

static int subcommand_gpio_mode(int argc, const char **argv) {
	/* > gpio mode $pin $mode(0:hi-z 1:pulldown, 2:pullup, 3:repeater) */
	uint8_t pin;
	CHIP_PIN_MODE_T mode;

	if (argc < 3) {
		return -1;
	}

	pin = (uint8_t)atoi(argv[1]);
	if (pin >= MAX_PIN_NUMBER) {
		return -1;
	}
	mode = (CHIP_PIN_MODE_T)atoi(argv[2]);
	if (mode > PIN_MODE_REPEATER) {
		return -1;
	}
	gpio_set_mode(pin, mode);

	uart_put_line("gpio mode set: pin=");
	uart_put_char(ascii_num_to_char(pin/10));
	uart_put_char(ascii_num_to_char(pin%10));
	uart_put_line(" mode=");
	uart_put_char(ascii_num_to_char(mode));
	uart_put_char('\n');

	return 0;
}

static int subcommand_gpio_dir(int argc, const char **argv) {
	/* > gpio dir $pin $dir(0:input, 1:output) */
	uint8_t pin;
	bool dir;

	if (argc < 3) {
		return -1;
	}

	pin = (uint8_t)atoi(argv[1]);
	if (pin >= MAX_PIN_NUMBER) {
		return -1;
	}
	dir = !!atoi(argv[2]);
	gpio_set_dir(pin, dir);

	uart_put_line("gpio dir set: pin=");
	uart_put_char(ascii_num_to_char(pin/10));
	uart_put_char(ascii_num_to_char(pin%10));
	uart_put_line(" dir=");
	uart_put_char(ascii_num_to_char(gpio_get_dir(pin)));
	uart_put_char('\n');

	return 0;
}

static int subcommand_gpio_read(int argc, const char **argv) {
	/* > gpio read $pin */
	uint8_t pin;

	if (argc < 2) {
		return -1;
	}

	pin = (uint8_t)atoi(argv[1]);
	if (pin >= MAX_PIN_NUMBER) {
		return -1;
	}
	uart_put_line("gpio read: pin=");
	uart_put_char(ascii_num_to_char(pin/10));
	uart_put_char(ascii_num_to_char(pin%10));
	uart_put_line(" value=");
	uart_put_char(ascii_num_to_char(gpio_read(pin)));
	uart_put_char('\n');

	return 0;
}

static int subcommand_gpio_write(int argc, const char **argv) {
	/* > gpio write $pin $value(0 or 1) */
	uint8_t pin;
	bool value;

	if (argc < 3) {
		return -1;
	}

	pin = (uint8_t)atoi(argv[1]);
	if (pin >= MAX_PIN_NUMBER) {
		return -1;
	}
	value = !!atoi(argv[2]);
	gpio_write(pin, value);

	uart_put_line("gpio write: pin=");
	uart_put_char(ascii_num_to_char(pin/10));
	uart_put_char(ascii_num_to_char(pin%10));
	uart_put_line(" value=");
	uart_put_char(ascii_num_to_char(gpio_read(pin)));
	uart_put_char('\n');

	return 0;
}

static int command_gpio(int argc, const char **argv) {
	static const subcmd_t subcmd[] = {
		{ "list",  subcommand_gpio_list,  "show gpio port list and it's infromation." },
		{ "mode",  subcommand_gpio_mode,  "set gpio port mode." },
		{ "dir",   subcommand_gpio_dir,   "set gpio port direction." },
		{ "read",  subcommand_gpio_read,  "read gpio input value." },
		{ "write", subcommand_gpio_write, "write gpio output value." },
	};
	int i;
	const int num_of_subcmds = sizeof(subcmd) / sizeof(subcmd[0]);
	const char *ptn;

	if (argc < 2) {
		usage((const subcmd_t*)subcmd, num_of_subcmds);
		return 0;
	}

	ptn = argv[1];
	for (i=0; i<num_of_subcmds; i++) {
		if (!strcmp(ptn, subcmd[i].name)) {
			return subcmd[i].func(argc-1, &argv[1]);
		}
	}

	return -1;
}


