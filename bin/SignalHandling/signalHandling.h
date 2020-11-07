/*
 * signalHandling.h
 *
 *  Created on: 7 nov. 2020
 *      Author: sebastian
 */

#ifndef BIN_SIGNALHANDLING_SIGNALHANDLING_H_
#define BIN_SIGNALHANDLING_SIGNALHANDLING_H_

sigjmp_buf env;

void create_signal_handler();

#endif /* BIN_SIGNALHANDLING_SIGNALHANDLING_H_ */
