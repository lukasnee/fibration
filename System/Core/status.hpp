
#include <cstdint>
#include <utility>

class Stats {
public:
    using KeyType = const char *;
    template<typename ValueType> using KeyValue = std::pair<KeyType, ValueType>;


    struct Sysinfo {
        KeyValue<std::uint32_t> curfreeHeapSize;
        KeyValue<std::uint32_t> minFreeHeapSize;
        KeyValue<std::uint32_t> maxFreeHeapSize;
    } sysInfo;
};
