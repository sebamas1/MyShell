/*
 * infoProcess.c
 *
 *  Created on: 18 sep. 2020
 *      Author: sebastian
 */

#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

char infoProcesada[5000];
/**
 * Recibe un archivo, y copia la info necesaria en un string, para luego devolver ese string.
 * Nota: es necesario liberar el arreglo una vez que se termine de usar.
 */
static char* getInfo(FILE *archivo) {
	int size = 0;
	char letra;
	while ((letra = (char) getc(archivo)) != EOF) {
		size++;
	}
	rewind(archivo);
	char *info = malloc((unsigned int) size * sizeof(char));
	for (int i = 0; i < size - 1; i++) {
		info[i] = (char) getc(archivo);
	}
	return info;
}
/**
 * Guarda en infoProcesada el string cortado del parametro info, segun la regex pasada
 * como argumento.
 */
static void leerRegex(char *info, char *expresion) {
	regex_t regex;
	int reti;
	char msgbuf[100]; //100 es el lenght del posible msg de error
	regmatch_t matches[10]; //10 es la cantidad de grupos que se puede capturar

	memset(infoProcesada, 0, 5000);

	reti = regcomp(&regex, expresion, REG_EXTENDED + REG_NEWLINE);
	if (reti) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}
	reti = regexec(&regex, info, 10, matches, 0);
	if (!reti) {
		memcpy(infoProcesada, info + matches[1].rm_so,
				(size_t) matches[1].rm_eo - (size_t) matches[1].rm_so);
	} else if (reti == REG_NOMATCH) {
		puts("No match");
	} else {
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}
	regfree(&regex);
}
/*
 *Usa las funciones definidas antes para leer la informacion necesaria.
 */
char* buscarInfo(char path[], char regex[]) {
	FILE *archivo = fopen(path, "r");
	if(archivo == NULL){
		printf("Carpeta de proc incorrecta, cancelando ejecucion.\n");
		exit(EXIT_FAILURE);
	}
	char *info = getInfo(archivo);
	leerRegex(info, regex);
	fclose(archivo);
	free(info);
	return infoProcesada;
}
