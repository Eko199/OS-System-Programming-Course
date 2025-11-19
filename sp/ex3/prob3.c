#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file name!\n");
		return -1;
	}
	
	int fd = open(argv[1], O_WRONLY | O_APPEND);

	if (fd < 0) {
		perror("prob3.c");
		return -1;
	}

	struct stat info;
	if (fstat(fd, &info) < 0) {
		perror("prob3.c");
	}

	printf("Access time: %s \n", ctime(&info.st_atime));
	printf("Modify time: %s \n", ctime(&info.st_mtime));
	
	info.st_atime -= 3600;
	info.st_mtime += 60;

	struct utimbuf buf = { info.st_atime, info.st_mtime };

	if (utime(argv[1], &buf) < 0) {
		perror("prob3.c");
		return -1;
	}
	
	if (fstat(fd, &info) < 0) {
		perror("prob3.c");
		return -1;
	}


	printf("Access time: %s \n", ctime(&info.st_atime));
	printf("Modify time: %s \n", ctime(&info.st_mtime));

	close(fd);
	return 0;
}
