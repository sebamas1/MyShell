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

#define PATH_MAX 500
bool quit = false;

int printPrompt() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char prompt[PATH_MAX];
		strcpy(prompt, getenv("USER"));
		strcat(prompt, "@");
		strcat(prompt, getenv("USER"));
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
int main() {
	while (!quit) {
		printPrompt();

		char *line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		printf("Comando: %s", line);
		if(strcmp(line, "quit\n") == 0){
			quit = true;
		}
		free(line);
	}
	return 0;
}
