#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* args[]) {
	int fd = open(args[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

	if (fd == -1) {
		perror("tee.c");
		return -1;
	}

	char buff[128];
	ssize_t count;

	while (count = read(0, buff, 128)) {
		write(1, buff, count);
		write(fd, buff, count);
	}

	close(fd);
	return 0;
}
