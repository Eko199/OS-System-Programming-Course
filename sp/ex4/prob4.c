#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	char file1[256], file2[256];
	scanf("%s %s", file1, file2);

	int child1 = fork();

	if (child1 < 0) {
		perror("prob4.c");
		return -1;
	}
	
	if (child1 == 0) {
		execlp(file1, file1, 0);
		perror("prob4.c");
		return -1;
	}

	int child2 = fork();

	if (child2 < 0) {
		perror("prob4.c");
		return -1;
	}

	if (child2 == 0) {
		execlp(file2, file2, 0);
		perror("prob4.c");
		return -1;
	}
	
	int status1, status2;
	pid_t pid1 = wait(&status1);
	pid_t pid2 = wait(&status2);
	if (pid1 < 0 || pid2 < 0) {
		perror("prob4.c");
		return -1;
	}

	if (WIFEXITED(status1)) {
		printf("%s exit code: %d\n", (pid1 == child1) ? file1 : file2, WEXITSTATUS(status1));
	} else {
		printf("%s did not exit normally!", (pid1 == child1) ? file1 : file2);
	}

	char* cmd2 = (pid2 == child1) ? file1 : file2;
	if (WIFEXITED(status2)) {
		printf("%s exit code: %d\n", cmd2, WEXITSTATUS(status2));
	} else {
		printf("%s did not exit normally!", cmd2);
	}

	return 0;
}
