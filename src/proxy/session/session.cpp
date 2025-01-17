#include "proxy/session/session.hpp"

#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/log/trivial.hpp>

#include "proxy/utils/utils.hpp"
#include "proxy/utils/constants.h"

constexpr std::size_t BUFFER_SIZE = 4096;
constexpr proxy::utils::byte CLEAN = 0x00;

proxy::session::Session::Session(boost::asio::io_context &ioContext, boost::asio::ip::tcp::socket &&clientSocket)
    : ioContext(ioContext), serverSocket(ioContext), clientSocket(std::move(clientSocket)),
      clientBuffer(BUFFER_SIZE, CLEAN), serverBuffer(BUFFER_SIZE, CLEAN) {}

void proxy::session::Session::start() {
    handleHandshake();
}

void proxy::session::Session::handleHandshake() {
    auto self = shared_from_this();

    clientSocket.async_read_some(
        boost::asio::buffer(clientBuffer, BUFFER_SIZE), [this, self](boost::system::error_code ec, std::size_t length) {
            if (ec || length <= 0) {
                BOOST_LOG_TRIVIAL(error) << "Failed to receive request. "
                                         << "Request length: " << length << ". "
                                         << "Error: " << ec.message();
                clientSocket.close();
                return;
            }

            BOOST_LOG_TRIVIAL(info) << "Client request: " << proxy::utils::bytesToString(clientBuffer, length);

            proxy::utils::byte ver = clientBuffer[0];
            if (static_cast<proxy::utils::SocksVersion>(ver) != proxy::utils::SocksVersion::SOCKS5) {
                BOOST_LOG_TRIVIAL(error) << "Invalid SOCKS version: " << static_cast<int>(ver);
                clientSocket.close();
                return;
            }

            int nmethods = static_cast<int>(clientBuffer[1]);
            if (nmethods < 1) {
                BOOST_LOG_TRIVIAL(error) << "Invalid number of methods: " << nmethods;
                clientSocket.close();
                return;
            } else if (nmethods != length - 2) {
                BOOST_LOG_TRIVIAL(error) << "Invalid handshake length "
                                         << "(number of methods: " << nmethods << ", "
                                         << "actual length: " << length << ")";
                clientSocket.close();
                return;
            }

            auto noAuth = std::find(
                clientBuffer.begin() + 2, clientBuffer.end(),
                static_cast<proxy::utils::byte>(proxy::utils::AuthMethods::NO_AUTH)
            );
            if (noAuth == clientBuffer.end()) {
                BOOST_LOG_TRIVIAL(error) << "No authentication not found in the user's request";
                clientSocket.close();
                return;
            }

            clientBuffer[0] = 0x05;
            clientBuffer[1] = 0x00;
            clientSocket.async_send(
                boost::asio::buffer(clientBuffer, 2),
                [this, self](boost::system::error_code ec, std::size_t length) {
                    if (ec || length != 2) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to send response. "
                                                 << "Response length: " << length << "."
                                                 << "Error: " << ec.message();
                        clientSocket.close();
                    }

                    BOOST_LOG_TRIVIAL(info) << "Response sent: " << proxy::utils::bytesToString(clientBuffer, length);
                    handleCommand();
                }
            );
        }
    );
}

void proxy::session::Session::handleCommand() {}

void proxy::session::Session::handleDataTransfer() {}
