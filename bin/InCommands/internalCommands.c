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
#include "../util/LinkedList.h"

int changeDir(char *comando) {
	struct Nodo *actual = crearLinkedList(comando);
	if (!(getListSize() == 2)) {
		borrarLista(getCabeza());
		fprintf(stderr, "Para cd: cantidad de argumentos invalida.\n");
		return -1;
	}

	char *path = find(actual, 1)->palabra;
	*(path + strlen(path) - 1) = '\0';
	if (chdir(path) != 0) {
		borrarLista(getCabeza());
		perror("No se encuentra el directorio especificado");
		return -1;
	} else {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			char pwd[PATH_MAX + 5];
			strcpy(pwd, "PWD=");
			strcat(pwd, cwd);
			if (putenv(pwd) != 0) {
				borrarLista(getCabeza());
				perror("putenv() error");
				return -1;
			}
		} else {
			borrarLista(getCabeza());
			perror("getcwd() error");
			return -1;
		}
	}
	borrarLista(getCabeza());
	return 0;
}

int echo(char *texto) {
	struct Nodo *actual = crearLinkedList(texto);
	actual = actual->siguienteNodo;
	while (actual != NULL) {
		printf("%s ", actual->palabra);
		actual = actual->siguienteNodo;
		if (actual->siguienteNodo == NULL) {
			printf("%s", actual->palabra);
			return 0;
		}
	}
	return 0;
}

