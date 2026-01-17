#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NAME "/sem00503"

int cleanup(sem_t* s) {
	sem_close(s);

	if (sem_unlink(NAME) < 0) {
		perror("sem_unlink");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	sem_t* s = sem_open(NAME, O_CREAT, 0600, 0);

	if (s == NULL) {
		perror("sem_open");
		return -1;
	}

	int child = fork();

	if (child < 0) {
		perror("fork");
		cleanup(s);
		return -1;
	}

	if (child == 0) {
		sleep(3);
		printf("First text\n");
		
		if (sem_post(s) < 0) {
			perror("sem_post");
			sem_close(s);
			return -1;
		}
	
		sem_close(s);
		return 0;
	}

	if (sem_wait(s) < 0) {
		perror("sem_wait");
		cleanup(s);
		return -1;
	}

	printf("Second text\n");
	return cleanup(s);
}
