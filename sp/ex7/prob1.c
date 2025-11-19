#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob1.c");
		return -1;
	}

	char c;
	//read(fds[0], &c, 1);
	close(fds[1]);

	printf("%d \n", read(fds[0], &c, 1));
	printf("%c \n", c);

	close(fds[0]);
	return 0;
}
