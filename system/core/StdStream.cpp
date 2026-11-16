// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "StdStream.hpp"

#include "uart2.hpp"

#include "ln/ln.h"
#include "ln/prof/prof.h"

#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

StdStream &StdStream::get_instance() {
    static StdStream instance;
    return instance;
}

bool StdStream::init(const Cfg &cfg) {

    {
        const auto rc =
            setvbuf(stdin, cfg.in.buf.data(), cfg.in.mode, cfg.in.buf.size());
        if (rc != 0) {
            return false;
        }
    }

    {
        const auto rc = setvbuf(stdout, cfg.out.buf.data(), cfg.out.mode,
                                cfg.out.buf.size());
        if (rc != 0) {
            return false;
        }
    }

    {
        const auto rc = setvbuf(stderr, cfg.err.buf.data(), cfg.err.mode,
                                cfg.err.buf.size());
        if (rc != 0) {
            return false;
        }
    }

    return true;
}

/* TODO: Expose/express Uart2::get_instance() as FILE to increase usage
 flexibility with features like ln::shell, ln::logger. It could also be cleanly
 tied to stdin/stdout. Currently it is hardcoded in StdStream which is not
 ideal.
*/
int StdStream::read(int fd, char *ptr, int len) {
    if (fd == STDIN_FILENO) {
        if (len <= 0) {
            return len;
        }
        const std::size_t n = Uart2::get_instance().read_some(
            reinterpret_cast<std::uint8_t *>(ptr),
            static_cast<std::size_t>(len));
        return static_cast<int>(n); // 0 is EOF to the caller
    }
    errno = EBADF;
    return -1;
}

int StdStream::write(int fd, char *ptr, int len) {
    if (fd == STDERR_FILENO || fd == STDOUT_FILENO) {
        LN_PROF_MSG(ptr, static_cast<std::size_t>(len));
        if (Uart2::get_instance().write(reinterpret_cast<std::uint8_t *>(ptr),
                                        len)) {
            return len;
        }
        return -1;
    }
    errno = EBADF;
    return -1;
}
