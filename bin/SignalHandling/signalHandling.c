/*
 * signalHandling.c
 *
 *  Created on: 7 nov. 2020
 *      Author: sebastian
 */
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include "signalHandling.h"
#include "../ProgramParsing/programExecution.h"
#include "jobs.h"

void create_suspension_and_zombie_handler() {
	struct sigaction sa;
	void check_signal_and_delete_zombies();

	sigfillset(&sa.sa_mask);
	sa.sa_handler = check_signal_and_delete_zombies;
	sa.sa_flags = 0;
	sigaction(SIGCHLD, &sa, NULL);
}

static void check_signals() {

	for (int i = 1; i < MAX_JOBS; i++) {
		if(job_id[i] == 0) return;
		int wstatus;
		int kid_pid;
		kid_pid = waitpid(job_id[i], &wstatus, WNOHANG | WSTOPPED);
		if (WIFSTOPPED(wstatus)) {
			fprintf(stdout, "\n[%i] %i suspended by signal %i\n", i,
					job_id[i], WSTOPSIG(wstatus));
			break;
		}

		int status;
		waitpid(job_id[i], &status, WNOHANG | WCONTINUED);
		if (WIFCONTINUED(status)) {
			fprintf(stdout, "\n[%i] %i resumed\n", i, job_id[i]);
			break;
		}
		if(WIFEXITED(wstatus) && kid_pid != -1){
			fprintf(stdout, "\nProgram %i terminated\n", kid_pid);
		}
	}
}

void check_signal_and_delete_zombies() {
	check_signals();
	siglongjmp(env, 1);
}

void create_SIGTSTP_handler() {
	struct sigaction sa;
	void enviar_SIGSTP();

	sigfillset(&sa.sa_mask);
	sa.sa_handler = enviar_SIGSTP;
	sa.sa_flags = 0;
	sigaction(SIGTSTP, &sa, NULL);
}
void enviar_SIGSTP(){
	stop_child();
	siglongjmp(env, 1);
}

void create_SIGINT_handler() {
	struct sigaction sa;
	void enviar_SIGINT();

	sigfillset(&sa.sa_mask);
	sa.sa_handler = enviar_SIGINT;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void enviar_SIGINT(){
	sigint_child();
	siglongjmp(env, 1);
}
