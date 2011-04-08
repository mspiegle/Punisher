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

	int16_t flags = 0;
	flags |= EVFILT_READ;

	printf("flags |= EVFILT_READ: [%d]\n", flags);

	printf("flags & EVFILT_READ = [%d]\n", flags & EVFILT_READ);
	if ((flags & EVFILT_READ) == EVFILT_READ) {
		printf("flags & EVFILT_READ: true\n");
	}

	printf("flags & EVFILT_WRITE = [%d]\n", flags & EVFILT_WRITE);
	if ((flags & EVFILT_WRITE) == EVFILT_WRITE) {
		printf("flags & EVFILT_WRITE: true\n");
	}

	return 0;
}
