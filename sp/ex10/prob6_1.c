#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <signal.h>

#include "mqname.h"

void handle_sigint(int sig) {
	// Just to interrupt read
}

int main(int argc, char* argv[]) {
	mqd_t mqd = mq_open(MQ_NAME, O_WRONLY | O_CREAT, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}

	struct sigaction sa;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	char buf[8192];
	int cnt;

	while ((cnt = read(0, buf, 8192)) > 0) {
		if (mq_send(mqd, buf, cnt, 0) < 0) {
			perror("mq_send");
			mq_close(mqd);
			return -1;
		}
	}

	if (mq_send(mqd, NULL, 0, 0) < 0) {
		perror("mq_send");
		mq_close(mqd);
		return -1;
	}

	mq_close(mqd);
	return 0;
}
