#pragma once

namespace Fib {

class Stream {
	// todo
	virtual bool write(std::uint8_t *pData, std::uint16_t size) = 0;
	virtual bool read(std::uint8_t *pData, std::uint16_t size) = 0;
};

} // namespace Fib