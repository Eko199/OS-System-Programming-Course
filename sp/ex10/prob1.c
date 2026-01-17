#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

#define MQ_NAME "/00503"

int cleanup(mqd_t mqd) {
	mq_close(mqd);

	if (mq_unlink(MQ_NAME) < 0) {
		perror("mq_unlink");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	mqd_t mqd = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}

	struct mq_attr attr;
	if (mq_getattr(mqd, &attr) < 0) {
		perror("mq_getattr");
		cleanup(mqd);
		return -1;
	}

	printf("Flags: %ld\n", attr.mq_flags);
	printf("Max messages: %ld\n", attr.mq_maxmsg);
	printf("Max message size: %ld\n", attr.mq_msgsize);
	printf("Current messages: %ld\n", attr.mq_curmsgs);

	return cleanup(mqd);
}
