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
	int index;
	char *palabra;
};
struct Nodo* crearLinkedList(char *string);
struct Nodo* getCabeza();
void borrarLista(struct Nodo *nodo);
int getListSize();
struct Nodo* find(struct Nodo* nodo, int index);

#endif /* BIN_UTIL_LINKEDLIST_H_ */
