#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_START_ELEVATOR 335
#define __NR_ISSUE_REQUEST 336
#define __NR_STOP_ELEVATOR 337

int start_elevator() {
	return syscall(335);
}

int issue_request(int type, int start, int dest) {
	return syscall(336, type, start, dest);
}

int stop_elevator() {
	return syscall(337);
}

#endif
