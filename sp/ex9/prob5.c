#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int counter = 0;
struct timespec nanot = {0, 1000};

void alarm_handler(int signum) {
	printf("%d\n", counter);
	alarm(1);
}

void int_handler(int signum) {
	counter = 0;
	printf("\n");
}

int main(int argc, char* argv[]) {
	signal(SIGALRM, alarm_handler);
	signal(SIGINT, int_handler);
	alarm(1);

	while (1) {
		++counter;
		nanosleep(&nanot, NULL);
	}

	return 0;
}
