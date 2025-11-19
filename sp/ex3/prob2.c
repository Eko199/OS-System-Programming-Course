#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		return 0;
	}
	
	int i;
	for (i = 1; i < argc; ++i) {
		int fd = open(argv[i], O_WRONLY | O_APPEND);

		if (fd < 0) {
			perror("prob2.c");
			return -1;
		}

		struct stat info;
		if (fstat(fd, &info) < 0) {
			perror("prob2.c");
			return -1;
		}

		if (fchmod(fd, (info.st_mode | S_IWGRP) & 0776) < 0) {
			perror("prob2.c");
			return -1;
		}

		close(fd);
	}

	return 0;
}
