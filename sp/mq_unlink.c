#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

int main(int argc, char* argv[]) {
	if (mq_unlink("/00503") < 0) {
		perror("mq_unlink");
		return -1;
	}

	return 0;
}
