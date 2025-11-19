#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* files[]) {
	char buf[10];
	int fd1 = open(files[1], O_RDONLY);
	int fd2 = open(files[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
	ssize_t count;

	do {
		count = read(fd1, buf, 10);
		write(fd2, buf, count);
	} while (count > 0);
	
	close(fd1);
	close(fd2);
	return 0;
}
