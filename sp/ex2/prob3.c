#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	int i;
	struct stat info;

	for (i = 1; i < argc; ++i) {
		lstat(argv[i], &info);
		
		printf("st_dev: %d\n", info.st_dev);
		printf("st_ino: %d\n", info.st_ino);
		printf("st_mode: %d\n", info.st_mode);
		printf("st_nlink: %d\n", info.st_nlink);
		printf("st_uid: %d\n", info.st_uid);
		printf("st_gid: %d\n", info.st_gid);
		printf("st_rdev: %d\n", info.st_rdev);
		printf("st_size: %d\n", info.st_size);
		printf("st_blksize: %d\n", info.st_blksize);
		printf("st_blocks: %d\n", info.st_blocks);
		printf("st_atime: %d\n", info.st_atime);
		printf("st_mtime: %d\n", info.st_mtime);
		printf("st_ctime: %d\n\n", info.st_ctime);
	}

	return 0;
}
