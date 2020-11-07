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

void create_signal_handler() {
	struct sigaction sa;
	void delete_zombies();

	sigfillset(&sa.sa_mask);
	sa.sa_handler = delete_zombies;
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

void delete_zombies() {
	check_signals();

	pid_t kidpid;
	while ((kidpid = waitpid(-1, NULL, WNOHANG)) > 0) {
		fprintf(stdout, "\nProgram %i terminated\n", kidpid);
	}
	siglongjmp(env, 1);
}
