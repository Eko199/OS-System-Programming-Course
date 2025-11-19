#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	char buff[128];
	ssize_t count;

	while (count = read(0, buff, 128)) {
		write(1, buff, count);
	}

	return 0;
}
