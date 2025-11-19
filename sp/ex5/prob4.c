#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int fdr = open("test4_r", O_RDONLY);

	if (fdr < 0) {
		perror("prob4.c");
		return -1;
	}

	int fdw = open("test4_wr", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fdw < 0) {
		close(fdr);
		perror("prob4.c");
		return -1;	
	}

	int child = fork();

	if (child < 0) {
		close(fdr);
		close(fdw);
		perror("prob4.c");
		return -1;
	}

	int i;
	for (i = 0; i < 5; ++i) {
		if (child == 0) {
			write(fdw, "child\n", 6);
			sleep(2);
		} else {
			char buf[5];
			int c = read(fdr, buf, 5);
			write(fdw, buf, c);
			sleep(1);
		}
	}

	close(fdr);
	close(fdw);
	return 0;
}
