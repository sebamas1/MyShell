/*
 * myshell.c
 *
 *  Created on: 13 oct. 2020
 *      Author: sebastian
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <grp.h>
#include "InCommands/internalCommands.h"
#include "ProgramInvocation/programInvocation.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
bool quit = false;

static int printPrompt() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char prompt[PATH_MAX];
		strcpy(prompt, getenv("USER"));
		strcat(prompt, "@");
		register gid_t gid;
		gid = getgid();
		strcat(prompt, getgrgid(gid)->gr_name);
		printf(ANSI_COLOR_YELLOW "%s", prompt);
		memset(prompt, 0, sizeof(prompt));
		printf(ANSI_COLOR_RESET "%s", ":");
		strcat(prompt, cwd);
		strcat(prompt, "$");
		printf(ANSI_COLOR_CYAN "%s " ANSI_COLOR_RESET, prompt);
		return 0;
	} else {
		perror("getcwd() error");
		return 1;
	}
}
static void parsearComando(char *line) {
	char comando[10];
	sscanf(line, "%9s", comando);
	if (strcmp(comando, "cd") == 0) {
		changeDir(line);
	} else
	if (strcmp(comando, "echo") == 0) {
		echo(line);
	} else
	if (strcmp(comando, "quit") == 0) {
		quit = true;
	} else
	if (strcmp(comando, "clr") == 0) {
		system("clear");
	} //else programInvocation();
}
int main() {
	while (!quit) {
		printPrompt();

		char *line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		parsearComando(line);
		free(line);
	}
	return 0;
}
