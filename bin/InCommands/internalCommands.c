/*
 * internalCommands.c
 *
 *  Created on: 14 oct. 2020
 *      Author: sebastian
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int changeDirectory(char *path){
	if(chdir(path) != 0){
		printf("%s", "No se encuentra el directorio especificado.\n");
		return -1;
	} else {
		char env[20];
		strcpy(env, "PWD=");
		strcat(env, path);
		if(putenv(env) != 0){
			printf("%s", "Error al cambiar PWD.\n");
			return -1;
		}
		puts(getenv("PWD"));
		return 0;
	}
}
int echo(char *texto){
	printf("%s\n", texto);
	return 0;
}

