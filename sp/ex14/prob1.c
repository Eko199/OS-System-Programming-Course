#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

void nothing() {}

int main(int argc, char* argv[]) {
	signal(SIGUSR1, nothing);
	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		return -1;
	}

	pid_t pid;

	if (child > 0) {
		sleep(2);
		pid = child;
	} else {
		pause();
		pid = getppid();
	}

	int i;
	for (i = 0; i < 5; ++i) {
		printf("%d\n", child);
		kill(pid, SIGUSR1);
		pause();
	}

	if (child > 0) {
		kill(child, SIGUSR1);
		wait(NULL);
	}

	return 0;
}
