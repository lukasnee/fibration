
#include "shell.hpp"
#include "system.hpp"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

int shellCmdEcho(Shell &shell, int argc, char *argv[])
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            shell.printf("%s ", argv[i]);
        }
        shell.printc('\n');
        return 0;
    }
    return -1;
}

int shellCmdClear(Shell &shell, int argc, char *argv[])
{
    std::size_t i = 0x20;
    while (i--)
    {
        shell.printc('\n');
    }
    return 1;
}

int shellCmdHelp(Shell &shell, int argc, char *argv[])
{
    static int i = 0;
    shell.printf("Hello World! %d\n", i++);
    return -2;
}

int shellCmdStatus(Shell &shell, int argc, char *argv[])
{
    char szRunTimeStats[10 * 40];
    vTaskGetRunTimeStats(szRunTimeStats);

    shell.printf(/*"initialFreeHeapSize:%lu\n\r"
                 "       freeHeapSize:%lu\n\r"
                 "    minFreeHeapSize:%lu\n\r"*/
                 "uptime: %lu s\n\r"
                 "RunTimeStatsCntVal:%lu\n\r"
                 "Task\t\ttime,.1ms\ttime,%%\n\r%s",
                 /*initialFreeHeapSize,
                 freeHeapSize,
                 minFreeHeapSize,*/
                 xTaskGetTickCount() / configTICK_RATE_HZ,
                 uint32GetRunTimeCounterValue(),
                 szRunTimeStats);
    return 0;
}

int shellPanic(Shell &shell, int argc, char *argv[])
{
    FibSys::panic();
    // will never be reached
    return 0;
}

#include "peripherals.hpp"
#include "../System/Streams/i2sAudioStreamer.hpp"
#include <limits>

extern "C" {
#include "arm_math.h"
}

// float modulator = 0.0f; 
// float frequecy = 1000; // [Hz]
// float time = 0.0; // [sec]
// constexpr float samplingRate = 44100.0f; // [1/sec]
// constexpr float dt = 1.0f/samplingRate; // [sec]

int shellStereo(Shell &shell, int argc, char *argv[])
{

    // if(false == Periph::getI2s2().txCircular(sawWave().data(), sawWave().size()))
    //     return;

    // static I2sStreamer i2s2streamer(Periph::getI2s2(), "i2s2streamer", 0x200, FibSys::getAudioPriority(), [](StereoAudioStreamInterface::Buffer * pStereoAudioBuffer){

    //     constexpr std::uint16_t u16max = std::numeric_limits<std::uint16_t>::max();

    //     if(pStereoAudioBuffer != nullptr)
    //     {
    //         for (auto &stereoSample : *pStereoAudioBuffer)
    //         {
    //             modulator = modulator += 1.0f;
    //             if(modulator > 100.0f)
    //             {
    //                 modulator = -100.0f;
    //             }

    //             //time += dt;
    //             time += dt*(1000 + modulator);
    //             if (time > 1.0f) 
    //             {
    //                 time = 0.0f;
    //             }
    //             stereoSample.first = static_cast<std::uint16_t>(time * u16max);
    //             //stereoSample.first = static_cast<std::uint16_t>((sinf(2.0f * PI * frequecy * time) / 3.0f + 1.0f) * u16max);
    //             stereoSample.second = stereoSample.first;
    //         }
    //     }
    // });
    

    
    // if(*argv[0] == '1')
    // {
    //     i2s2streamer.start();
    // }
    // else if(*argv[0] == '0')
    // {
    //     i2s2streamer.stop();
    // }
    // else {
    //     return -1;
    // }

    return 0;
}

const std::array<Shell::Command, 6> Shell::shellCmds{{
    {"echo", "echos typed content", shellCmdEcho},
    {"clear", "clear screen", shellCmdClear},
    {"help", "Lists all commands", shellCmdHelp},
    {"status", "show system status", shellCmdStatus},
    {"panic", "cause system panic", shellPanic},
    {"stereo", "start i2s2 <0/1>", shellStereo},
}};

// #include <array>
// #include <string_view>
// #include <stdexcept>
// #include <algorithm>

// template <typename Key, typename Value, std::size_t Size>
// struct Map {
//   std::array<std::pair<Key, Value>, Size> data;

//   [[nodiscard]] constexpr Value at(const Key &key) const {
//     const auto itr =
//         std::find_if(begin(data), end(data),
//                      [&key](const auto &v) { return v.first == key; });
//     if (itr != end(data)) {
//       return itr->second;
//     } else {
//       throw std::range_error("Not Found");
//     }
//   }

// };
