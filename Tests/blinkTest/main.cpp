#include <cstdint>
extern "C"
{
#include "arm_math.h"
#include "FreeRTOS.h"
}
#include "thread.hpp"
#include "timer.hpp"
#include "ticks.hpp"

#include "system.hpp"
#include "peripherals.hpp"
#include "log.hpp"

#include "peripherals.hpp"
#include "../System/Streams/i2sDuplexStream.hpp"
#include <limits>

int main()
{
    FibSys::boot();
}

float randomF = 0.0f;

class Timah : public cpp_freertos::Timer {
public:
    Timah() : Timer("snh",1000, true) {} 
private:   
   void Run() override {
       randomF = static_cast<float>(((2.0f * std::rand()) / RAND_MAX) - 1.0f);
   }
};

Timah timah;

//! Byte swap unsigned int
uint32_t swap_uint32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

static I2sDuplexStream::CircularBuffer i2s2DuplexStreamCircularBufferTx;
static I2sDuplexStream::CircularBuffer i2s2DuplexStreamCircularBufferRx;

static void processTxRxBufferI2s2(const DuplexStereoStreamInterface::Buffer *pStereoAudioRxBuffer, DuplexStereoStreamInterface::Buffer *pStereoAudioTxBuffer)
{
    constexpr std::uint32_t u24max = (1 << 24) - 1;
    constexpr float u24maxf = u24max;
    constexpr float samplingRate = 44100.0f;  // [1/sec]
    constexpr float dt = 1.0f / samplingRate; // [sec]
    static float frequecy = 1000;             // [Hz]
    static float time = 0.0f;                 // [sec]
    
    if (pStereoAudioTxBuffer != nullptr)
    {
        // 1KHz FM modulation to DAC LEFT
        for (auto &stereoSample : *pStereoAudioTxBuffer)
        {
            time += dt;
            auto mod = arm_sin_f32((1.0f + randomF/3.0f) * PI * frequecy / 10000 * time);
            auto fm = arm_sin_f32(2.0f * PI * time * frequecy * mod / 10);
            auto fmSample = swap_uint32(1 + static_cast<std::uint32_t>((fm / 2.0f + 1.0f) * (u24maxf)));

            stereoSample.left = fmSample;

            // stereoSample.right = 0;
            if (time > 10000.0f / frequecy)
                time = -10000.0f / frequecy;
        }

        // dry pass ADC LEFT -> DAC RIGHT
        for (std::size_t i = 0; i < pStereoAudioTxBuffer->size(); i++)
        {
            (*pStereoAudioTxBuffer)[i].right = (*pStereoAudioRxBuffer)[i].left;
        }
    }
}
/*__attribute__((section(".ccmram")))*/
static I2sDuplexStream i2s2DuplexStream(Periph::getI2s2(),
                                "i2s2stream",
                                0x500,
                                FibSys::getAudioPriority(),
                                i2s2DuplexStreamCircularBufferTx,
                                i2s2DuplexStreamCircularBufferRx,
                                processTxRxBufferI2s2);

class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority) : Thread(pcName, usStackDepth, uxPriority)
    {
        if (!Start())
        {
            FibSys::panic();
        }
    };

private:
    char buffer[0x200];

    virtual void Run() override
    {
        Gpio &onBoardLed = Periph::getGpio(Gpio::Port::A, Gpio::Pin::pin5);
        //Gpio &rotaryButton = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin14);

        onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);
        //rotaryButton.init(Gpio::Mode::input, Gpio::Pull::up);

        Delay(cpp_freertos::Ticks::MsToTicks(100));
        i2s2DuplexStream.start();
        timah.Start();
        int i = 0;
        while (true)
        {
            // rotaryButton.read() ? Log::colorEnable() : Log::colorDisable(); // todo config over uart

            onBoardLed.write(Gpio::PinState::low);
            Delay(cpp_freertos::Ticks::MsToTicks(500));
            onBoardLed.write(Gpio::PinState::high);
            Delay(cpp_freertos::Ticks::MsToTicks(500));

            // Log::info("mainTask", "blink %d !", i++);
            // vTaskList(buffer);
            // Log::info("mainTask", "\r\nTask            State   Prio    Stack   Num\r\n%s", buffer);
            // vTaskGetRunTimeStats(buffer);
            // Log::info("mainTask", "\r\n%s", buffer);
        }
    }
};

BlinkTestApp blinkTestApp("App1", 0x500, FibSys::getAppMaxPriority());

// for (int y = 0; y < i; y++)
// {
//     Log::trace("mainTask", "%.*s", y, "=================================================================================");
// }
// i++;
