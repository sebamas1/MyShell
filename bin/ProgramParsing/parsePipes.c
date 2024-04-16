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

/*
Se le pasa la lista doblemente enlazada para que guarde los comandos correspondientes a cada pipe.
*/
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

	//aca busca el primer |, corta la lista ahi, borra el | y guarda el comando que es hasta el |.
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
	//aca guarda el ultimo comando que queda.
	guardar_comando(tmp);
	return 0;
}

void limpiar_comand_list() {
	for (int i = 0; i < MAX_PIPES + 1; i++) {
		if (comandos[i] == NULL){
			break;
		}
		borrarLista(comandos[i]);
		if(comandos[i + 1] != NULL) borrarLista(comandos[i + 1]);
		comandos[i] = NULL;
		comandos[i + 1] = NULL;
		i++;
	}
}
// devuelve un array de listas doblemente enlazadas, cada una con un comando.
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
