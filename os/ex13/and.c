#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		perror("Argument count");
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("Fork");
		return -1;
	}

	if (child == 0) {
		execlp(argv[1], argv[1], 0);
		perror("First command");
		return -1;
	}

	int status;
	child = wait(&status);

	if (status != 0) {
		return status;
	}

	execlp(argv[2], argv[2], 0);
	perror("Second command");
	return -1;
}
