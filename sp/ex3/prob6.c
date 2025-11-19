#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>

int main(int argc, char* argv[]) {
	if (mkdir("testdir", 0777) < 0 || symlink("testdir/asdf", "testdir/asdf2") < 0 || rmdir("testdir") < 0) {
		perror("prob6.c");
		return -1;
	}

	return 0;
}
