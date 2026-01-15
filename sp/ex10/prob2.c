#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

int main(int argc, char* argv[]) {
	mqd_t mqd = mq_open("/00503", O_RDWR | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}
	
	if (mq_send(mqd, "Hello there!\n", 13, 1) < 0) {
		perror("mq_send");
		mq_unlink("/00503");
		return -1;
	}

	char msg[8192];
	ssize_t cnt = mq_receive(mqd, msg, 8192, NULL);

	if (cnt < 0) {
		perror("mq_receive");
		mq_unlink("/00503");
		return -1;
	}

	write(1, msg, cnt);
	
	if (mq_close(mqd) < 0) {
		perror("mq_close");
		mq_unlink("/00503");
		return -1;
	}

	if (mq_unlink("/00503") < 0) {
		perror("mq_unlink");
		return -1;
	}
	
	return 0;
}
