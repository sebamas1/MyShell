/*
 * programInvocation.c
 *
 *  Created on: 26 oct. 2020
 *      Author: sebastian
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include "../util/LinkedList.h"

static int spawn(char *program, char **arg_list) {
	pid_t child_pid;

	/* Duplicate this process. */
	child_pid = fork();

	if (child_pid != 0) {
		return child_pid;
	} else {
		execvp(program, arg_list);

		/* returns only if an error occurs. */
		perror("");
		abort();
	}
}
/*
 * Notar que el programa no va a aceptar cosas como <file > o < file>. Las llaves tienen que ponerse
 * de manera simetrica, como <file> o < file >
 */
static void parseIOredirection(struct Nodo *nodo) {
	bool aperturaLlave = false;
	bool cierreLlave = false;
	int listSize = getSize(nodo);
	if (strncmp(nodo->palabra, "<", 1) == 0) {
		aperturaLlave = true;
		if (strlen(nodo->palabra) == 1) {
			if (listSize <= (nodo->index + 2)) {
				fprintf(stderr, "No se reconoce el comando de redireccion.\n");
			} else {
				struct Nodo *nodoLlaveCerrada = find(nodo, nodo->index + 2);
				if (strncmp(nodoLlaveCerrada->palabra, ">", 1) == 0) {
					cierreLlave = true;
				}
			}
		} else if (*(nodo->palabra + strlen(nodo->palabra) - 1) == '>') {
			cierreLlave = true;
		}
	}


	if(cierreLlave ^ aperturaLlave){
		fprintf(stderr, "No se reconoce el comando de redireccion.\n");
	}
}

int programInvocation(char *comando) {
	struct Nodo *lista = crearLinkedList(comando);
	int child_status;
	int listSize = lista->listSize;
	char *arg_list[listSize + 1];

	for (int i = 0; i < listSize; i++) { //recorro la lista, y armo el arg_list con cada nodo
		arg_list[i] = lista->palabra;
		parseIOredirection(lista); //aprovechando que ya estoy recorriendo la lista, parseo el IOredirection
		if (i < listSize - 1) {
			lista = lista->siguienteNodo;
		}
	}
	arg_list[listSize] = NULL;
	//spawn(arg_list[0], arg_list);

	/* Wait for the child process to complete. */
	wait(&child_status);

	borrarLista(lista);

	return 0;
}
