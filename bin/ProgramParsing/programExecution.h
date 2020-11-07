/*
 * programInvocation.h
 *
 *  Created on: 26 oct. 2020
 *      Author: sebastian
 */

#ifndef BIN_PROGRAMPARSING_PROGRAMEXECUTION_H_
#define BIN_PROGRAMPARSING_PROGRAMEXECUTION_H_

#include <stdbool.h>
#include "../util/LinkedList.h"

int programExecution(struct Nodo *lista, bool background);
bool terminateShell();

#endif /* BIN_PROGRAMPARSING_PROGRAMEXECUTION_H_ */
