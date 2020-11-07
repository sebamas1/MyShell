/*
 * myshell.c
 *
 *  Created on: 13 oct. 2020
 *      Author: sebastian
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <grp.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include "ProgramParsing/programParsing.h"
#include "ProgramParsing/parseIO.h"
#include "SignalHandling/signalHandling.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int printPrompt() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char prompt[PATH_MAX];
		strcpy(prompt, getenv("USER"));
		strcat(prompt, "@");
		register gid_t gid;
		gid = getgid();
		strcat(prompt, getgrgid(gid)->gr_name);
		printf(ANSI_COLOR_YELLOW "%s", prompt);
		memset(prompt, 0, sizeof(prompt));
		printf(ANSI_COLOR_RESET "%s", ":");
		strcat(prompt, cwd);
		strcat(prompt, "$");
		printf(ANSI_COLOR_CYAN "%s " ANSI_COLOR_RESET, prompt);
		return 0;
	} else {
		perror("getcwd() error");
		return 1;
	}
}
static void ejecucionNormal() {
	while (!programTerminated()) {
		create_suspension_and_zombie_handler();
		if (sigsetjmp(env, 1) == 1) {
			restaurarSTDIO(); //bueno, esta linea se necesita aca si o si, igual para el batch file
			continue;
		}
		printPrompt();
		char *line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		parsearComando(line);
		free(line);
	}
}
static void ejecucionBatchFile(char *path) {
	FILE *archivo = fopen(path, "r");
	if (archivo == NULL) {
		archivo = fopen("./batch.sh", "r");
		if (archivo == NULL) {
			printf("Batch file no encontrado.\n");
			exit(EXIT_FAILURE);
		}
	}
	char *comando = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&comando, &len, archivo)) != -1) {
		create_suspension_and_zombie_handler();
		if (sigsetjmp(env, 1) == 1) {
			restaurarSTDIO();
			continue;
		}
		parsearComando(comando);
	}
	fclose(archivo);
}
int main(int argc, char *argv[]) {
	create_SIGTSTP_handler();
	create_SIGINT_handler();
	if (argc > 2) {
		fprintf(stderr, "Cantidad de argumentos incorrecta.\n");
		exit(EXIT_FAILURE);
	} else if (argc == 2) {
		ejecucionBatchFile(argv[1]);
		return 0;
	} else {
		ejecucionNormal();
		return 0;
	}
}
