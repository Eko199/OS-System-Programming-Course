#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	printf("Current PID: %d\n", getpid());
	int pid = fork();

	if (pid < 0) {
		perror("prob2.c");
		return -1;
	}

	printf("Fork result: %d\n", pid);
	printf("Current PID: %d\n", getpid());

	if (pid == 0) {
		printf("Parent PID: %d\n", getppid());
		return 0;
	}

	int status;
	if (wait(&status) < 0) {
		perror("prob2.c");
	}

	printf("Child status: %d\n", status);		
	return 0;
}
