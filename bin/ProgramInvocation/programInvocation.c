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
#include <fcntl.h>
#include "../util/LinkedList.h"

bool comando_correcto = true;
bool io_redirect = false;
struct Nodo *input;
struct Nodo *output;
/*
 * El programa no va a aceptar cosas como <file > o < file> o <file>. Las llaves tienen
 * que ponerse asi < file >
 */
static bool parseIOredirection(struct Nodo *nodo) {
	bool aperturaLlave = false;
	bool cierreLlave = false;
	int listSize = getSize(nodo);
	if (strncmp(nodo->palabra, "<", 1) == 0) {
		aperturaLlave = true;
		if (strlen(nodo->palabra) == 1) {
			if (!(listSize <= (nodo->index + 2))) {
				struct Nodo *nodoLlaveCerrada = find(nodo, nodo->index + 2);
				if (strncmp(nodoLlaveCerrada->palabra, ">", 1) == 0) {
					cierreLlave = true;
				}
			}
		}
	}

	if (!cierreLlave && aperturaLlave) {
		fprintf(stderr, "No se reconoce el comando de redireccion.\n");
		comando_correcto = false;
		return false;
	} else if (!aperturaLlave && !cierreLlave) {
		return false;
	} else {
		input = nodo->siguienteNodo;
		output = input->siguienteNodo->siguienteNodo;
		io_redirect = true;
		return true;
	}
}
static int redirectIO(struct Nodo *input, struct Nodo *output) {
	int input_file = open(input->palabra, O_RDONLY, 0777);
	if (input_file == -1) {
		io_redirect = false;
		return -1;
	}
	dup2(input_file, 0);
	if (output != NULL) {
		int output_file = open(output->palabra, O_WRONLY | O_CREAT, 0777);
		if (output_file == -1) {
			io_redirect = false;
			return -1;
		}
		dup2(output_file, 1);
	}
	io_redirect = false;
	return 0;
}

static int spawn(char *program, char **arg_list) {
	if (!comando_correcto) {
		return -1;
	}
	if (io_redirect) {
		if (redirectIO(input, output) == -1) {
			perror("No se encuentra el archivo a redireccionar el input");
			return -1;
		}
	}
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

int programInvocation(char *comando) {
	struct Nodo *lista = crearLinkedList(comando);
	int child_status;
	int listSize = lista->listSize;
	char *arg_list[listSize + 1];

	for (int i = 0; i < listSize; i++) { //recorro la lista, y armo el arg_list con cada nodo
		if (parseIOredirection(lista)) {
			arg_list[lista->index] = NULL; //si encuentro un IO valido, finaliza la arg_list con NULL
			break;
		}
		arg_list[i] = lista->palabra;
		if (i < listSize - 1) {
			lista = lista->siguienteNodo;
		}
	}
	arg_list[listSize] = NULL;
	spawn(arg_list[0], arg_list);

	/* Wait for the child process to complete. */
	wait(&child_status);

	borrarLista(lista);

	return 0;
}
