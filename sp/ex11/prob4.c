#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

#define NAME "/shm00503"

int cleanup(int shm) {
	close(shm);

	if (shm_unlink(SHARED) < 0) {
		perror("shm_unlink");
		return -1;
	}

	return 0;
}

void bubbleSort(int* arr, size_t size) {
	size_t i, j;

	for (i = 0; i < size - 1; ++i) {
		for (j = 0; j < size - i - 1; ++j) {
			if (arr[j] <= arr[j + 1]) {
				continue;
			}

			int tmp = arr[j];
			arr[j] = arr[j + 1];
			arr[j + 1] = tmp;
		}
	}
}

int main(int argc, char* argv[]) {
	srandom(time(NULL));
	int shm = shm_open(SHARED, O_CREAT | O_RDWR | O_TRUNC, 0644);

	if (shm < 0) {
		perror("shm_open");
		return -1;
	}

	if (ftruncate(shm, sizeof(int[20])) < 0) {
		perror("ftruncate");
		cleanup(shm);
		return -1;
	}

	int* arr = mmap(NULL, sizeof(int[20]), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

	if (arr == MAP_FAILED) {
		perror("mmap");
		cleanup(shm);
		return -1;
	}

	size_t i;
	for (i = 0; i < 20; ++i) {
		arr[i] = random() % 100;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("fork");

		if (munmap(arr, sizeof(int[20])) < 0) {
			perror("munmap");
		}

		cleanup(shm);
		return -1;
	}

	if (child == 0) {
		for (i = 0; i < 20; ++i) {
			printf("arr[%ld] = %d\n", i, arr[i]);
		}

		if (munmap(arr, sizeof(int[20])) < 0) {
			perror("munmap");
			close(shm);
			return -1;
		}

		close(shm);
		return 0;
	}

	bubbleSort(arr, 20);

	if (munmap(arr, sizeof(int[20])) < 0) {
		perror("munmap");
		cleanup(shm);
		return -1;
	}

	return cleanup(shm);
}
