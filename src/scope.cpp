/*
  		One program to rule, one program to find them,
  One program to bring them all, and in the darkness bind them
*/

// This is that one program

#include "scope.h"
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <fstream>
#include <cstdlib>
#include <iostream>


pid_t Fork(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error: %s\n", strerror(errno));
		exit(1);
	}
	return pid;
}

void child_handler(int sig) {
	pid_t pid;
	while ((pid = waitpid(-1, NULL, 0)) > 0)
	return;
}

SCOPE::SCOPE() {
	// Regester SIGCHLD handler to reap dead children 
	if (signal(SIGCHLD, child_handler) == SIG_ERR) {
		fprintf(stderr, "signal error: %s\n", strerror(errno));
		exit(1);
	}

}

SCOPE::~SCOPE() {
	if(acquireSem)
		sem_close(acquireSem);
	if(trackSem)
		sem_close(trackSem);
}

void SCOPE::run() {
	std::string input;
	printf("Starting SCOPE\n");
	// Main Loop
	while(1) {
		printf("Choose Command: \n\n");
		printf("'t1' - Start test 1\n");
		printf("'t2' - Start test 2\n");
		printf("'a'  - Start acquire phase only\n");
		printf("'t'  - Start track phase only\n");
		printf("'o'  - Start orientation phase only\n");
		printf("'e'  - Stop SCOPE\n\n");
		printf("Enter Command: ");
		std::cin >> input;
		// Check options
		if(!input.compare("t1")) {
			if(test1()) {
				fprintf(stderr, "test 1 did not exit properly\n");
				exit(1);
			}
		} else if(!input.compare("t2")) {
			if(test2()) {
				fprintf(stderr, "test 2 did not exit properly\n");
				exit(1);
			}
		} else if(!input.compare("a")) {
			if(acquire()) {
				fprintf(stderr, "acquire phase did not exit properly\n");
				exit(1);
			}
		} else if(!input.compare("t")) {
			if(track()) {
				fprintf(stderr, "test phase did not exit properly\n");
				exit(1);
			}
		} else if(!input.compare("o")) {
			if(orientation()) {
				fprintf(stderr, "orientation phase did not exit properly\n");
				exit(1);
			}
		} else if(!input.compare("e")) {
			return;
		}
	}
}

int SCOPE::test1() {
	printf("Starting test 1\n");
	printf("Starting acquire phase\n");
	if(acquire()) {
		fprintf(stderr, "acquire phase did not exit properly\n");
		exit(1);
	}
	printf("Starting tracking phase\n");
	if(track()) {
		fprintf(stderr, "track phase did not exit properly\n");
		exit(1);
	}
	printf("Exiting test 1\n");
	return 0;
}

int SCOPE::test2() {
	printf("Starting test 2\n");
	printf("Starting tracking phase\n");
	if(track()) {
		fprintf(stderr, "track phase did not exit properly\n");
		exit(1);
	}
	printf("Starting orientation phase\n");
	if(orientation()) {
		fprintf(stderr, "orientation phase did not exit properly\n");
		exit(1);
	}
	printf("Exiting test 2\n");
	return 0;
}

int SCOPE::acquire() {
	std::string acq_sem = "/acq_sem";
	std::string acq_name = "test.py";
	if(create_process(acq_name, acq_sem, &acquireSem)) {
		fprintf(stderr, "create_process error\n");
		exit(1);
	}
	if(sem_wait(acquireSem)) {
		fprintf(stderr, "sem_wait error: %s\n", strerror(errno));
		exit(1);
	}
	return 0;
}

int SCOPE::track() {
	std::string trck_sem = "/trck_sem";
	std::string trck_name = "test.py";
	if(create_process(trck_name, trck_sem, &trackSem)) {
		fprintf(stderr, "create_process error\n");
		exit(1);
	}
	if(sem_wait(trackSem)) {
		fprintf(stderr, "sem_wait error: %s\n", strerror(errno));
		exit(1);
	}
	return 0;
}

int SCOPE::orientation() {
	pid_t pid;
	std::string ornt_name = "pcExe";
	if(create_process(ornt_name, "", NULL)) {
		fprintf(stderr, "create_process error\n");
		exit(1);
	}
	pid = (waitpid(-1, NULL, 0));
	printf("%d\n", pid);
	return 0;
}

int SCOPE::create_process(std::string exeName, std::string semName, sem_t **sem) {
	pid_t pid;
	char *exe = strdup(exeName.c_str());
	char *name = strdup(semName.c_str());
	char *const command[3] = {exe, name, NULL};
	// Initialize semaphore between parent/child
	if(!semName.empty()) {
		*sem = sem_open(semName.c_str(), O_CREAT, 0644, 0);
	}

	if(!(pid = Fork())) {
		printf("%s\n", exeName.c_str());
		if(execv(exeName.c_str(), command) < 0) {
			printf("failure\n");
		}
	}
	return 0;
}