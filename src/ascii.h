/*
 * ascii.h
 *
 *  Created on: Sep 6, 2015
 *      Author: ryochack
 */

#ifndef ASCII_H_
#define ASCII_H_

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ASCII_BS 0x08

bool ascii_is_graphic_char(char c);
bool ascii_is_control_char(char c);
bool ascii_is_space(char c);
bool ascii_is_eol(char c);
bool ascii_uint32_to_hexstr(uint32_t v, char *buf, size_t len);
char ascii_num_to_char(uint32_t n);

#ifdef __cplusplus
}
#endif

#endif /* ASCII_H_ */
