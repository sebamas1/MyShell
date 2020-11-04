/*
 * programInvocation.c
 *
 *  Created on: 26 oct. 2020
 *      Author: sebastian
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../util/LinkedList.h"
#include "parseIO.h"

static int spawn(char *program, char **arg_list) {
	pid_t child_pid;

	/* Duplicate this process. */
	child_pid = fork();

	if (child_pid != 0) {
		return child_pid;
	} else {
		execvp(program, arg_list);

		/* returns only if an error occurs. */
		perror("Child process");
		abort();
	}
}

int programInvocation(char *comando) {
	struct Nodo *lista = crearLinkedList(comando);
	int listSize = generarComandoIOParseado(lista);
	if(listSize == -1){
		return -1;
	}
	char *arg_list[listSize + 1];
	lista = find(lista, 0); //vuelvo al principio de la lista(si es que no estoy)
	for (int i = 0; i < listSize; i++) {
		if (lista != NULL) {
			arg_list[i] = lista->palabra;
		}

		if (i < listSize - 1) { //podria perder la ref. y despues no puedo limpiar la lista
			lista = lista->siguienteNodo;
		}
	}

	arg_list[listSize] = NULL;

	int child_status;

	spawn(arg_list[0], arg_list);

	/* Wait for the child process to complete. */
	wait(&child_status);

	restaurarSTDIO();

	borrarLista(lista);

	return 0;
}
