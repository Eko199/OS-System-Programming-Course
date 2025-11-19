#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Invalid command!");
		return -1;
	}

	int fd = open(argv[2], O_RDONLY);

	if (fd < 0) {
		perror("Invalid file");
		return -1;
	}

	int count;
	char buf[80];

	while (count = read(fd, &buf, 80)) {
		char* pos = strchr(buf, '\n');
		*pos = '\0';

		lseek(fd, pos - buf + 1 - count, SEEK_CUR);
		if (strstr(buf, argv[1]) != NULL) {
			write(1, buf, pos - buf + 1);
			write(1, "\n", 1);
		}
	}

	close(fd);
	return 0;
}
