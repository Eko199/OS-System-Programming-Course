#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* files[]) {
	if (argc < 3) {
		printf("Invalid command!");
	}

	int fd1 = open(files[1], O_RDONLY);

	if (fd1 < 0) {
		perror("cp.c");
		return 1;
	}

	int fd2 = open(files[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd2 < 0) {
		perror("cp.c");
		close(fd1);
		return 1;
	}

	char buf[32];
	ssize_t count;

	while (count = read(fd1, buf, 32)) {
		write(fd2, buf, count);
	}
	
	close(fd1);
	close(fd2);
	return 0;
}
