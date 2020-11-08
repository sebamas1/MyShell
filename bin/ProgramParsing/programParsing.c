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
		if(tmp->siguienteNodo != NULL){
			tmp = tmp->siguienteNodo;
		}
	}
	return lista;
}

int parsearComando(char *line) {
	if (strcmp(line, "\n") == 0)
		return 0;

	struct Nodo *lista = crearLinkedList(line);

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

	programExecution(lista, background);

	background = false;

	restaurarSTDIO();

//	int max_pipes = get_max_pipes();
//	struct Nodo** comandos;
//	comandos = parse_pipes(lista);
//	for(int i = 0; i < max_pipes; i++){
//		if(comandos[i] == NULL) break;
//		printf("%s\n", comandos[i]->palabra);
//	}

//	limpiar_comand_list();

	borrarLista(lista);

	return 0;
}

bool programTerminated() {
	return terminateShell();
}
