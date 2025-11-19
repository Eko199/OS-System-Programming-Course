#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No directory provided!\n");
		return -1;
	}

	if (chdir(argv[1]) < 0) {
		perror("cd.c");
		return -1;
	}

	return 0;
}
