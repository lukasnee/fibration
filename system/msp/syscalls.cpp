#include "StdStream.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include <cstdarg>
#include <cstdio>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

// #undef errno
extern int errno;

register char *stack_ptr asm("sp");

char *__env[1] = {0};
char **environ = __env;

extern "C" int _gettimeofday(struct timeval *tp, struct timezone *tzp) {
    if (tzp) {
        tzp->tz_minuteswest = 0;
        tzp->tz_dsttime = 0;
    }

    return 0;
}

/* Functions */
extern "C" void initialise_monitor_handles() {}

extern "C" int _getpid(void) { return 1; }

extern "C" int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

extern "C" void _exit(int status) {
    _kill(status, -1);
    while (1) {
    } /* Make sure we hang here */
}

extern "C" int _read(int fd, char *ptr, int len) { return StdStream::getInstance().read(fd, ptr, len); }

extern "C" int _write(int fd, char *ptr, int len) { return StdStream::getInstance().write(fd, ptr, len); }

extern "C" int _close(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

extern "C" int _fstat(int fd, struct stat *st) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

extern "C" int _isatty(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

extern "C" int _lseek(int fd, int ptr, int dir) {
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

extern "C" int _open(char *path, int flags, ...) {
    (void)path;
    (void)flags;
    /* Pretend like we always fail */
    return -1;
}

extern "C" int _wait(int *status) {
    (void)status;
    errno = ECHILD;
    return -1;
}

extern "C" int _unlink(char *name) {
    (void)name;
    errno = ENOENT;
    return -1;
}

extern "C" int _times(struct tms *buf) {
    (void)buf;
    return -1;
}

extern "C" int _stat(char *file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

extern "C" int _link(const char *oldpath, const char *newpath) {
    (void)oldpath;
    (void)newpath;
    errno = EMLINK;
    return -1;
}

extern "C" int _fork(void) {
    errno = EAGAIN;
    return -1;
}

extern "C" int _execve(char *name, char **argv, char **env) {
    (void)name;
    (void)argv;
    (void)env;
    errno = ENOMEM;
    return -1;
}
