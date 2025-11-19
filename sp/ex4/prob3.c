#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	char file[256];
	scanf("%s", file);

	int child = fork();

	if (child < 0) {
		perror("prob3.c");
		return -1;
	}
	
	if (child == 0) {
		execlp(file, file, 0);
		perror("prob3.c");
		return -1;
	}
	
	int status;
	if (wait(&status) < 0) {
		perror("prob3.c");
		return -1;
	}

	if (WIFEXITED(status)) {
		printf("Child exit status: %d\n", WEXITSTATUS(status));
		return 0;
	}

	printf("Child did not exit normally!");
	return -1;
}
