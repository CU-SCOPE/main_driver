/*
  		One program to rule, one program to find them,
  One program to bring them all, and in the darkness bind them
*/

// This is that one program

#include "scope.h"
#include <sys/types.h>
#include <unistd.h>

pid_t Fork(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error: %s\n", strerror(errno));
		exit(1);
	}
	return pid;
}

void child_handler() {
	pid_t pid;
	while ((pid = waitpid(-1, NULL, 0)) > 0)
	if (errno != ECHILD) {
		fprintf(stderr, "waitpid error: %s\n", strerror(errno));
		exit(1);
	}
	return;
}

SCOPE::SCOPE() {
	// Regester SIGCHLD handler to reap dead children 
	if (signal(SIGCHLD, child_handler) == SIG_ERR) {
		fprintf(stderr, "signal error: %s\n", strerror(errno));
		exit(1);
	}

}

int SCOPE::create_process(char *exeName) {
	char argv[] = {NULL, NULL};
	char envp[] = {NULL};
	int mypipe[2];
	argv[0] = exeName;
	// Initialize communication with child
	if (pipe(mypipe)){
		fprintf (stderr, "Pipe failed.\n");
		exit(1);
    }

	if(!Fork()) {
		execve(exeName, argv, envp);
	}
}