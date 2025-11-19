#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int fdr = open("test4_r", O_RDONLY);

	if (fdr < 0) {
		perror("prob5.c");
		return -1;
	}

	int fdw = open("test5_wr", O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (fdw < 0) {
		close(fdr);
		perror("prob5.c");
		return -1;	
	}

	int child = fork();

	if (child < 0) {
		close(fdr);
		close(fdw);
		perror("prob5.c");
		return -1;
	}

	int i;

	if (child > 0) {
		while (waitpid(child, NULL, WNOHANG) == 0) {
			write(fdw, "parent\n", 7);
			sleep(2);
		}
	} else {
		for (i = 0; i < 5; ++i) {
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
