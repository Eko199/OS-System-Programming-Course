#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Invalid command!");
		return -1;
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		perror("Invalid file");
		return -1;
	}

	int m = atoi(argv[2]), n = atoi(argv[3]);
	int count = 1;
	char buf;

	while (read(fd, &buf, 1)) {
		if (count >= m && count <= n || buf == '\n') {
			write(1, &buf, 1);
		}
		
		++count;

		if (buf == '\n') {
			count = 1;
		}
	}

	close(fd);
	return 0;
}
