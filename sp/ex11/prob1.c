#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

#define SHARED "/shm00503"

int main(int argc, char* argv[]) {
	int fd = shm_open(SHARED, O_RDWR | O_CREAT, 0644);

	if (fd < 0) {
		perror("shm_open");
		return -1;
	}

	if (ftruncate(fd, 13) < 0) {
		perror("ftruncate");
		close(fd);

		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
		}

		return -1;
	}
	
	char* msg = mmap(NULL, 13, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (msg == MAP_FAILED) {
		perror("mmap");
		close(fd);

		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
		}

		return -1;
	}
	
	strncpy(msg, "Hello there!\n", 13);
	write(1, msg, 13);

	if (munmap(msg, 13) < 0) {
		perror("munmap");
		close(fd);
		
		if (shm_unlink(SHARED) < 0) {
			perror("unlink");
		}

		return -1;
	}

	close(fd);
	if (shm_unlink(SHARED) < 0) {
		perror("unlink");
		return -1;
	}

	return 0;
}
