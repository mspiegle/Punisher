#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/event.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

int main() {
	int kfd;
	kfd = kqueue();
	printf("kfd: %d\n", kfd);

	int fd;
	fd = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == fcntl(fd, F_SETFL, O_NONBLOCK)) {
		printf("fcntl() failure\n");
		return 1;
	}
	printf("fd: %d\n", fd);

	struct kevent kev;
	EV_SET(&kev, fd, EVFILT_WRITE, EV_ADD, 0, NULL, NULL);
	if (-1 == kevent(kfd, &kev, 1, NULL, 0, 0)) {
		printf("Error with kevent()\n");
		return 1;
	}

	sleep(1);

	struct kevent event;
	int num_events;
	struct timespec timeout;
	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	num_events = kevent(kfd, NULL, 0, &event, 1, &timeout);
	printf("numevents: %d\n", num_events);

	struct in_addr addr;
	inet_aton("64.34.252.43", &addr);

	struct sockaddr_in target;
	target.sin_family = AF_INET;
	target.sin_port = htons(80);
	target.sin_addr = addr;
	int error;
	if (-1 == connect(fd, (const struct sockaddr*)&target,
	                  sizeof(struct sockaddr_in))) {
		error = errno;
		printf("connect failure [%s]\n", strerror(error));
	}
	
	num_events = kevent(kfd, NULL, 0, &event, 1, &timeout);
	printf("numevents: %d\n", num_events);

	close(fd);
	close(kfd);

	return 0;
}
