/*
 * Copyright (c) 2025 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "ln/lua/VM.hpp"
#include "ln/shell/CLI.hpp"

#include "FreeRTOS/Task.hpp"

#include <cstdint>

class FibSys {
public:
    // entry point of fibration system, should be called in main
    static void boot();

    static ln::lua::VM &getLuaVmInstance() {
        static ln::lua::VM lua_vm;
        return lua_vm;
    }

    static ln::shell::CLI &getCliInstance() {
        static std::array<char, 512> input_buf;
        static std::array<char, 512> history_buf;
        static ln::shell::CLI instance{input_buf, history_buf};
        instance.config.interpreter = &getLuaVmInstance();
        return instance;
    }

    enum Priority : BaseType_t {
        lowest = 0,
        veryLow = 1,
        low = 2,
        medium = 3,
        high = 4,
        veryHigh = 5,
        highest = 6,

        // configuration
        appLow = lowest,
        appMedium = veryLow,
        appHigh = low,
        sysLow = medium,
        sysMedium = high,
        sysHigh = veryHigh,
        audioStream = sysLow,

        _enumSize
    };

    static_assert(Priority::_enumSize <= configMAX_PRIORITIES, "invalid priority configration!");

private:
    FibSys() = default;
    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;

    void startup();
    bool init_logger();
    bool init_cli_service();

    static constexpr size_t task_stack_size = 1 * 1024;
    struct Task : public FreeRTOS::StaticTask<task_stack_size> {
        Task(FibSys &owner)
            : FreeRTOS::StaticTask<task_stack_size>(FibSys::Priority::highest, "FibSys"), owner(owner) {}

    private:
        virtual void taskFunction() final;
        FibSys &owner;
    };
    Task task{*this};

    static constexpr size_t cli_svc_task_stack_size = 1024;
    struct CliSvcTask : public FreeRTOS::StaticTask<cli_svc_task_stack_size> {
        CliSvcTask(ln::shell::CLI &cli)
            : FreeRTOS::StaticTask<cli_svc_task_stack_size>(FibSys::Priority::low, "CliSvc"), cli(cli) {}

    private:
        virtual void taskFunction() final {
            while (true) {
                this->cli.routine();
            }
        }
        ln::shell::CLI &cli;
    };
    CliSvcTask cliSvcTask{getCliInstance()};
};
