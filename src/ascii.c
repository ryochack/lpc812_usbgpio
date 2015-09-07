/*
 * shell.c
 *
 *  Created on: Sep 6, 2015
 *      Author: ryochack
 *
 * @note
 * MIT License
 */

#include "ascii.h"

static char lut[0x10] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};


bool ascii_is_graphic_char(char c) {
	if ((c >= 0x20) && (c <= 0x7e)) {
		return true;
	}
	return false;
}

bool ascii_is_control_char(char c) {
	return !ascii_is_graphic_char(c);
}

bool ascii_is_space(char c) {
	switch (c) {
		case 0x20: /* space */
		case 0x09: /* horizontal tab */
			return true;
		default:
			return false;
	}
}

bool ascii_is_eol(char c) {
	switch (c) {
		case '\r':
		case '\n':
			return true;
		default:
			return false;
	}
}

static char convert_4bit_to_char(uint8_t b) {
	if (b > 0x0F) {
		return '*';
	}
	return lut[b];
}

bool ascii_uint32_to_hexstr(uint32_t v, char *buf, size_t len) {
	char *p = buf;
	if (!buf) {
		return false;
	}
	if (len < 11) {
		/* 0x3456789A + '\0' */
		return false;
	}

	*(p++) = '0';
	*(p++) = 'x';
	*(p++) = convert_4bit_to_char((v & 0xF0000000) >> 28);
	*(p++) = convert_4bit_to_char((v & 0x0F000000) >> 24);
	*(p++) = convert_4bit_to_char((v & 0x00F00000) >> 20);
	*(p++) = convert_4bit_to_char((v & 0x000F0000) >> 16);
	*(p++) = convert_4bit_to_char((v & 0x0000F000) >> 12);
	*(p++) = convert_4bit_to_char((v & 0x00000F00) >> 8);
	*(p++) = convert_4bit_to_char((v & 0x000000F0) >> 4);
	*(p++) = convert_4bit_to_char((v & 0x0000000F) >> 0);
	*p = '\0';

	return true;
}

char ascii_num_to_char(uint32_t n) {
	if (n > 9) {
		return '*';
	}
	return '0' + n;
}

