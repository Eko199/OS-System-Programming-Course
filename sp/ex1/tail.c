#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Invalid command!");
		return -1;
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		perror("Invalid file");
		return -1;
	}

	int count = 0;
	char buf;

	while (read(fd, &buf, 1)) {
		count += buf == '\n';
	}

	lseek(fd, 0, SEEK_SET);

	int curr = 0;
	while (read(fd, &buf, 1)) {
		if (curr >= count - 10) {
			write(1, &buf, 1);
		}

		curr += buf == '\n';
	}

	close(fd);
	return 0;
}
