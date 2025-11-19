#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {	
	if (argc < 3) {
		perror("fgrepq.c");
		return -1;
	}

	char curr, buf[80];
	int fd = open(argv[2], O_RDONLY);

	if (fd == -1) {
		perror("fgrepq.c");
		return -1;
	}

	while (read(fd, &curr, 1)) {
		if (curr != argv[1][0]) {
			continue;
		}

		int count = read(fd, buf, 80);
		int i;

		for (i = 0; i < count; ++i) {
			if (argv[1][i + 1] == '\0') {
				close(fd);
				exit(0);
			}

			if (argv[1][i + 1] != buf[i]) {
				break;
			}
		}
	}

	close(fd);
	return 1;
}
