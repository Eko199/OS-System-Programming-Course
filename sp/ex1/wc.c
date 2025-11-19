#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int is_eow(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int main(int argc, char* files[]) {
	if (argc < 2) {
		printf("Invalid command!");
	}

	int fd = open(files[1], O_RDONLY);

	if (fd < 0) {
		perror("wc.c");
		return 1;
	}

	char buf;
	int bytes = 0, words = 0, lines = 0;
	int in_word = 0;

	while (read(fd, &buf, 1)) {
		++bytes;
				
		if (is_eow(buf) && in_word) {
			++words;
			in_word = 0;
		} else if (!is_eow(buf)) {
			in_word = 1;
		} 
				

		lines += buf == '\n';
	}

	printf(" %d %d %d %s\n", lines, words, bytes, files[1]);

	close(fd);
	return 0;
}
