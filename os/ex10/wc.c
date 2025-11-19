#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int c = 0, l = 0, w = 0;
	char buf;
	int fd = open(argv[1], O_RDONLY);
	int in_word = 0;

	while (read(fd, &buf, 1)) {
		if (buf == '\n') {
			++l;
		}

		if ((buf == ' ' || buf == '\n' || buf == '	') && in_word) {
			++w;
			in_word = 0;
		} else {
			in_word = 1;
			w += c == 0;
		}

		++c;
	}

	close(fd);
	printf(" %d %d %d %s\n", l, w, c, argv[1]);
	return 0;
}
