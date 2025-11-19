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
	char buf[1];

	while (read(fd, buf, 1)) {
		write(1, buf, 1);
		
		if (buf[0] == '\n') {
			++count;
		
			if (count >= 10) {
				break;
			}
		}
	}

	close(fd);
	return 0;
}
