#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>

void clean_shm(int shm) {
	close(shm);

	if (shm_unlink("/kn00503shm") < 0) {
		perror("shm_unlink");
	}
}

void clean_sem(sem_t* s, const char* const name) {
	sem_close(s);

	if (sem_unlink(name) < 0) {
		perror("sem_unlink");
	}
}

int main(int argc, char* argv[]) {
	sem_t* t = sem_open("/kn00503semt", O_CREAT, 0644, 1);

	if (t == SEM_FAILED) {
		perror("sem_open");
		return -1;
	}

	sem_t* mutex = sem_open("/kn00503mutex", O_CREAT, 0644, 1);

	if (mutex == SEM_FAILED) {
		perror("sem_open");
		clean_sem(t, "/kn00503semt");
		return -1;
	}

	int shm = shm_open("/kn00503shm", O_CREAT | O_RDWR | O_TRUNC, 0644);

	if (shm < 0) {
		perror("shm_open");
		clean_sem(t, "/kn00503semt");
		clean_sem(mutex, "/kn00503mutex");
		return -1;
	}
	
	if (ftruncate(shm, sizeof(int)) < 0) {
		perror("ftruncate");
		clean_shm(shm);
		clean_sem(t, "/kn00503semt");
		clean_sem(mutex, "/kn00503mutex");
		return -1;
	}	

	sem_t* w = sem_open("/kn00503semw", O_CREAT, 0644, 1);

	if (w == SEM_FAILED) {
		perror("sem_open");
		clean_shm(shm);
		clean_sem(t, "/kn00503semt");
		clean_sem(mutex, "/kn00503mutex");
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		clean_shm(shm);
		clean_sem(t, "/kn00503semt");
		clean_sem(mutex, "/kn00503mutex");
		clean_sem(w, "/kn00503semw");
		return -1;
	}

	if (child > 0) {
		child = fork();

		if (child < 0) {
			perror("fork");
			clean_shm(shm);
			clean_sem(t, "/kn00503semt");
			clean_sem(mutex, "/kn00503mutex");
			clean_sem(w, "/kn00503mutex");
			return -1;
		}
	}

	if (child > 0) {
		int fd = open("test", O_CREAT | O_TRUNC | O_WRONLY, 0644);
		sleep(2);

		unsigned i;
		for (i = 0; i < 10; ++i) {
			if (sem_wait(t) < 0 || sem_wait(w) < 0) {
				perror("sem_wait");
				clean_shm(shm);
				clean_sem(t, "/kn00503semt");
				clean_sem(mutex, "/kn00503mutex");
				clean_sem(w, "/kn00503mutex");
				return -1;
			}
			
			write(fd, "Hello!\n", 7);

			if (sem_post(w) < 0 || sem_post(t) < 0) {
				perror("sem_post");
				clean_shm(shm);
				clean_sem(t, "/kn00503semt");
				clean_sem(mutex, "/kn00503mutex");
				clean_sem(w, "/kn00503mutex");
				return -1;
			}

			sleep(1);
		}

		close(fd);
		wait(NULL);
		wait(NULL);

		clean_shm(shm);
		clean_sem(t, "/kn00503semt");
		clean_sem(mutex, "/kn00503mutex");
		clean_sem(w, "/kn00503semw");
		return 0;
	}

	int* counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

	if (counter == MAP_FAILED) {
		perror("mmap");
		close(shm);
		sem_close(t);
		sem_close(mutex);
		sem_close(w);
		return -1;
	}

	unsigned i;

	for (i = 0; i < 5; ++i) {
		if (sem_wait(t) < 0) {
			perror("sem_wait");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		if (sem_post(t) < 0) {
			perror("sem_post");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		if (sem_wait(mutex) < 0) {
			perror("sem_wait");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		(*counter)++;

		if (*counter == 1 && sem_wait(w) < 0) {
			perror("sem_wait");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		if (sem_post(mutex) < 0) {
			perror("sem_post");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		int fd = open("test", O_RDONLY);

		if (fd >= 0) {
			printf("%d:\n", getpid());
			char buff[256];
			int readCount;

			while (readCount = read(fd, buff, 256)) {
				write(1, buff, readCount);
			}

			close(fd);
		}

		if (sem_wait(mutex) < 0) {
			perror("sem_wait");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}
	
		(*counter)--;

		if (*counter == 0 && sem_post(w) < 0) {
			perror("sem_post");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		if (sem_post(mutex) < 0) {
			perror("sem_post");
			close(shm);
			sem_close(t);
			sem_close(mutex);
			sem_close(w);
			return -1;
		}

		sleep(2);
	}

	if (munmap(counter, sizeof(int)) < 0) {
		perror("munmap");
		close(shm);
		sem_close(t);
		sem_close(mutex);
		sem_close(w);
		return -1;
	}

	close(shm);
	sem_close(t);
	sem_close(mutex);
	sem_close(w);

	return 0;
}
