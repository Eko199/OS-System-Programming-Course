#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <utime.h>

int conv(int argc, const char* argv[]) {
	if (argc == 0) {
		return 0;
	}

	if (argc == 1) {
		execlp(argv[0], argv[0], 0);
		perror("prob1.c");
		return -1;
	}

	int fds[2];
	if (pipe(fds) < 0) {
		perror("prob1.c");
		return -1;
	}

	int child = fork();

	if (child < 0) {
		perror("prob1.c");
		return -1;
	}

	if (child == 0) {
		close(fds[0]);
		dup2(fds[1], 1);
		close(fds[1]);

		return conv(argc - 1, argv);
	}

	close(fds[1]);
	dup2(fds[0], 0);
	close(fds[0]);

	execlp(argv[argc - 1], argv[argc - 1], 0);
	perror("prob1.c");
	return -1;
}

int main(int argc, const char* argv[]) {
	if (argc < 4) {
		printf("Not enough arguments!");
		return -1;
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		perror("prob1.c");
		return -1;
	}

	dup2(fd, 0);
	close(fd);
	
	fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd < 0) {
		perror("prob1.c");
		return -1;
	}

	dup2(fd, 1);
	close(fd);
	
	return conv(argc - 3, argv + 2);
}
