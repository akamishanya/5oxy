#ifndef PROXY_PROXY_HPP
#define PROXY_PROXY_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace proxy {
    class Proxy {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::acceptor acceptor;

    public:
        explicit Proxy(unsigned short port);

        void start();
    };
}

#endif
