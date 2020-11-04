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
#include "../ProgramParsing/parseIO.h"

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
	struct Nodo *lista = crearLinkedList(texto);
	if (generarComandoIOParseado(lista) == -1) {
		fprintf(stderr, "Error en el comando echo.\n");
		return -1;
	}
	if (isatty(STDIN_FILENO)) {
		lista = lista->siguienteNodo; //saltea la primera porque es echo
		while (lista != NULL) {
			if (lista->siguienteNodo == NULL) {
				printf("%s\n", lista->palabra);
				restaurarSTDIO();
				borrarLista(lista);
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
	restaurarSTDIO(); //cuando se termina de usar generarComandoParseado() es necesario restaurar los STDIO
	borrarLista(lista);
	return 0;
}

