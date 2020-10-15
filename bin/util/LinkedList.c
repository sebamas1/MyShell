/*
 * LinkedList.c
 *
 *  Created on: 15 oct. 2020
 *      Author: sebastian
 */
#include <stdlib.h>
#include <string.h>
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
		listSize++;
		head->index = 0;
	} else {
		current->siguienteNodo = nodo;
		int index = current->index;
		current = nodo;
		listSize++;
		current->index = index + 1;
	}
}
struct Nodo* getCabeza(){
	return head;
}

struct Nodo* crearLinkedList(char *string){
	guardarPalabra(strtok(string, " "));
	char *tmp;
	while((tmp = strtok(NULL, " ")) != NULL){
		guardarPalabra(tmp);
	}
	current->siguienteNodo = NULL;
    return getCabeza();
}
int getListSize(){
	return listSize;
}
struct Nodo* find(struct Nodo *nodo, int index){
	if(index <= listSize -1){
		struct Nodo *tmp = nodo;
		while(tmp->index != index){
			tmp = tmp->siguienteNodo;
		}
		return tmp;
	} else {
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
