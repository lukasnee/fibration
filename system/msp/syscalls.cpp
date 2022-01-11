#include "FreeRTOS.h"
#include "peripherals/resources.hpp"
#include "ioDataIF.hpp"
#include "task.h"

#include "stm32f3xx_hal.h"

#include <cstdarg>
#include <cstdio>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include <time.h>

struct SysCalls
{
    static SysCalls &getInstance()
    {
        static SysCalls instance;
        return instance;
    }

    int _read(int fd, char *ptr, int len)
    {
        if (fd == STDIN_FILENO)
        {
            if (Periph::getUart2().rx(reinterpret_cast<std::uint8_t *>(ptr), len, len))
            {
                return 1;
            }
            else
            {
                return EIO;
            }
        }
        errno = EBADF;
        return -1;
    }

    int _write(int fd, char *ptr, int len)
    {
        if ((fd != STDOUT_FILENO) && (fd != STDERR_FILENO))
        {
            errno = EBADF;
            return -1;
        }

        if (Periph::getUart2().tx(reinterpret_cast<const std::uint8_t *>(ptr), len, len))
        {
            return len;
        }
        return -1;
    }

private:
    SysCalls()
    {
        setvbuf(stdout, this->buf.data(), _IOLBF, this->buf.size());
    }

    std::array<char, 64> buf;
};

//#undef errno
extern int errno;

register char *stack_ptr asm("sp");

char *__env[1] = {0};
char **environ = __env;

/* Functions */
extern "C" void initialise_monitor_handles()
{
}

extern "C" int _getpid(void)
{
    return 1;
}

extern "C" int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

extern "C" void _exit(int status)
{
    _kill(status, -1);
    while (1)
    {
    } /* Make sure we hang here */
}

extern "C" int _read(int fd, char *ptr, int len)
{
    return SysCalls::getInstance()._read(fd, ptr, len);
}

extern "C" int _write(int fd, char *ptr, int len)
{
    return SysCalls::getInstance()._write(fd, ptr, len);
}

extern "C" int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

extern "C" int _fstat(int fd, struct stat *st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

extern "C" int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

extern "C" int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

extern "C" int _open(char *path, int flags, ...)
{
    (void)path;
    (void)flags;
    /* Pretend like we always fail */
    return -1;
}

extern "C" int _wait(int *status)
{
    (void)status;
    errno = ECHILD;
    return -1;
}

extern "C" int _unlink(char *name)
{
    (void)name;
    errno = ENOENT;
    return -1;
}

extern "C" int _times(struct tms *buf)
{
    (void)buf;
    return -1;
}

extern "C" int _stat(char *file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

extern "C" int _link(const char *oldpath, const char *newpath)
{
    (void)oldpath;
    (void)newpath;
    errno = EMLINK;
    return -1;
}

extern "C" int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

extern "C" int _execve(char *name, char **argv, char **env)
{
    (void)name;
    (void)argv;
    (void)env;
    errno = ENOMEM;
    return -1;
}
