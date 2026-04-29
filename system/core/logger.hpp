// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "ln/logger/logger.h"

#include "ln/build.hpp"

LOG_EXTERN(system);

namespace logger {

LOG_SCOPE(system);

bool init() {
    auto logger_config = ln::logger::get_instance().get_config();
    constexpr size_t logger_out_buf_size = 512;
    static std::array<char, logger_out_buf_size> logger_out_buf{};
    logger_config.out_buf = logger_out_buf;
    logger_config.eol = "\r\n";
    logger_config.enabled_run_time = true;
    const auto res = ln::logger::get_instance().set_config(logger_config);
    LOG_INFO("starting up {} v{} [{}] {} {}", ln::build::name,
             ln::build::version::str, ln::build::git_hash, ln::build::date,
             ln::build::time);
    return res;
}

} // namespace logger
