/*
 * programInvocation.h
 *
 *  Created on: 26 oct. 2020
 *      Author: sebastian
 */

#ifndef BIN_PROGRAMPARSING_PROGRAMPARSING_H_
#define BIN_PROGRAMPARSING_PROGRAMPARSING_H_

#include "../util/LinkedList.h"

int programInvocation(char *comando);
int generarComandoParseado(struct Nodo *lista);
void restaurarSTDIO();

#endif /* BIN_PROGRAMPARSING_PROGRAMPARSING_H_ */
