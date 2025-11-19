#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Need 3 commands!");
		return -1;
	}

	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob8.c");
		return -1;
	}
	
	int child = fork();

	if (child < 0) {
		perror("prob8.c");
		return -1;
	}

	if (child == 0) {
		close(fds[0]);
		int fds2[2];

		if (pipe(fds2) < 0) {
			perror("prob8.c");
			return -1;
		}

		int grandchild = fork();

		if (grandchild < 0) {
			perror("prob8.c");
			return -1;
		}

		if (grandchild == 0) {
			close(fds[1]);
			close(fds2[0]);
			dup2(fds2[1], 1);
			close(fds2[1]);

			execlp(argv[1], argv[1], 0);
			perror("prob8.c");
			return -1;
		}
		
		close(fds2[1]);
		dup2(fds2[0], 0);
		close(fds2[0]);

		dup2(fds[1], 1);
		close(fds[1]);
		
		execlp(argv[2], argv[2], 0);
		perror("prob8.c");
		return -1;
	}
	
	close(fds[1]);
	dup2(fds[0], 0);
	close(fds[0]);

	execlp(argv[3], argv[3], 0);
	perror("prob8.c");
	return -1;
}
