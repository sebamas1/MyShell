
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
	struct Nodo *cabeza = crearLinkedList(comando);
	struct Nodo *actual = cabeza;

	if (!(cabeza->listSize == 2)) {
		borrarLista(cabeza);
		fprintf(stderr, "Para cd: cantidad de argumentos invalida.\n");
		return -1;
	}

	char *path = find(actual, 1)->palabra;
	*(path + strlen(path)) = '\0';
	if (chdir(path) != 0) {
		borrarLista(cabeza);
		perror("No se encuentra el directorio especificado");
		return -1;
	} else {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			char pwd[PATH_MAX + 5];
			strcpy(pwd, "PWD=");
			strcat(pwd, cwd);
			if (putenv(pwd) != 0) {
				borrarLista(cabeza);
				perror("putenv() error");
				return -1;
			}
		} else {
			borrarLista(cabeza);
			perror("getcwd() error");
			return -1;
		}
	}
	borrarLista(cabeza);
	return 0;
}

int echo(char *texto) {
	struct Nodo *cabeza = crearLinkedList(texto);
	struct Nodo *actual = cabeza;
	actual = actual->siguienteNodo; //saltea la primera porque es echo
	while (actual != NULL) {
		if (actual->siguienteNodo == NULL) {
			printf("%s\n", actual->palabra);
			borrarLista(cabeza);
			return 0;
		} else {
			printf("%s ", actual->palabra);
			actual = actual->siguienteNodo;
		}
	}
	borrarLista(cabeza);
	return 0;
}

