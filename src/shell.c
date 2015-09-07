/*
 * shell.c
 *
 *  Created on: Sep 6, 2015
 *      Author: ryochack
 */

#include <string.h>
#include "shell.h"
#include "ascii.h"

#define ERR_COMMAND_NOT_FOUND 0x80000000

typedef struct command_t {
	char name[SHELL_COMMAND_LEN];
	shell_command_t func;
} command_t;

typedef struct shell_t {
	char buffer[SHELL_BUFFER_LEN];
	char prompt[SHELL_PROMPT_LEN];
	shell_get_char_t   get_char;
	shell_put_char_t   put_char;
	int num_of_commands;
	command_t command[SHELL_MAX_COMMANDS];
} shell_t;

typedef struct shell_opt_t {
	int argc;
	char *argv[SHELL_MAX_ARGS];
} shell_opt_t;

static shell_t shell_instance = {
	.get_char = NULL,
	.put_char = NULL,
	.num_of_commands = 0,
};


static size_t write_line(const char *msg);
static void echoback(char c);
static void backspace(void);
static size_t read_line(char *buffer, size_t len);
static void parse_command(char *buffer, shell_opt_t *opt);
static int execute_command(int argc, const char **argv);
static int builtincommand_help(int argc, const char **argv);


void shell_init(shell_get_char_t func_get, shell_put_char_t func_put) {
	shell_t *sh = &shell_instance;
	memset(sh, 0, sizeof(shell_t));

	if (func_get) {
		sh->get_char = func_get;
	}
	if (func_put) {
		sh->put_char = func_put;
	}
	shell_add_command("help", builtincommand_help);
}

bool shell_add_command(const char* command, shell_command_t func_command) {
	shell_t *sh = &shell_instance;
	int i;

	if (!command) {
		return false;
	}
	if (!func_command) {
		return false;
	}
	if (sh->num_of_commands == SHELL_MAX_COMMANDS) {
		return false;
	}
	for (i=0; i<SHELL_MAX_COMMANDS; i++) {
		command_t *cmd = &sh->command[i];
		if (!cmd->func) {
			strncpy(cmd->name, command, SHELL_COMMAND_LEN-1);
			cmd->func = func_command;
			sh->num_of_commands++;
			return true;
		}
	}
	return false;
}

void shell_set_prompt(const char* prompt) {
	shell_t *sh = &shell_instance;
	strncpy(sh->prompt, prompt, SHELL_PROMPT_LEN-1);
}

static size_t write_line(const char *msg) {
	shell_t *sh = &shell_instance;
	const char *p = msg;
	if (sh->put_char) {
		while(*p) {
			sh->put_char(*(p++));
		}
	}
	return 0;
}

static void echoback(char c) {
	shell_t *sh = &shell_instance;
	if (sh->put_char) {
		sh->put_char(c);
	}
}

static void backspace(void) {
	echoback(ASCII_BS);
	echoback(' ');
	echoback(ASCII_BS);
}

static size_t read_line(char *buffer, size_t len) {
	shell_t *sh = &shell_instance;
	char *p = buffer;
	char * const head = buffer;
	char * const tail = buffer + len - 1;

	if (!sh->get_char) {
		return 0;
	}

	*tail = '\0';

	while (p < tail) {
		char c = sh->get_char();
		if (ascii_is_space(c)) {
			if ((p > head) && (!ascii_is_space(*(p-1)))) {
				/* no echoback if space is head or continuous space */
				echoback(' ');
				*(p++) = c;
			}
		}
		else if (ascii_is_graphic_char(c)) {
			echoback(c);
			*(p++) = c;
		}
		else if (c == ASCII_BS) {
			if (p > head) {
				backspace();
				*p = '\0';
				p--;
			}
		}
		else if (ascii_is_eol(c)) {
			echoback('\n');
			*p = '\0';
			p++;
			break;
		}
		else {
			;
		}
	}

	return (size_t)(p - head);
}

static void parse_command(char *buffer, shell_opt_t *opt) {
	char *p = buffer;
	bool is_head = true;
	opt->argc = 0;

	while (*p) {
		if (*p == ' ') {
			*p = '\0';
			is_head = true;
		}
		else if (is_head) {
			opt->argv[opt->argc] = p;
			opt->argc++;
			is_head = false;
		}
		else {
			;
		}
		p++;
	}
}

static int execute_command(int argc, const char **argv) {
	shell_t *sh = &shell_instance;
	int i;

	for (i=0; i<SHELL_MAX_COMMANDS; i++) {
		command_t *cmd = &sh->command[i];
		if (!strcmp(argv[0], cmd->name)) {
			if (cmd->func) {
				return cmd->func(argc, argv);
			}
		}
	}
	return ERR_COMMAND_NOT_FOUND;
}

void shell_start(void) {
	shell_t *sh = &shell_instance;
	shell_opt_t opt = { .argc = 0 };
	size_t rlen;
	int ercd;

	while (1) {
		write_line(sh->prompt);

		rlen = read_line(sh->buffer, SHELL_BUFFER_LEN);
		if (rlen == 0) {
			continue;
		}
		parse_command(sh->buffer, &opt);
		if (opt.argc == 0) {
			continue;
		}

		ercd = execute_command(opt.argc, (const char**)opt.argv);
		switch (ercd) {
			case 0:
				write_line("-> ok\n");
				break;
			case ERR_COMMAND_NOT_FOUND:
				write_line("-> command is not found\n");
				break;
			default:
				{
					char ngmsg[24] = "-> ng: ";
					size_t prefixlen = strlen(ngmsg);
					ascii_uint32_to_hexstr(ercd, ngmsg+prefixlen, 24-prefixlen);
					write_line(ngmsg);
					sh->put_char('\n');
				}
				break;
		}
	}
}


static int builtincommand_help(int argc, const char **argv) {
	shell_t *sh = &shell_instance;
	int i;

	for (i=0; i<sh->num_of_commands; i++) {
		command_t *cmd = &sh->command[i];
		write_line(cmd->name);
		sh->put_char('\n');
	}

	return 0;
}

