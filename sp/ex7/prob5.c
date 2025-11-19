#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Need 2 commands!");
		return -1;
	}

	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob5.c");
		return -1;
	}
	
	int child = fork();

	if (child < 0) {
		perror("prob5.c");
		return -1;
	}

	if (child > 0) {
		close(fds[1]);
		dup2(fds[0], 0);
		close(fds[0]);
		
		execlp(argv[2], argv[2], 0);
		perror("prob5.c");
		return -1;
	}

	close(fds[0]);
	dup2(fds[1], 1);
	close(fds[1]);

	execlp(argv[1], argv[1], 0);
	perror("prob5.c");
	return -1;
}
