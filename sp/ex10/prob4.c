#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>

#define MQ_NAME "/00503"

int cleanup(mqd_t mqd) {
	mq_close(mqd);

	if (mq_unlink(MQ_NAME) < 0) {
		perror("mq_unlink");
		return -1;
	}

	return 0;
}

void handle_sigint(int sig) {
	// Just to interrupt read
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file name given!\n");
		return -1;
	}

	mqd_t mqd = mq_open(MQ_NAME, O_RDWR | O_CREAT | O_EXCL, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}
	
	int child = fork();

	if (child < 0) {
		perror("fork");
		cleanup(mqd);
		return -1;
	}

	if (child == 0) {
		int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd < 0) {
			perror("open");
			mq_close(mqd);
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
			mq_close(mqd);
			return -1;
		}

		close(fd);

		if (mq_close(mqd) < 0) {
			perror("mq_close");
			return -1;
		}

		return 0;
	}
	
	struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

	char msg[8192];
	ssize_t cnt;
	while ((cnt = read(0, msg, 8192)) >= 0) {
		if (mq_send(mqd, msg, cnt, 0) < 0) {
			perror("mq_send");
			cleanup(mqd);
			return -1;
		}

		if (cnt == 0) {
			break;
		}
	}

	if (cnt < 0 && errno == EINTR && mq_send(mqd, NULL, 0, 0) < 0) {
		perror("mq_send");
		cleanup(mqd);
		return -1;
	}
	
	return cleanup(mqd);
}
