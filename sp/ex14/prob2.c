#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <mqueue.h>

void cleanupc() {
	if (mq_unlink("/mq00503c") < 0) {
		perror("mq_unlink");
	}
}

void cleanupp() {
	if (mq_unlink("/mq00503p") < 0) {
		perror("mq_unlink");
	}
}

int main(int argc, char* argv[]) {
	mqd_t mq_tochild = mq_open("/mq00503c", O_CREAT | O_RDWR, 0644, NULL);

	if (mq_tochild < 0) {
		perror("mq_open");
		return -1;
	}

	mqd_t mq_toparent = mq_open("/mq00503p", O_CREAT | O_RDWR, 0644, NULL);	

	if (mq_toparent < 0) {
		perror("mq_open");
		mq_close(mq_tochild);
		cleanupc();
		return -1;
	}

	pid_t child = fork();

	if (child < 0) {
		perror("fork");
		mq_close(mq_tochild);
		mq_close(mq_toparent);
		cleanupc();
		cleanupp();
		return -1;
	}

	mqd_t sender, receiver;

	if (child > 0) {
		atexit(cleanupc);
		atexit(cleanupp);

		sender = mq_tochild;
		receiver = mq_toparent;
		
		sleep(2);
	} else {
		sender = mq_toparent;
		receiver = mq_tochild;
		
		if (mq_receive(receiver, NULL, 8192, NULL) < 0) {
			perror("mq_receive");
			mq_close(receiver);
			mq_close(sender);
			return -1;
		}
	}

	int i;
	for (i = 0; i < 5; ++i) {
		printf("%d\n", child);
		
		if (mq_send(sender, "", 0, 0) < 0) {
			perror("mq_send");
			mq_close(sender);
			mq_close(receiver);
			return -1;
		}

		if (mq_receive(receiver, NULL, 8192, NULL) < 0) {
			perror("mq_receive");
			mq_close(receiver);
			mq_close(sender);
			return -1;
		}
	}

	if (child > 0) {
		mq_send(sender, "", 0, 0);
		wait(NULL);
	}

	return 0;
}
