#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NAME "/sem00503"

int cleanup(sem_t* s, int child) {
	sem_close(s);

	if (child > 0 && sem_unlink(NAME) < 0) {
		wait(NULL);
		perror("sem_unlink");
		return -1;
	}

	return 0;
}


int main(int argc, char* argv[]) {
	sem_t* s = sem_open(NAME, O_CREAT, 0600, 1);

	if (s == NULL) {
		perror("sem_open");
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		cleanup(s, 1);
		return -1;
	}

	int fd = open("test", O_WRONLY | O_CREAT, 0644);
	int i;
	
	for (i = 0; i < 3; ++ i) {
		if (child > 0) {
			sleep(1);
		}

		if (sem_wait(s) < 0) {
			perror("sem_wait");
			cleanup(s, child);
			return -1;
		}
	
		lseek(fd, 0, SEEK_END);
		sleep(2);
	
		char* buff = (child == 0 ? "parent\n" : "child \n");
		write(fd, buff, 7);
		
		if (sem_post(s) < 0) {
			perror("sem_post");
			cleanup(s, child);
			return -1;
		}

		sleep(1);
	}

	return cleanup(s, child);
}
