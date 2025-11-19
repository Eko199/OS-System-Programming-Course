#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob4.c");
		return -1;
	}
	
	int child = fork();

	if (child < 0) {
		perror("prob4.c");
		return -1;
	}

	if (child == 0) {
		close(fds[1]);
		char buf[128];
		int c;

		while (c = read(fds[0], buf, 128)) {
			write(1, buf, c);
		}

		close(fds[0]);
		return 0;
	}

	close(fds[0]);
	int i;
	for (i = 1;  i < argc; ++i) {
		write(fds[1], argv[i], strlen(argv[i]));
		sleep(1);
	}

	wait(NULL);
	close(fds[1]);
	return 0;
}
