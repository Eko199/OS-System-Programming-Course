#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int child = fork();

	if (child < 0) {
		perror("prob1.c");
		return -1;
	}

	int fd = open("test1", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd < 0) {
		perror("prob1.c");
		return -1;
	}

	int i;
	for (i = 0; i < 5; ++i) {
		if (child == 0) {
			write(fd, "child\n", 6);
			sleep(2);
		} else {
			write(fd, "parent\n", 7);
			sleep(3);
		}
	}

	close(fd);
	return 0;
}
