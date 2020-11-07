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
#include <string.h>
#include <stdbool.h>
#include "../util/LinkedList.h"
#include "../InCommands/internalCommands.h"
#include "../SignalHandling/jobs.h"

static bool quit = false;
static struct Nodo *saved_list;

static int ejecutar_comando_interno() {
	if (strcmp(saved_list->palabra, "cd") == 0) {
		changeDir(saved_list);
		return 0;
	} else if (strcmp(saved_list->palabra, "echo") == 0) {
		echo(saved_list);
		return 0;
	} else if (strcmp(saved_list->palabra, "quit") == 0) {
		quit = true;
		return 0;
	} else if (strcmp(saved_list->palabra, "clr") == 0) {
		system("clear");      //ACORDATE QUE TENES QUE CAMBIAR ESTO
		return 0;
	}
	return -1;
}

static int get_job_id(pid_t child_pid) {
	for (int i = 1; i < MAX_JOBS; i++) {

		if (job_id[i] == 0) {
			job_id[i] = child_pid;
			return i;
		}
	}

	return -1;
}

static int spawn(char *program, char **arg_list, bool background) {
	pid_t child_pid;

	/* Duplicate this process. */
	child_pid = fork();

	if (child_pid != 0) {
		return child_pid;
	} else {
		if (background) {
			if (ejecutar_comando_interno() == 0) {
				exit(EXIT_SUCCESS);
			}
		}
		execvp(program, arg_list);

		/* returns only if an error occurs. */
		perror("Child process");
		abort();
	}
}

static int programInvocation(struct Nodo *lista, bool background) {
	int list_size = getSize(lista);
	char *arg_list[list_size + 1];
	lista = find(lista, 0); //vuelvo al principio de la lista(si es que no estoy)
	for (int i = 0; i < list_size; i++) {
		if (lista != NULL) {
			arg_list[i] = lista->palabra;
		}

		if (i < list_size - 1) { //podria perder la ref. y despues no puedo limpiar la lista
			lista = lista->siguienteNodo;
		}
	}

	arg_list[list_size] = NULL;

	int child_status;

	pid_t child_pid = spawn(arg_list[0], arg_list, background);

	if (background) {
		waitpid(child_pid, NULL, WNOHANG);
		printf("[%i] %d\n", get_job_id(child_pid), child_pid);
	} else {
		wait(&child_status);
	}

	return 0;
}

void programExecution(struct Nodo *lista, bool background) {
	saved_list = lista;
	if (background) {
		programInvocation(lista, background);
	} else {
		if (ejecutar_comando_interno() == 0)
			return;
		programInvocation(lista, background);
	}

}
bool terminateShell() {
	return quit;
}
