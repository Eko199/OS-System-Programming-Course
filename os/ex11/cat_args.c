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
	if (argc < 2) {
		append_from_to(0, 1);
	}
	
	unsigned i;

	for (i = 1; i < argc; ++i) {
		int fd = open(args[i], O_RDONLY);

		if (fd == -1) {
			perror("cat_file.c");
			continue;
		}

		append_from_to(fd, 1);
		close(fd);
	}

	return 0;
} 
