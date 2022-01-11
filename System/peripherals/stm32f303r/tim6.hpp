#include <cstdint>

class Tim6 
{
public:
    typedef std::uint32_t CounterType;

    Tim6(std::uint16_t prescaler, std::uint16_t period);
    void start();
    void overflowCallback();
    CounterType getCounterValue();
    void stop();
    void operator=(Tim6 const&) = delete;
    Tim6(Tim6 const&) = delete;
    ~Tim6();
private:
    CounterType counterValue;
};