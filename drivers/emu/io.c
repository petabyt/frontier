#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <io.h>

void syscall(int v);

void _exit(int status) {
	sys_exit();
}

int _kill(int pid, int sig) {
	return -1;
}

int _fstat(FILE *fd, void *stat) {
	return 0;
}

int _stat(const char *path, struct stat *buf) {
	((uintptr_t *)SYS_REGS)[0] = (uintptr_t)path;
	syscall(SYS_FILE_SIZE);
	buf->st_size = ((int *)SYS_READ_REGS)[0];
}

int _open(char *pathname, int oflag, int rflag) {
	((uintptr_t *)SYS_REGS)[0] = (uintptr_t)pathname;
	((uintptr_t *)SYS_REGS)[1] = (uintptr_t)"rwb";
	syscall(SYS_FOPEN);
	if (((int *)SYS_READ_REGS)[0] == 0) {
		return NULL;
	} else {
		return (FILE *)100;
	}
}

int _getpid() {
	return -1;
}

int _write(FILE *fd, void *buf, int bytes) {
	((uintptr_t *)SYS_REGS)[0] = (uintptr_t)buf;
	((int *)SYS_REGS)[1] = bytes;
	syscall(SYS_FWRITE);
	return ((int *)SYS_READ_REGS)[0];
}

int _read(FILE *fd, void *buf, int bytes) {
	((uintptr_t *)SYS_REGS)[0] = (uintptr_t)buf;
	((int *)SYS_REGS)[1] = bytes;
	syscall(SYS_FREAD);
	return ((int *)SYS_READ_REGS)[0];
}

int _close(int file) {
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
