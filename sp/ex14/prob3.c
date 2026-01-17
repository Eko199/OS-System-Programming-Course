#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>

void cleanup_shm() {
	if (shm_unlink("/kn00503shm") < 0) {
		perror("shm_unlink");
	}
}

void cleanup_sem() {
	if (sem_unlink("/kn00503s") < 0) {
		perror("sem_unlink");
	}
}

int main(int argc, char* argv[]) {
	int shared = shm_open("/kn00503shm", O_CREAT | O_RDWR | O_TRUNC, 0644);

	if (shared < 0) {
		perror("shm_open");
		return -1;
	}

	if (ftruncate(shared, sizeof(int)) < 0) {
		perror("ftruncate");
		close(shared);
		cleanup_shm();
		return -1;
	}

	int* c = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);
	
	if (c == MAP_FAILED) {
		perror("mmap");
		close(shared);
		cleanup_shm();
		return -1;
	}

	sem_t* s = sem_open("/kn00503s", O_CREAT, 0644, 1);

	if (s == SEM_FAILED) {
		perror("sem_open");
		
		munmap(c, sizeof(int));
		close(shared);
		cleanup_shm();
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		
		munmap(c, sizeof(int));
		close(shared);
		sem_close(s);
		
		cleanup_shm();
		cleanup_sem();
		return -1;
	}

	if (child > 0) {
		atexit(cleanup_shm);
		atexit(cleanup_sem);
	}

	int i;
	for (i = 0; i < 5000000; ++i) {
		if (sem_wait(s) < 0) {
			perror("sem_wait");

			munmap(c, sizeof(int));
			close(shared);
			sem_close(s);
			return -1;
		}

		*c = child > 0 ? *c + 1 : *c - 1;
		
		if (sem_post(s) < 0) {
			perror("sem_post");

			munmap(c, sizeof(int));
			close(shared);
			sem_close(s);
			return -1;
		}
	}

	if (sem_wait(s) < 0) {
		perror("sem_wait");

		munmap(c, sizeof(int));
		close(shared);
		sem_close(s);
		return -1;
	}

	printf("%d\n", *c);
		
	if (sem_post(s) < 0) {
		perror("sem_post");

		munmap(c, sizeof(int));
		close(shared);
		sem_close(s);
		return -1;
	}

	munmap(c, sizeof(int));
	close(shared);
	sem_close(s);

	if (child > 0) {
		wait(NULL);
	}

	return 0;
}
