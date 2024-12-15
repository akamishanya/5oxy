#ifndef PROXY_SESSION_SESSION_HPP
#define PROXY_SESSION_SESSION_HPP

#include <memory>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "proxy/utils/utils.hpp"

namespace proxy::session {
    class Session : public std::enable_shared_from_this<Session> {
        boost::asio::io_context &ioContext;
        boost::asio::ip::tcp::socket serverSocket;
        boost::asio::ip::tcp::socket clientSocket;

        std::vector<proxy::utils::byte> clientBuffer;
        std::vector<proxy::utils::byte> serverBuffer;

    public:
        Session(boost::asio::io_context &ioContext, boost::asio::ip::tcp::socket clientSocket);

        void start();

    private:
        void handleHandshake();
        void handleCommand();
        void handleDataTransfer();
    };
}

#endif
