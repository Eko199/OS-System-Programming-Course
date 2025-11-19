#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("No file name!\n");
		return -1;
	}
	
	if (link(argv[1], argv[2]) < 0 || unlink(argv[1]) < 0) {
		perror("prob4.c");
		return -1;
	}

	return 0;
}
