#pragma once

#include "peripherals/resources.hpp"

#include <stdio.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include <array>

/* TODO: Expose/express Periph::getUart2Stream() as FILE to increase usage
 flexibility with features like ln::shell, ln::logger. It could also be cleanly
 tied to stdin/stdout. Currently it is hardcoded in StdStream which is not ideal.
*/

struct StdStream {
    static StdStream &getInstance() {
        static StdStream instance;
        return instance;
    }

    bool init() {
        setvbuf(stdin, nullptr, _IONBF, 0);
        setvbuf(stdout, this->stdout_buf.data(), _IOLBF, this->stdout_buf.size());
        return true;
    }

    int read(int fd, char *ptr, int len) {
        if (fd == STDIN_FILENO) {
            // TODO: simple getc call invokes read with len=1024 (likely tied
            // to BUFSIZ). Currently we disregard len and read one byte only for
            // simplicity. This could potentially pose performance issues, so
            // should be revisited later.
            if (len) {
                if (!Periph::getUart2Stream().pull(reinterpret_cast<std::uint8_t &>(ptr[0]), portMAX_DELAY)) {
                    return 0;
                }
            }
            return 1;
        }
        errno = EBADF;
        return -1;
    }

    int write(int fd, char *ptr, int len) {
        if ((fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
            errno = EBADF;
            return -1;
        }
        // TODO: why not txDma()?
        if (Periph::getUart2().tx(reinterpret_cast<const std::uint8_t *>(ptr), len, len)) {
            return len;
        }
        return -1;
    }

private:
    std::array<char, 64> stdout_buf;
};
