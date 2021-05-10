#include "fibration.hpp"

#include <cstdint>

#include <limits>

#include "../System/Streams/i2sDuplexStream.hpp"

float randomF = 0.0f;

class Timah : public cpp_freertos::Timer
{
public:
    Timah() : Timer("snh", 1000, true) {}

private:
    void Run() override
    {
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

class Oscillator
{
public:
    Oscillator(float frequencyInHz = 0.0f, float InitialPhaseInRad = (-1.0f * PI)) : frequencyInHz(frequencyInHz), phaseInRad(InitialPhaseInRad){};

    float evaluateStep(float stepInSeconds)
    {
        this->phaseInRad += (2.0f * PI * this->frequencyInHz * stepInSeconds);

        if (this->phaseInRad > PI)
        {
            this->phaseInRad = this->phaseInRad - (2.0f * PI);
        }

        return (amplitude * arm_sin_f32(this->phaseInRad));
    }
    float getFrequency() { return frequencyInHz; }
    void setFrequency(float frequencyInHz) { this->frequencyInHz = frequencyInHz; }
    void setAmplitude(float amplitude) { this->amplitude = amplitude; }

private:
    float amplitude = 0;
    float frequencyInHz = 0;
    float phaseInRad = 0;

    //const Oscillator(Oscillator &) = delete;
    Oscillator(Oscillator &&) = delete;
};

// constexpr float e = 2.7182818;
// constexpr float e2 = (e*e);

float mainFreqVar, mainAmplVar, modFreqVar, modAmplVar;

static void processTxRxBufferI2s2(const DuplexStereoStreamInterface::Buffer *pStereoAudioRxBuffer, DuplexStereoStreamInterface::Buffer *pStereoAudioTxBuffer)
{
    constexpr std::uint32_t u24max = (1 << 24) - 1;
    constexpr float u24maxf = u24max;
    constexpr float samplingRate = 44100.0f;  // [1/sec]
    constexpr float dt = 1.0f / samplingRate; // [sec]

    static Oscillator main(440.0f);
    static Oscillator mod(10.0f);

    if (pStereoAudioTxBuffer != nullptr)
    {
        Periph::getAdc2().getValue(3, mainFreqVar);
        Periph::getAdc2().getValue(4, mainAmplVar);
        Periph::getAdc2().getValue(5, modFreqVar);
        Periph::getAdc2().getValue(6, modAmplVar);
        main.setAmplitude(mainAmplVar);
        //mod.setFrequency(20000.0f * pow(2.0f, 10.0f * modFreqVar - 10.0f));
        //mod.setFrequency(20000.0f * (pow(2.0f, 0.00863268f + (e2 * modFreqVar) - e2) - 0.00600166f));
        mod.setFrequency(20000.0f * modFreqVar * modFreqVar * modFreqVar * modFreqVar);

        mod.setAmplitude(modAmplVar);
        // 1KHz FM modulation to DAC LEFT
        for (auto &stereoSample : *pStereoAudioTxBuffer)
        {
            main.setFrequency(20000.0f * mainFreqVar * mainFreqVar * mainFreqVar * mainFreqVar * (220.0f * mod.evaluateStep(dt)));
            auto fmSample = swap_uint32(1 + static_cast<std::uint32_t>((main.evaluateStep(dt) / 2.0f + 1.0f) * (u24maxf)));

            stereoSample.left = fmSample;

            // stereoSample.right = 0;
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

        DelayUntil(cpp_freertos::Ticks::MsToTicks(1000));

        Periph::getAdc2().init();

        i2s2DuplexStream.start();
        timah.Start();
        int i = 0;
        while (true)
        {
            // rotaryButton.read() ? Logger::setColoring() : Logger::colorDisable(); // todo config over uart

            onBoardLed.write(Gpio::PinState::low);
            DelayUntil(cpp_freertos::Ticks::MsToTicks(500));
            onBoardLed.write(Gpio::PinState::high);
            DelayUntil(cpp_freertos::Ticks::MsToTicks((500)));

            Logger::log(Logger::Verbosity::mid, Logger::Type::info, "blink %d !\n", i++);
            vTaskList(buffer);
            Logger::log(Logger::Verbosity::mid, Logger::Type::info, "\nTask            State   Prio    Stack   Num\n%s\n", buffer);
            vTaskGetRunTimeStats(buffer);
            Logger::log(Logger::Verbosity::mid, Logger::Type::info, "\n%s\n", buffer);

            Logger::logFast("logFast!\n");
            Logger::logFastFromISR("logFastFromISR!\n");
            Logger::log("%f\n%f\n%f\n%f\n", mainFreqVar, mainAmplVar, modFreqVar, modAmplVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::error, "%f\n", mainFreqVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::fatal, "%f\n", mainAmplVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::info, "%f\n", modFreqVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::system, "%f\n", mainFreqVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::trace, "%f\n", mainAmplVar);
            Logger::log(Logger::Verbosity::mid, Logger::Type::warning, "%f\n", modFreqVar);
        }
    }
};

BlinkTestApp blinkTestApp("App1", 0x500, FibSys::getAppMaxPriority());

// for (int y = 0; y < i; y++)
// {
//     Logger::trace("mainTask", "%.*s", y, "=================================================================================");
// }
// i++;