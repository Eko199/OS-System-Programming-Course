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
	mqd_t mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}

	char buf[8192];
	int cnt;

	while ((cnt = mq_receive(mqd, buf, 8192, NULL)) > 0) {
		write(1, buf, cnt);
	}

	if (cnt < 0) {
		perror("mq_receive");
		cleanup(mqd);
		return -1;
	}

	return cleanup(mqd);
}
