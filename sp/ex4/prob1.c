#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	printf("Current process: %d\n", getpid());
	printf("Parent process: %d\n", getppid());
	
	execlp("ps", "ps", 0);
	perror("prob1.c");
	return -1;
}
