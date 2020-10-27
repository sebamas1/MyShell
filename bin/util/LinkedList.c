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

struct Nodo* crearLinkedList(char *string){
	guardarPalabra(strtok(string, " \t\n"));
	char *tmp;
	while((tmp = strtok(NULL, " \t\n")) != NULL){
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
struct Nodo* find(struct Nodo *nodo, int index){
	if(index <= nodo->listSize -1){
		struct Nodo *tmp = nodo;
		while(tmp->index != index){
			tmp = tmp->siguienteNodo;
		}
		return tmp;
	} else {
		fprintf(stderr, "Algo fallo en la funcion find de la linked list");
		exit(EXIT_FAILURE); //si termina aca el programa, es tu culpa por poner un indice mal en algun lado
	}
}

void borrarLista(struct Nodo *nodo){
	listSize = 0;
	if(nodo == NULL){
		return;
	}
	struct Nodo *next = nodo->siguienteNodo;
	while(next != NULL){
		free(nodo);
		nodo = next;
		next = nodo->siguienteNodo;
	}
	free(nodo);
	head = NULL;
	current = NULL;
}
