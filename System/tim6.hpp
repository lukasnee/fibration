#include <cstdint>

class Tim6 
{
public:
    Tim6(std::uint16_t prescaler, std::uint16_t period);
    void start();
    uint32_t getCounterValue();
    void stop();
    void operator=(Tim6 const&) = delete;
    Tim6(Tim6 const&) = delete;
    ~Tim6();
};