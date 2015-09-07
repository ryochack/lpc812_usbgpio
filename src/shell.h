/*
 * shell.h
 *
 *  Created on: Sep 6, 2015
 *      Author: ryochack
 *
 * @note
 * MIT License
 */

#ifndef SHELL_H_
#define SHELL_H_

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SHELL_MAX_COMMANDS 8
#define SHELL_COMMAND_LEN 16
#define SHELL_BUFFER_LEN  64
#define SHELL_PROMPT_LEN   4
#define SHELL_MAX_ARGS     8

typedef int (*shell_command_t)(int argc, const char **argv);
typedef char (*shell_get_char_t)(void);
typedef void (*shell_put_char_t)(char c);

void shell_init(
		shell_get_char_t   func_get,
		shell_put_char_t   func_put
		);
bool shell_add_command(const char* command, shell_command_t func_command);
void shell_set_prompt(const char* prompt);
void shell_start(void);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_H_ */
