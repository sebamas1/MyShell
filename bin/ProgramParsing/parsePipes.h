/*
 * parsePipes.h
 *
 *  Created on: 8 nov. 2020
 *      Author: sebastian
 */

#ifndef BIN_PROGRAMPARSING_PARSEPIPES_H_
#define BIN_PROGRAMPARSING_PARSEPIPES_H_

#include "../util/LinkedList.h"

struct Nodo** parse_pipes(struct Nodo *lista);
void limpiar_comand_list();
int get_max_pipes();

#endif /* BIN_PROGRAMPARSING_PARSEPIPES_H_ */
