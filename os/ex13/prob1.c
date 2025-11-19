#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		perror("prob1.c");
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("prob1.c");
		return -1;
	}

	if (child == 0) {
		execvp(argv[1], argv + 1);
		perror("Child execution");
	} else {
		int status;
		pid_t pid = wait(&status);
		printf("Child %d finished with status %d.\n", pid, status);
	}

	return 0;
}
