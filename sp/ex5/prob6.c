#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No command\n");
		return -1;
	}

	argv[argc] = 0;
	int child = fork();

	if (child < 0) {
		perror("prob5.c");
		return -1;
	} else if (child == 0) {
		execvp(argv[1], argv + 1);
		perror("prob5.c");
		return -1;
	}

	int status;
	wait(&status);
	
	if (WIFEXITED(status)) {
		printf("PID: %d, status code: %d\n", child, WEXITSTATUS(status));
	}
	
	return 0;
}
