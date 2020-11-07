/*
 * signalHandling.h
 *
 *  Created on: 7 nov. 2020
 *      Author: sebastian
 */

#ifndef BIN_SIGNALHANDLING_SIGNALHANDLING_H_
#define BIN_SIGNALHANDLING_SIGNALHANDLING_H_

sigjmp_buf env;

void create_suspension_and_zombie_handler();
void create_SIGTSTP_handler();
void create_SIGINT_handler();

#endif /* BIN_SIGNALHANDLING_SIGNALHANDLING_H_ */
