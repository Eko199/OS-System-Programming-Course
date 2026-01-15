#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

void child_handler(int signum) {
	printf("CHILD HAS BEEN EXECUTED\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	signal(SIGCHLD, child_handler);

	int child = fork();
	
	if (child < 0) {
		perror("prob3.c");
		return -1;
	}

	if (child > 0) {
		while(1);
	}

	return 0;
}
