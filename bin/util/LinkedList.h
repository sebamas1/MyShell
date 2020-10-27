/*
 * LinkedList.h
 *
 *  Created on: 15 oct. 2020
 *      Author: sebastian
 */

#ifndef BIN_UTIL_LINKEDLIST_H_
#define BIN_UTIL_LINKEDLIST_H_

struct Nodo {
	struct Nodo *siguienteNodo;
	struct Nodo *anteriorNodo;
	int index;
	int listSize;
	char *palabra;
};
struct Nodo* crearLinkedList(char *string);
void borrarLista(struct Nodo *nodo);
struct Nodo* find(struct Nodo* nodo, int index);

#endif /* BIN_UTIL_LINKEDLIST_H_ */
