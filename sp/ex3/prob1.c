#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int fd = open("test1", O_CREAT | O_TRUNC, 0777);

	if (fd < 0) {
		perror("prob1.c");
		return -1;
	}

	mode_t old_mask = umask(0002);
	printf("%d \n", old_mask);

	close(fd);
	fd = open("new_test1", O_CREAT | O_TRUNC, 0777);

	if (fd < 0) {
		perror("prob1.c");
		return -1;
	}

	close(fd);
	old_mask = umask(old_mask);
	printf("%d \n", old_mask);

	return 0;
}
