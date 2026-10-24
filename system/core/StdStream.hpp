// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "peripherals/resources.hpp"

#include <stdio.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include <array>

/* TODO: Expose/express Periph::getUart2Stream() as FILE to increase usage
 flexibility with features like ln::shell, ln::logger. It could also be cleanly
 tied to stdin/stdout. Currently it is hardcoded in StdStream which is not
 ideal.
*/

struct StdStream {
    static StdStream &getInstance() {
        static StdStream instance;
        return instance;
    }

    bool init() {
        setvbuf(stdin, nullptr, _IONBF, 0);
        setvbuf(stdout, this->stdout_buf.data(), _IOLBF,
                this->stdout_buf.size());
        return true;
    }

    int read(int fd, char *ptr, int len) {
        if (fd == STDIN_FILENO) {
            if (len <= 0) {
                return len;
            }
            // TODO: simple getc call invokes read with len=1024 (likely tied to
            // BUFSIZ). Currently we disregard len and read one byte only for
            // responsiveness. This could potentially pose performance issues.
            // It would likely be best to read min(available, len) if available
            // > 0, else wait for data.
            if (!Periph::getUart2().read(reinterpret_cast<std::uint8_t *>(ptr),
                                         1)) {
                return 0;
            }
            return 1;
        }
        errno = EBADF;
        return -1;
    }

    int write(int fd, char *ptr, int len) {
        if (fd == STDERR_FILENO || fd == STDOUT_FILENO) {
            if (Periph::getUart2().write(reinterpret_cast<std::uint8_t *>(ptr),
                                         len)) {
                return len;
            }
            return -1;
        }
        errno = EBADF;
        return -1;
    }

private:
    std::array<char, 64> stdout_buf;
};
