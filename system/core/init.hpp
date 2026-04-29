// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "ln/algo/CompileTimeDAG.hpp"
#include "ln/logger/logger.h"

#include <string_view>

namespace init {

LOG_MODULE(init, ln::logger::Level::notset);

template <typename ID> using Dep = ln::algo::CompileTimeDAG::Edge<ID>;

template <typename ID> struct Item {
    ID key;
    std::string_view name;
    bool (*init_fn)();
};

template <typename T> static void init(T &ordered_initializers) {
    LOG_INFO("initializing {} items", ordered_initializers.size());
    size_t index = 0;
    for (auto &ordered_initializer : ordered_initializers) {
        const auto result = ordered_initializer.init_fn();
        LOG(result ? ln::logger::Level::info : ln::logger::Level::critical,
            "[{}/{}] {}: {}", index + 1, ordered_initializers.size(),
            ordered_initializer.name, result ? "OK" : "FAIL");
        if (!result) {
            LN_PANIC();
        }
        ++index;
    }
}

} // namespace init
