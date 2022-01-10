#pragma once

#include <cstddef>
#include <cstdint>

template <typename PinEnumType> class GpioIF
{
public:
    enum class Mode
    {
        input,
        output
    };

    enum class PinState
    {
        low,
        high
    };

    enum class Pull
    {
        none,
        up,
        down
    };

    virtual void init(PinEnumType pin, Mode mode, PinState pinStateInitial, Pull pull) = 0;
    [[nodiscard]] virtual inline bool read(PinEnumType pin) = 0;
    virtual inline void write(PinEnumType pin, PinState pinState) = 0;
    virtual void deinit(PinEnumType pin) = 0;

protected:
    GpioIF() = default;
    virtual ~GpioIF() = default;
};
