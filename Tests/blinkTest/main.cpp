#include <cstdint>

#include "FreeRTOS.h"
#include "thread.hpp"
#include "timer.hpp"
#include "ticks.hpp"

#include "system.hpp"
#include "peripherals.hpp"
#include "log.hpp"

int main() 
{
    FibSys::start();
}

#include "peripherals.hpp"
#include "../System/Streams/i2sAudioStreamer.hpp"
#include <limits>

extern "C" {
#include "arm_math.h"
}

float mod = 0.0f; 
float frequecy = 1000; // [Hz]
float time = 0.0f; // [sec]
constexpr float samplingRate = 44100.0f; // [1/sec]
constexpr float dt = 1.0f/samplingRate; // [sec]


class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(std::uint16_t stackDepth) : 
        Thread("blinkTestApp", 0x200, FibSys::getAppMaxPriority())
    {
        if(!Start())
        {
            FibSys::panic();
        }
    };
private:
    virtual void Run() override 
    {
        Gpio &onBoardLed = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin13);
        Gpio &rotaryButton = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin14);
        
        onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);
        rotaryButton.init(Gpio::Mode::input, Gpio::Pull::up);

        static I2sStreamer i2s2streamer(Periph::getI2s2(), "i2s2streamer", 0x500, FibSys::getAudioPriority(), [](StereoAudioStreamInterface::Buffer * pStereoAudioBuffer){

            constexpr std::uint16_t u16max = std::numeric_limits<std::uint16_t>::max();
            constexpr float u16maxf = u16max;

            if(pStereoAudioBuffer != nullptr)
            {
                for (auto &stereoSample : *pStereoAudioBuffer)
                {


                    static bool up = false;
                    // if (up) 
                    // {
                    //     time += dt*(10000.0f + modulator);
                    //     if(time > u16maxFloat)
                    //     {
                    //         time = u16maxFloat;
                    //         up = false;
                    //     }
                    // }
                    // else
                    // {
                    //     time -= dt*(10000.0f + modulator);
                    //     if(time < 0.00f)
                    //     {
                    //         time = 0.00f;
                    //         up = true;
                    //     }
                    // }

                    // static std::int16_t a = 0;
                    // a-=64;
                    // stereoSample.left = *(std::uint16_t*)(&a);


                    time += dt;

                    stereoSample.left = 1 + static_cast<std::uint16_t>((arm_sin_f32(2.0f * PI * time * frequecy * arm_sin_f32(2.0f * PI * frequecy/100 * time)/100) / 2.0f + 1.0f) * (u16maxf));

                    if(time > 10000.0f/frequecy)
                        time = -10000.0f/frequecy;

                    stereoSample.right = stereoSample.left;
                }
            }
        });
        
        Delay(cpp_freertos::Ticks::MsToTicks(10));
        i2s2streamer.start();

        while (true)
        {
            // rotaryButton.read() ? Log::colorEnable() : Log::colorDisable(); // todo config over uart

            onBoardLed.write(Gpio::PinState::low);
            Delay(cpp_freertos::Ticks::MsToTicks(500));
            //i2s2streamer.start();
            //Log::info("mainTask", "blink %d !", i++);
            onBoardLed.write(Gpio::PinState::high);
            Delay(cpp_freertos::Ticks::MsToTicks(500));
            //i2s2streamer.stop();
            
            char buffer[0x200];
            vTaskList(buffer);
            Log::info("mainTask", "\r\nTask            State   Prio    Stack   Num\r\n%s", buffer);
            vTaskGetRunTimeStats(buffer);
            Log::info("mainTask", "\r\n%s", buffer);

            // for (int y = 0; y < i; y++)
            // {
            //     Log::trace("mainTask", "%.*s", y, "=================================================================================");
            // }
            // i++;
        }
    }
};

BlinkTestApp blinkTestApp(0x200);
