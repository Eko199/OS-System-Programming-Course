#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	printf("PID: %d\n", getpid());
	printf("Parent PID: %d\n", getppid());

	int result = execlp("ps", "ps", 0);
	printf("Exec result: %d\n", result);
	return 0;
}
