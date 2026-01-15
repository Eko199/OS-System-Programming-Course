#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int kill_attempts = 0;

void term_handler(int signum) {
	++kill_attempts;
	printf("Termination attempt %d.\n", kill_attempts);
}

int main(int argc, char* argv[]) {
	int child = fork();
	
	if (child < 0) {
		perror("prob6.c");
		return -1;
	}

	if (child > 0) {
		sleep(1);
		if (kill(child, SIGTERM) < 0) {
			perror("kill");
			return -1;
		}

		sleep(1);
		if (kill(child, SIGTERM) < 0) {
			perror("kill");
			return -1;
		}

		wait(NULL);
		return 0;
	}

	signal(SIGTERM, term_handler);
	while (kill_attempts < 2);
	return 0;
}
