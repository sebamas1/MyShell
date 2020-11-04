/*
 * ParseIO.c
 *
 *  Created on: 4 nov. 2020
 *      Author: sebastian
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
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
void restaurarSTDIO(){
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
}
int generarComandoIOParseado(struct Nodo *lista) {
	saved_stdin = dup(0);
	saved_stdout = dup(1);

	int list_size = getSize(lista);
	struct Nodo *tmp = find(lista, 0);
	for (int i = 0; i < list_size; i++) {
		tmp = parseIOredirection(tmp);
		if(tmp == NULL){
			restaurarSTDIO();
			IO_encontrado = false;
			fprintf(stderr, "Comando incompleto.\n");
			return -1;
		}
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
