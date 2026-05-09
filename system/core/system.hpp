// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "ln/lua/VM.hpp"
#include "ln/shell/CLI.hpp"
#include "streams/i2sStream.hpp"

#include "FreeRTOS/Task.hpp"

#include "resources.hpp"

class FibSys {
public:
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

    static_assert(Priority::_enumSize <= configMAX_PRIORITIES,
                  "invalid priority configration!");

    static FibSys &get_instance() {
        static FibSys instance;
        return instance;
    }

    static ln::lua::VM &get_lua_vm_instance() {
        static ln::lua::VM instance;
        return instance;
    }

    static ln::shell::CLI &get_cli_instance() {
        static std::array<char, 512> input_buf;
        static std::array<char, 512> history_buf;
        static ln::shell::CLI instance{input_buf, history_buf};
        instance.config.interpreter = &get_lua_vm_instance();
        return instance;
    }

    static I2sStream &get_i2s2_stream() {
        static I2sStream::Buffer i2s2StreamBuffer;
        static I2sStream instance(Periph::getI2s2(), "i2s2stream",
                                  4 * 1024 / sizeof(StackType_t),
                                  Priority::audioStream, i2s2StreamBuffer);
        return instance;
    }

    /**
     * @brief Launch the fibration system. This should be called in bare-metal
     * main() function, and it will never return.
     */
    static void launch();

private:
    FibSys() = default;
    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;

    void startup();

    static constexpr size_t task_stack_size = 1 * 1024;
    struct Task : public FreeRTOS::StaticTask<task_stack_size> {
        Task(FibSys &owner)
            : FreeRTOS::StaticTask<task_stack_size>(FibSys::Priority::highest,
                                                    "FibSys"),
              owner(owner) {}

    private:
        virtual void taskFunction() final;
        FibSys &owner;
    };
    Task task{*this};

    static constexpr size_t cli_svc_task_stack_size = 1024;
    struct CliSvcTask : public FreeRTOS::StaticTask<cli_svc_task_stack_size> {
        CliSvcTask(ln::shell::CLI &cli)
            : FreeRTOS::StaticTask<cli_svc_task_stack_size>(
                  FibSys::Priority::low, "CliSvc"),
              cli(cli) {}

        void start() { this->notifyGive(); }

    private:
        virtual void taskFunction() final {
            this->notifyTake();
            while (true) {
                this->cli.routine();
            }
        }
        ln::shell::CLI &cli;
    };
    CliSvcTask cli_svc_task{get_cli_instance()};
};
