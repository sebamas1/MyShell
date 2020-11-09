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
#include "parseIO.h"

static bool quit = false;
static pid_t current_child_pid = -1;

static void generar_comand_arguments(struct Nodo *lista, char **arg_list) {
	int list_size = getSize(lista);
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
}

static int ejecutar_comando_interno(struct Nodo *lista) {
	if (strcmp(lista->palabra, "cd") == 0) {
		changeDir(lista);
		return 0;
	} else if (strcmp(lista->palabra, "echo") == 0) {
		echo(lista);
		return 0;
	} else if (strcmp(lista->palabra, "quit") == 0) {
		quit = true;
		return 0;
	} else if (strcmp(lista->palabra, "clr") == 0) {
		printf ("\033c");
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
static void close_pipes(int fd[][2], int cant_comandos) {
	for (int i = 0; i < cant_comandos - 1; i++) {
		close(fd[i][0]);
		close(fd[i][1]);
	}
}

static int programInvocation(bool background, struct Nodo **comandos) {

	int cant_comandos = 0; // cuenta cant de comandos
	for (int i = 0; i < 1001; i++) {
		if (comandos[i] != NULL) {
			cant_comandos++;
		}
	}
	int fd[cant_comandos - 1][2];
	if (cant_comandos > 1) { //crea los pipes siempre que haya mas de un comando
		for (int i = 0; i < cant_comandos - 1; i++) {
			if (pipe(fd[i]) < 0) {
				fprintf(stderr, "Problemas con los pipes fd.\n");
				return -1;
			}
		}
	}
	pid_t child_pid[cant_comandos - 1];

	for (int i = 0; i < cant_comandos; i++) { //ejecuta los comandos uno a uno

		int list_size = getSize(comandos[i]);
		char *arg_list[list_size + 1];
		generar_comand_arguments(comandos[i], arg_list);

		current_child_pid = fork();

		child_pid[i] = current_child_pid;

		if (child_pid[i] == 0) { //child process
			if (cant_comandos > 1) {
				if (i < cant_comandos - 1)
					dup2(fd[i][1], STDOUT_FILENO);
				if (i - 1 >= 0)
					dup2(fd[i - 1][0], STDIN_FILENO);
				close_pipes(fd, cant_comandos);
			}

			if (background) {
				if (ejecutar_comando_interno(comandos[i]) == 0) {
					exit(EXIT_SUCCESS);
				}
			}
			execvp(arg_list[0], arg_list);

			perror("Child process");
			abort();

		} //end child process
	}
	if (cant_comandos > 1)
		close_pipes(fd, cant_comandos);

	for (int i = 0; i < cant_comandos; i++) {
		if (background) {
			printf("[%i] %d\n", get_job_id(child_pid[i]), child_pid[i]);
		} else {
			waitpid(child_pid[i], NULL, 0);
		}
	}
	return 0;
}

void programExecution(bool background, struct Nodo **comandos) {
	if (background) {
		programInvocation(background, comandos);
	} else {
		if (ejecutar_comando_interno(comandos[0]) == 0)
			return;
		programInvocation(background, comandos);
	}
}
bool terminateShell() {
	return quit;
}
void stop_child() {
	if (current_child_pid != -1) {
		kill(current_child_pid, SIGTSTP);
		printf("\n%i suspended by signal %i\n", current_child_pid, SIGTSTP);
		current_child_pid = -1;
	}
}
void sigint_child() {
	if (current_child_pid != -1) {
		kill(current_child_pid, SIGINT);
		current_child_pid = -1;
	}
}
