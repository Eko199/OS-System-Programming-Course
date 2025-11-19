#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob3.c");
		return -1;
	}

	write(fds[1], "asdf\n", 5);
	char buf[5];
	int count;

	if (count = read(fds[0], buf, 5)) {
		write(1, buf, 5);
	}

	close(fds[1]);
	close(fds[0]);
	return 0;
}
