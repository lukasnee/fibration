// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include <span>

#include <cstdio>

struct StdStream {
    static StdStream &get_instance();

    struct Cfg {

        /**
         * @brief The std::setvbuf mode (stdio.h). Either `_IONBF` (unbuffered),
         * `_IOLBF` (line buffered), or `_IOFBF` (fully buffered).
         */
        using BufferMode = int;

        struct {
            BufferMode mode{_IONBF};

            /**
             * @note Buffer may be provided only for modes other than `_IONBF`.
             */
            std::span<char> buf{};
        } in;

        struct {
            BufferMode mode{_IONBF};

            /**
             * @note Buffer may be provided only for modes other than `_IONBF`.
             */
            std::span<char> buf{};
        } out;

        struct {
            BufferMode mode{_IONBF};

            /**
             * @note Buffer may be provided only for modes other than `_IONBF`.
             */
            std::span<char> buf{};
        } err;
    };

    /**
     * @brief Initialize the standard streams with the given configuration.
     *
     * @param cfg
     */
    bool init(const Cfg &cfg);

    /**
     * @brief System call `read` handler for standard input (stdin).
     *
     * @param fd
     * @param ptr
     * @param len
     * @retval int
     */
    int read(int fd, char *ptr, int len);

    /**
     * @brief System call `write` handler for standard output (stdout) or
     * standard error (stderr).
     *
     * @param fd
     * @param ptr
     * @param len
     * @retval int
     */
    int write(int fd, char *ptr, int len);
};
