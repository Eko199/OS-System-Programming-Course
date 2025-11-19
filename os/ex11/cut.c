#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int m, n;
	scanf("%d", &m);
	scanf("%d", &n);

	if (n < m || m < 1 || n < 1) {
		write(2, "Error\n", 6);
		return -1;
	}

	char buf[1];
	int fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		write(2, "No file\n", 8);
	}

	int i = 1;
	while (read(fd, buf, 1)) {
		if (buf[0] == '\n') {
			i = 1;
			write(1, "\n", 1);
			continue;
		}

		if (i >= m && i <= n) {
			write(1, buf, 1);
		}

		++i;
	}

	if (i == 1) {
		write(1, "\n", 1);
	}

	close(fd);
	return 0;
}
