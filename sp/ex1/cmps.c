#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* files[]) {
	if (argc < 3) {
		printf("Invalid command!");
	}

	int fd1 = open(files[1], O_RDONLY);

	if (fd1 < 0) {
		perror("cmps.c");
		return 2;
	}

	int fd2 = open(files[2], O_RDONLY);

	if (fd2 < 0) {
		perror("cmps.c");
		close(fd1);
		return 2;
	}

	char buf1, buf2;

	while (read(fd1, &buf, 1)) {
		if (!read(fd2, &buf, 1) || buf1 != buf2) {
			close(fd1);
			close(fd2);
			return 1;
		}
	}

	bool res = read(fd2, &buf, 1);
	
	close(fd1);
	close(fd2);
	return res;
}
