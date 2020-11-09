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

int programExecution(bool background, struct Nodo** comandos);
bool terminateShell();
void stop_child();
void sigint_child();

#endif /* BIN_PROGRAMPARSING_PROGRAMEXECUTION_H_ */
