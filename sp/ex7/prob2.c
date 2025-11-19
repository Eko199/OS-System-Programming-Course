#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int fds[2];

	if (pipe(fds) < 0) {
		perror("prob2.c");
		return -1;
	}

	close(fds[0]);
	printf("%d \n", write(fds[1], "asdf", 4));
	printf("please help");
	close(fds[1]);

	return 0;
}
