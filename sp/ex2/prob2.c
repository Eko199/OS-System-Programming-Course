#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Invalid command!");
	}

	int fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);

	if (fd < 0) {
		perror("prob2.c");
		return 1;
	}
	
	close(1);
	if (dup(fd) < 0) {
		perror("prob2.c");
		close(fd);
		return 1;
	}

	char c;

	while (read(0, &c, 1)) {
		write(1, &c, 1);
	}

	close(fd);
	close(1);
	return 0;
}
