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
#include "../util/LinkedList.h"

static int spawn (char* program, char** arg_list)
{
	pid_t child_pid;

	/* Duplicate this process. */
	child_pid = fork ();

	if (child_pid != 0) {
		return child_pid;
	}
	else {
		execvp (program, arg_list);

		/* returns only if an error occurs. */
		perror("");
		abort ();
	}
}

int programInvocation(char *comando)
{
	struct Nodo *cabeza = crearLinkedList(comando);
	int child_status;
	/* The argument list to pass to the “ls” command. */
	int listSize = cabeza->listSize;
	char* arg_list[listSize + 1];
	int i = -1;
	do{
		i++;
		arg_list[i] = cabeza->palabra;
		cabeza = cabeza->siguienteNodo;
	} while(i < listSize - 1);
	arg_list[listSize] = NULL;


	/* Spawn a child process running the “ls” command. Ignore the
	returned child process ID. */
	spawn(arg_list[0], arg_list);

	/* Wait for the child process to complete. */
	wait(&child_status);

	borrarLista(cabeza);

	return 0;
}
