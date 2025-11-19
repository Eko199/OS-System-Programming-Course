#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Invalid command!");
	}

	int fd1 = open(argv[1], O_RDONLY);

	if (fd1 < 0) {
		perror("prob1_2.c");
		return 1;
	}

	int fd2 = dup(fd1);

	if (fd2 < 0) {
		perror("prob1_2.c");
		close(fd1);
		return 1;
	}

	char c;
	int i;
	for (i = 0; i < 20 && read(fd1, &c, 1); ++i) {
		write(1, &c, 1);

		if (read(fd2, &c, 1)) {
			write(1, &c, 1);
		}
	}

	close(fd1);

	while (read(fd2, &c, 1)) {
		write(1, &c, 1);
	}

	close(fd2);
	return 0;
}
