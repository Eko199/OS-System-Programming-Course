#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	printf("PID: %d\n", getpid());
	printf("Parent PID: %d\n", getppid());

	pid_t child = fork();
	printf("Fork result: %d\nPID:%d\nParent PID: %d\n", child, getpid(), getppid());

	if (child > 0) {
		int status;
		child = wait(&status);
		printf("Child finished - pid %d, status %d\n", child, status);
	}
	return 0;
}
