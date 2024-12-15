#include "proxy/utils/utils.hpp"

#include <cstddef>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

std::string proxy::utils::bytesToString(std::vector<proxy::utils::byte> &bytes, std::size_t length) {
    std::ostringstream oss;

    for (int i = 0; i < length; i++) {
        oss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
        if (i != length - 1) {
            oss << " ";
        }
    }

    return oss.str();
}
