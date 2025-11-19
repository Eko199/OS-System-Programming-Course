#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("No command or files given!\n");
		return -1;
	}

	int child = fork();

	if (child < 0) {
		perror("prob3.c");
		return -1;
	}

	if (child == 0) {
		int fd = open(argv[argc - 2], O_RDONLY);

		if (fd < 0) {
			perror("prob3.c");
			return -1;
		}

		dup2(fd, 0);
		close(fd);

		fd = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);

		if (fd < 0) {
			perror("prob3.c");
			return -1;
		}

		dup2(fd, 1);
		close(fd);

		argv[argc - 2] = 0;
		execvp(argv[1], argv + 1);
		perror("prob3.c");
		return -1;
	}

	int status;
	wait(&status);

	if (WIFEXITED(status)) {
		printf("Exit code: %d\n", WEXITSTATUS(status));
		return 0;
	}

	printf("Child terminated abnormally.");
	return 0;
}
