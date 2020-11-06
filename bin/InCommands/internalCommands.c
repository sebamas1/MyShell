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
#include <unistd.h>
#include <sys/stat.h>
#include "../util/LinkedList.h"

int changeDir(struct Nodo *lista) {
	if (!(lista->listSize == 2)) {
		fprintf(stderr, "Para cd: cantidad de argumentos invalida.\n");
		return -1;
	}

	char *path = find(lista, 1)->palabra;
	*(path + strlen(path)) = '\0';
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

int echo(struct Nodo *lista) {
	if (isatty(STDIN_FILENO)) {
		lista = lista->siguienteNodo; //saltea la primera porque es echo
		while (lista != NULL) {
			if (lista->siguienteNodo == NULL) {
				printf("%s\n", lista->palabra);
				return 0;
			} else {
				printf("%s ", lista->palabra);
				lista = lista->siguienteNodo;
			}
		}
	} else {
		struct stat st;
		fstat(0, &st);
		long int file_size = st.st_size;
		char buff[file_size];
		read(0, buff, sizeof(buff));
		write(1, buff, sizeof(buff));
	}
	return 0;
}

