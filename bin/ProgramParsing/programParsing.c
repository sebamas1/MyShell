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
#include <limits.h>
#include "../util/LinkedList.h"

static bool comando_correcto = true;
static bool IO_encontrado = false;
static int saved_stdin;
static int saved_stdout;

static int redirectInput(struct Nodo *input) {

	int input_file = open(input->palabra, O_RDONLY, 0777);
	if (input_file == -1) {
		char cwd[PATH_MAX + 3];
		strcpy(cwd, "./");
		strcat(cwd, input->palabra);
		input_file = open(cwd, O_RDONLY, 0777);
		if (input_file == -1) {
			comando_correcto = false;
			fprintf(stderr,
					"Redireccion incorrecta del input: archivo no encontrado.\n");
			return -1;
		}
	}
	dup2(input_file, 0);
	return 0;
}
static int redirectOutput(struct Nodo *output) {

	if (output != NULL) {
		int output_file = open(output->palabra, O_WRONLY | O_CREAT, 0644);
		if (output_file == -1) {
			comando_correcto = false;
			fprintf(stderr, "Redireccion incorrecta del output.\n");
			return -1;
		}
		dup2(output_file, 1);
	}
	return 0;
}
/*
 * Busca en la linked list un nodo con > ó < , y corta ese nodo junto con el path que esta al
 * lado de ese nodo.
 *
 * El programa no va a aceptar cosas como <file > o < file> o <file>. Las llaves tienen
 * que ponerse asi < file >
 *
 * return
 * En caso de encontrarun caracter de IO valido, retorna una
 * referencia al nodo que esta despues del path tomado por > ó <.
 * En caso de no encontrar un caracter valido de IO, retorna el mismo nodo que se le paso como argument.
 * Si encuentra un formato invalido, retorna NULL
 */
static struct Nodo* parseIOredirection(struct Nodo *lista) {
	if (strncmp(lista->palabra, "<", 1) == 0) {
		if (strlen(lista->palabra) == 1 && lista->siguienteNodo != NULL) {
			struct Nodo *tmp = lista->siguienteNodo;
			redirectInput(lista->siguienteNodo);
			borrarNodo(lista);
			IO_encontrado = true;
			return borrarNodo(tmp); //borrarNodo va a devolver el siguiente nodo a tmp
		} else {
			fprintf(stderr, "Debe dejar espacios entre < y input_file.\n");
			comando_correcto = false;
			return lista;
		}
	}

	if (strncmp(lista->palabra, ">", 1) == 0) {
		if (strlen(lista->palabra) == 1 && lista->siguienteNodo != NULL) {
			struct Nodo *tmp = lista->siguienteNodo;
			redirectOutput(lista->siguienteNodo);
			borrarNodo(lista);
			IO_encontrado = true;
			return borrarNodo(tmp); //borrarNodo va a devolver el siguiente nodo a tmp
		} else {
			fprintf(stderr, "Debe dejar espacios entre > y output_file.\n");
			comando_correcto = false;
			return lista;
		}
	}
	return lista;
}
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
void restaurarSTDIO(){
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
}
int generarComandoParseado(struct Nodo *lista) {
	saved_stdin = dup(0);
	saved_stdout = dup(1);

	int list_size = getSize(lista);
	struct Nodo *tmp = find(lista, 0);
	for (int i = 0; i < list_size; i++) {
		tmp = parseIOredirection(tmp);
		if (IO_encontrado) {
			IO_encontrado = false;
			list_size -= 2; //saque dos elementos de la lista
			i--; //quiero que se sigan leyendo entradas despues de los IO
		} else {
			if (tmp->siguienteNodo != NULL) {
				tmp = tmp->siguienteNodo;
			}
		}
	}
	if(!comando_correcto){
		restaurarSTDIO();
		comando_correcto = true;
		return -1;
	}
	return list_size;
}
int programInvocation(char *comando) {
	struct Nodo *lista = crearLinkedList(comando);
	int listSize = generarComandoParseado(lista);
	if(listSize == -1){
		restaurarSTDIO();
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
