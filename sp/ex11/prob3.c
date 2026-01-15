#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SHARED "/shm00503"

int main(int argc, char* argv[]) {
	int fd = shm_open(SHARED, O_RDWR | O_CREAT, 0644);

	if (fd < 0) {
		perror("shm_open");
		return -1;
	}

	if (ftruncate(fd, sizeof(int)) < 0) {
		perror("ftruncate");
		close(fd);

		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
		}

		return -1;
	}
	
	int* cnt = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (cnt == MAP_FAILED) {
		perror("mmap");
		close(fd);

		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
		}

		return -1;
	}
	
	*cnt = 0;
	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		close(fd);

		if (shm_unlink(SHARED) < 0) {
			perror("shm_unlink");
		}

		return -1;
	}

	
	while (*cnt < 100) {
		sleep(child == 0 ? 1 : 2);
		(*cnt)++;
	}

	printf("PID: %d, value: %d\n", getpid(), *cnt);

	if (munmap(cnt, sizeof(int)) < 0) {
		perror("munmap");
		close(fd);
		
		if (child > 0) {
			wait(NULL);

			if (shm_unlink(SHARED) < 0) {
				perror("unlink");
			}
		}

		return -1;
	}

	close(fd);

	if (child > 0) {
		wait(NULL);

		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
			return -1;
		}
	}

	return 0;
}
