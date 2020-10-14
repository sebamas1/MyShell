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

bool quit = false;

int printPrompt() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char prompt[PATH_MAX];
		strcpy(prompt, getenv("USER"));
		strcat(prompt, "@");
		register gid_t gid;
		gid = getgid();
		strcat(prompt, getgrgid(gid)->gr_name);
		strcat(prompt, ":~");
		strcat(prompt, cwd);
		strcat(prompt, "$");
		printf("-> %s ", prompt);
		return 0;
	} else {
		perror("getcwd() error");
		return 1;
	}
}
static void parsearComando(char *line) {
	char comando[10];
	char argument[PATH_MAX];
	sscanf(line, "%9s %4095s", comando, argument);
	if (strcmp(comando, "cd") == 0) {
		changeDirectory(argument);
	}
	if (strcmp(comando, "echo") == 0) {
		echo(line);
	}
	if (strcmp(comando, "quit") == 0) {
		quit = true;
	}
	if (strcmp(comando, "clr") == 0) {
		system("clear");
	}
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
