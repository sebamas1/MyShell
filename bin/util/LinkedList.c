/*
 * LinkedList.c
 *
 *  Created on: 15 oct. 2020
 *      Author: sebastian
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkedList.h"

static struct Nodo *head = NULL;
static struct Nodo *current = NULL;
static int listSize = 0;

static void guardarPalabra(char *palabra) {
	struct Nodo *nodo = (struct Nodo*) malloc(sizeof(struct Nodo));
	nodo->palabra = palabra;
	if (current == NULL) {
		head = nodo;
		current = nodo;
		nodo->anteriorNodo = NULL;
		listSize++;
		head->index = 0;
	} else {
		current->siguienteNodo = nodo;
		nodo->anteriorNodo = current;
		int index = current->index;
		current = nodo;
		listSize++;
		current->index = index + 1;
	}
}

struct Nodo* crearLinkedList(char *string) {
	guardarPalabra(strtok(string, " \t\n"));
	char *tmp;
	while ((tmp = strtok(NULL, " \t\n")) != NULL) {
		guardarPalabra(tmp);
	}
	current->siguienteNodo = NULL;
	struct Nodo *nodo = head;
	nodo->listSize = listSize;
	head = NULL;
	current = NULL;
	listSize = 0;
	return nodo;
}
struct Nodo* find(struct Nodo *nodo, int index) {
	struct Nodo *tmp = nodo;
	while (tmp->index != 0) {
		tmp = tmp->anteriorNodo;
	}
	if (index < tmp->listSize) {
		while (tmp->index != index) {
			tmp = tmp->siguienteNodo;
		}
		return tmp;
	} else {
		fprintf(stderr,
				"Algo fallo en la funcion find de la linked list(pediste un nodo inexistente).\n");
		exit(EXIT_FAILURE); //si termina aca el programa, es tu culpa por poner un indice mal en algun lado
	}
}

int borrarLista(struct Nodo *nodo) {
	listSize = 0;
	if (nodo == NULL) {
		fprintf(stderr, "borrarLista: pasaste un nodo que apunta a NULL\n");
		return -1;
	}
	while (nodo->index != 0) {
		nodo = nodo->anteriorNodo;
	}
	struct Nodo *next = nodo->siguienteNodo;
	while (next != NULL) {
		free(nodo);
		nodo = next;
		next = nodo->siguienteNodo;
	}
	free(nodo);
	current = NULL;
	return 0;
}
int getSize(struct Nodo *nodo) {
	if (nodo == NULL) {
		fprintf(stderr, "getSize: pasaste un nodo que apunta a NULL");
		return -1;
	}
	struct Nodo *tmp = nodo;
	while (tmp->index != 0) {
		tmp = tmp->anteriorNodo;
	}
	return tmp->listSize;
}
static void actualizarLista(struct Nodo *nodo) {
	struct Nodo *tmp = nodo;
	while (!(tmp == NULL)) {
		tmp->index--;
		tmp = tmp->siguienteNodo;
	}
	tmp = nodo;
	find(tmp, 0)->listSize--;
}
struct Nodo* borrarNodo(struct Nodo *nodo) {
	struct Nodo *tmp;
	if (nodo == NULL) {
		fprintf(stderr, "borrarNodo: pasaste un nodo que apunta a NULL");
		return NULL;
	}
	if (nodo->anteriorNodo == NULL) {
		tmp = nodo->siguienteNodo;
		if (tmp == NULL) {
			return NULL;
		}
		tmp->anteriorNodo = NULL;
		tmp->listSize = nodo->listSize - 1;
		free(nodo);
		tmp->index = 0;
		return tmp;
	} else if (nodo->siguienteNodo == NULL) {
		tmp = nodo->anteriorNodo;
		tmp->siguienteNodo = NULL;
		free(nodo);
		find(tmp, 0)->listSize--;
		return tmp;
	} else {
		struct Nodo *siguiente;
		siguiente = nodo->siguienteNodo;
		tmp = nodo->anteriorNodo;
		tmp->siguienteNodo = siguiente;
		siguiente->anteriorNodo = tmp;
		free(nodo);
		actualizarLista(siguiente);
		return siguiente;
	}
}
static void actualizarListasCortadas(struct Nodo *lista) {
	int i = 0;
	if (lista->siguienteNodo != NULL) {
		while (lista->siguienteNodo != NULL) {
			lista->index = i;
			i++;
			lista = lista->siguienteNodo;
		}
		lista->index = i;
		while(lista->anteriorNodo != NULL)lista = lista->anteriorNodo;
		lista->listSize = i + 1;
		return;
	}
	if (lista->anteriorNodo != NULL) {
		while (lista->anteriorNodo != NULL){
			i++;
			lista = lista->anteriorNodo;
		}
		int index = 0;
		lista->listSize = i + 1;
		while(lista->siguienteNodo != NULL){
			lista->index = index;
			index++;
			lista = lista->siguienteNodo;
		}
		lista->index = index;
		find(lista, 0);
		return;
	}
	lista->index = 0;
	lista->listSize = 1;
}
struct Nodo* cortarLista(struct Nodo *nodo) {
	struct Nodo *siguiente_nodo;
	struct Nodo *anterior_nodo;
	if (nodo == NULL) {
		fprintf(stderr, "cortar_lista: pasaste un nodo que apunta a NULL");
		return NULL;
	}
	siguiente_nodo = nodo->siguienteNodo;
	anterior_nodo = nodo->anteriorNodo;
	if (siguiente_nodo == NULL || anterior_nodo == NULL) {
		return NULL;
	}
	siguiente_nodo->anteriorNodo = NULL;
	anterior_nodo->siguienteNodo = NULL;
	free(nodo);
	actualizarListasCortadas(anterior_nodo);
	actualizarListasCortadas(siguiente_nodo);
	return siguiente_nodo;
}
