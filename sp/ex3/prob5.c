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
	
	if (symlink(argv[1], argv[2]) < 0) {
		perror("prob5.c");
		return -1;
	}

	char buf[255];
	int count = readlink(argv[2], buf, 255);

	if (count < 0) {
		perror("prob5.c");
		return -1;
	}

	write(1, buf, count);
	printf("\n");

	return 0;
}
