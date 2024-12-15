#include "proxy/proxy.hpp"

#include <memory>
#include <utility>

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include "proxy/session/session.hpp"

proxy::Proxy::Proxy(unsigned short port)
    : ioContext(), acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

void proxy::Proxy::start() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket clientSocket) {
            if (!ec) {
                std::make_shared<proxy::session::Session>(ioContext, std::move(clientSocket))->start();
            }

            start();
        }
    );

    ioContext.run();
}
