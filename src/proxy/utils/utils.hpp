#ifndef PROXY_UTILS_UTILS_HPP
#define PROXY_UTILS_UTILS_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace proxy::utils {
    typedef uint8_t byte;

    std::string bytesToString(std::vector<proxy::utils::byte> &bytes, std::size_t length);
}

#endif
