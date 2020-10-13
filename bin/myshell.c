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

int printCurrDir() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("-> %s : ", cwd);
		return 0;
	} else {
		perror("getcwd() error");
		return 1;
	}
}
int main() {
	while (!quit) {
		printCurrDir();

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
