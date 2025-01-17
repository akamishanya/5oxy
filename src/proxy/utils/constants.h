#ifndef PROXY_UTILS_CONSTANTS_H
#define PROXY_UTILS_CONSTANTS_H

namespace proxy::utils {
    enum class SocksVersion : proxy::utils::byte {
        SOCKS5 = 0x05
    };

    enum class AuthMethods : proxy::utils::byte {
        NO_AUTH = 0x00
    };
}

#endif
