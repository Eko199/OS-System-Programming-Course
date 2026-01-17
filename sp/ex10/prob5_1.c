#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

#include "mqname.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file name given!\n");
		return -1;
	}

	mqd_t mqd = mq_open(MQ_NAME, O_WRONLY | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		perror("open");
		mq_close(mqd);
		return -1;
	}

	char buf[8192];
	int cnt;

	while (cnt = read(fd, buf, 8192)) {
		if (mq_send(mqd, buf, cnt, 0) < 0) {
			perror("mq_send");
			close(fd);
			mq_close(mqd);
			return -1;
		}
	}

	close(fd);

	if (mq_send(mqd, NULL, 0, 0) < 0) {
		perror("mq_send");
		mq_close(mqd);
		return -1;
	}

	mq_close(mqd);
	return 0;
}
