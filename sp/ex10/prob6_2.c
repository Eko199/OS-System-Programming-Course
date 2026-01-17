#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

#include "mqname.h"

int cleanup(mqd_t mqd) {
	mq_close(mqd);

	if (mq_unlink(MQ_NAME) < 0) {
		perror("mq_unlink");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file name given!\n");
		return -1;
	}
	
	mqd_t mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}

	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd < 0) {
		perror("open");
		cleanup(mqd);
		return -1;
	}

	char buf[8192];
	int cnt;

	while ((cnt = mq_receive(mqd, buf, 8192, NULL)) > 0) {
		write(fd, buf, cnt);
	}

	if (cnt < 0) {
		perror("mq_receive");
		close(fd);
		cleanup(mqd);
		return -1;
	}

	close(fd);
	return cleanup(mqd);
}
