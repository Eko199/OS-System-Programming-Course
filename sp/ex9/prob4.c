#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

int counter = 0;

void alarm_handler(int signum) {
	printf("%d\n", counter);
	alarm(1);
}

int main(int argc, char* argv[]) {
	signal(SIGALRM, alarm_handler);
	alarm(1);

	while (1) {
		++counter;
	}

	return 0;
}
