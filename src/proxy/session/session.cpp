#include "proxy/session/session.hpp"

#include <cstddef>
#include <utility>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "proxy/utils/utils.hpp"

constexpr std::size_t BUFFER_SIZE = 4096;
constexpr proxy::utils::byte CLEAN = 0x00;

proxy::session::Session::Session(boost::asio::io_context &ioContext, boost::asio::ip::tcp::socket &&clientSocket)
    : ioContext(ioContext), serverSocket(ioContext), clientSocket(std::move(clientSocket)),
      clientBuffer(BUFFER_SIZE, CLEAN), serverBuffer(BUFFER_SIZE, CLEAN) {}

void proxy::session::Session::start() {
    handleHandshake();
}

void proxy::session::Session::handleHandshake() {}

void proxy::session::Session::handleCommand() {}

void proxy::session::Session::handleDataTransfer() {}
