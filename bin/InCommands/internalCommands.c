/*
 * internalCommands.c
 *
 *  Created on: 14 oct. 2020
 *      Author: sebastian
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int changeDirectory(char *path) {
	if (chdir(path) != 0) {
		perror("No se encuentra el directorio especificado");
		return -1;
	} else {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			char pwd[PATH_MAX + 5];
			strcpy(pwd, "PWD=");
			strcat(pwd, cwd);
			if (putenv(pwd) != 0) {
				perror("putenv() error");
				return -1;
			}
		} else {
			perror("getcwd() error");
			return -1;
		}
	}
	return 0;
}

int echo(char *texto) {
	printf("%s", strchr(texto, ' ') + 1);
	return 0;
}

