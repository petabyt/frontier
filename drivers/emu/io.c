#include <stdio.h>
#include <stdarg.h>

#include <io.h>

void _exit(int status) {
	sys_exit();
}

int _kill(int pid, int sig) {
	return -1;
}

int _fstat(FILE *fd, void *stat) {
	return 0;
}

int _open(const char *filename, uint32_t flags, uint32_t mode) {
	return -1;
}

int _getpid() {
	return -1;
}

int _write(FILE *fd, void *buf, int bytes) {
	return -1;
}

int _read(FILE *fd, void *buf, int bytes) {
	return -1;
}

int _close(FILE *fd) {
	return -1;
}

int _lseek(FILE *fd, int offset, uint32_t whence) {
	return -1;
}

int _isatty(FILE *fd) {
	return 1;
}

int _gettimeofday() {
	return 0;
}
