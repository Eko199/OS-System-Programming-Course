#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SHARED "/shm00503"

int main(int argc, char* argv[]) {
	int fd = shm_open(SHARED, O_RDONLY, 0644);

	if (fd < 0) {
		perror("shm_open");
		return -1;
	}

	char* msg = mmap(NULL, 13, PROT_READ, MAP_SHARED, fd, 0);

	if (msg == MAP_FAILED) {
		perror("mmap");
		close(fd);
		return -1;
	}
	
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
