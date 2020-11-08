/*
 * parsePipes.c
 *
 *  Created on: 8 nov. 2020
 *      Author: sebastian
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/LinkedList.h"

#define MAX_PIPES 1000

struct Nodo* comandos[MAX_PIPES + 1] =  { NULL } ;

static void guardar_comando(struct Nodo *lista) {
	for (int i = 0; i < MAX_PIPES + 1; i++) {
		if (comandos[i] == NULL) {
			if(lista->anteriorNodo == NULL){
				comandos[i] = lista;
				break;
			} else {
				comandos[i] = lista->anteriorNodo;
				break;
			}
		}
	}
}

static int findPipes(struct Nodo *lista) {
	struct Nodo *tmp = find(lista, 0);
	int list_size = getSize(lista);
	for (int i = 0; i < list_size; i++) {
		if (strncmp(tmp->palabra, "|", 1) == 0) {
			if (strlen(tmp->palabra)
					== 1&& tmp->anteriorNodo != NULL && tmp->siguienteNodo != NULL) {
				guardar_comando(tmp);
				tmp = cortarLista(tmp);
			} else {
				fprintf(stderr, "Error en el pipe parsing.\n");
				return -1;
			}
		}
		if (tmp->siguienteNodo != NULL) {
			tmp = tmp->siguienteNodo;
		}
	}
	guardar_comando(tmp);
	return 0;
}
void limpiar_comand_list() {
	for (int i = 0; i < MAX_PIPES + 1; i++) {
		if (comandos[i] == NULL){
			break;
		}
		comandos[i] = NULL;
		comandos[i + 1] = NULL;
		i++;
	}
}
struct Nodo** parse_pipes(struct Nodo *lista) {
	if (findPipes(lista) == -1) {
		limpiar_comand_list();
		return NULL;
	}
	for (int i = 0; i < MAX_PIPES + 1; i++) {
		if (comandos[i] != NULL){
			comandos[i] = find(comandos[i], 0);
		} else {
			break;
		}
	}

	return comandos;
}
int get_max_pipes(){
	return MAX_PIPES;
}
