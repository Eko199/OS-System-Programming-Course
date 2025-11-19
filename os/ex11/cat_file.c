#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void append_from_to(int src_fd, int out_fd) {
	char buff[128];
	ssize_t count;

	while (count = read(src_fd, buff, 128)) {
		write(out_fd, buff, count);
	}
}

int main(int argc, char* args[]) {
	int fd = open(args[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0600);

	if (fd == -1) {
		perror("tee.c:1");
		return -1;
	}

	if (argc < 3) {
		append_from_to(0, fd);
	}
	
	unsigned i;

	for (i = 1; i < argc - 1; ++i) {
		int read_fd = open(args[i], O_RDONLY);

		if (read_fd == -1) {
			perror("cat_file.c:2");
			continue;
		}

		append_from_to(read_fd, fd);
		close(read_fd);
	}

	close(fd);
	return 0;
} 
