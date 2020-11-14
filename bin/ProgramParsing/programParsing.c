/*
 * programParsing.c
 *
 *  Created on: 5 nov. 2020
 *      Author: sebastian
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <sys/wait.h>
#include "../util/LinkedList.h"
#include "programExecution.h"
#include "parseIO.h"
#include "parsePipes.h"

static bool background = false;

static struct Nodo* parseBackground(struct Nodo *lista) {
	struct Nodo *tmp = find(lista, 0);
	for (int i = 0; i < getSize(lista); i++) {
		if (strncmp(tmp->palabra, "&", 1) == 0) {
			if (strlen(tmp->palabra) == 1) {
				background = true;
				return find(borrarNodo(tmp), 0); //borra el & y devuelve la cabeza de la lista
			} else {
				background = false;
				fprintf(stderr,
						"Debe dejar espacios entre & y el resto de los comandos.\n");
				return NULL;
			}
		}
		if (tmp->siguienteNodo != NULL) {
			tmp = tmp->siguienteNodo;
		}
	}
	return lista;
}
void limpiarPrograma() {
	background = false;
	restaurarSTDIO();
	limpiar_comand_list();
}

int parsearComando(char *line) {
	if (strcmp(line, "\n") == 0)
		return 0;

	struct Nodo *lista = crearLinkedList(line); //cuando haya IO redirection, y pipes, cancelar ejecucion

	struct Nodo *tmp = generarComandoIOParseado(lista);
	if (tmp == NULL) {
		return -1;
	} else {
		lista = tmp;
	}

	tmp = parseBackground(lista);
	if (tmp == NULL) {
		return -1;
	} else {
		lista = tmp;
	}

	struct Nodo **comandos;
	if ((comandos = parse_pipes(lista)) == NULL) {
		return -1;
	}

	programExecution(background, comandos);
	limpiarPrograma();

	return 0;
}

bool programTerminated() {
	return terminateShell();
}
