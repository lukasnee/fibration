#include "argBuffer.hpp"

#include <string_view>
#include <algorithm> // for std::min, std::copy_n

ArgBuffer::ArgBuffer() : ArgVector() {}

ArgBuffer::ArgBuffer(std::size_t argc, const char *argv[]) { this->copyFrom(argc, argv); }

bool ArgBuffer::copyFrom(std::size_t argc, const char *argv[]) {
    if (!argv) {
        return false;
    }
    this->count = 0;
    this->args.fill(nullptr);
    std::size_t buff_left = this->buffer.size();
    for (const char **argv_it = argv; (*argv_it && argv_it < (argv + argc) && count < this->args.size()); argv_it++) {
        const std::string_view arg_it_sv{*argv_it};
        if ((arg_it_sv.length() + sizeof('\0')) > buff_left) {
            return false;
        }
        char *arg = &this->buffer[this->buffer.size() - buff_left];
        std::copy_n(arg_it_sv.begin(), arg_it_sv.length(), arg);
        arg[arg_it_sv.length()] = '\0';
        buff_left -= (arg_it_sv.length() + sizeof('\0'));
        this->args[this->count++] = arg;
    }
    return true;
}

ArgBuffer::ArgBuffer(const char *arg_str) {
    std::string_view src{arg_str};
    auto copy_len = std::min(src.length(), this->buffer.size() - sizeof('\0'));
    std::copy_n(src.begin(), copy_len, this->buffer.data());
    this->buffer[copy_len] = '\0';
    this->resolveIntoArgs();
}

bool ArgBuffer::resolveIntoArgs() {
    return this->resolveStringToArgs(reinterpret_cast<char *>(this->buffer.data()), this->buffer.size());
}

bool ArgBuffer::restoreIntoString() {
    bool result = false;

    for (std::size_t arg_it = 1; arg_it < this->count; arg_it++) {
        std::size_t char_buff_offset = this->args[arg_it] - this->buffer.data();
        if (char_buff_offset < this->buffer.size()) {
            if (this->buffer[char_buff_offset] == '\0') {
                this->buffer[char_buff_offset] = ' ';
            }
            result = true;
        }
        else {
            break;
        }
    }

    return result;
}
