#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file name given!\n");
		return -1;
	}

	mqd_t mqd = mq_open("/00503", O_RDWR | O_CREAT | O_EXCL, 0644, NULL);

	if (mqd < 0) {
		perror("mq_open");
		return -1;
	}
	
	int child = fork();

	if (child < 0) {
		perror("fork");
		mq_close(mqd);
		mq_unlink("/00503");
		return -1;
	}

	if (child == 0) {
		int fd = open(argv[1], O_RDONLY);
		
		if (fd < 0) {
			perror("open");
			mq_send(mqd, NULL, 0, 1);
			mq_close(mqd);
			return -1;
		}

		char buf[8192];
		int cnt;		

		while (cnt = read(fd, buf, 8192)) {
			if (mq_send(mqd, buf, cnt, 1) < 0) {
				perror("mq_send");
				close(fd);
				return -1;
			}
		}

		close(fd);

		if (mq_send(mqd, NULL, 0, 1) < 0) {
			perror("mq_send");
			return -1;
		}

		if (mq_close(mqd)) {
			perror("mq_close");
			return -1;
		}

		return 0;
	}
			
	char msg[8192];
	ssize_t cnt;
	while (cnt = mq_receive(mqd, msg, 8192, NULL)) {
		write(1, msg, cnt);
	}

	if (cnt < 0) {
		perror("mq_receive");
		mq_unlink("/00503");
		return -1;
	}

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
