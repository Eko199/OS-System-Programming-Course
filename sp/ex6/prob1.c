#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No command given!\n");
		return -1;
	}

	int child = fork();

	if (child < 0) {
		perror("prob1.c");
		return -1;
	}

	if (child == 0) {
		argv[argc] = 0;
		execvp(argv[1], argv + 1);
		perror("prob1.c");
		return -1;
	}

	int status;
	child = wait(&status);

	if (WIFEXITED(status)) {
		printf("Child PID: %d, exit code: %d\n", child, WEXITSTATUS(status));
		return 0;
	}

	printf("Child terminated abnormally.");
	return 0;
}
