#include <climits>
#include <cstdlib>
#include <iostream>
// TODO: #include <memory>
#include <stdexcept>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/detail/trivial_keyword.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

// TODO: #include "proxy/proxy.hpp"

namespace {
    void initLog();

    unsigned short getPort(int argc, char **argv);
}

int main(int argc, char **argv) {
    initLog();

    unsigned short port = getPort(argc, argv);
    BOOST_LOG_TRIVIAL(info) << "Using port: " << port;

    // TODO: auto proxyInstance = std::make_unique<proxy::Proxy>(port);
    // TODO: proxyInstance->start();

    return EXIT_SUCCESS;
}

namespace {
    void initLog() {
        // Логирование в консоль
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "%Severity% | %TimeStamp%: %Message%");

        // Логирование в файл
        boost::log::add_file_log(
            boost::log::keywords::file_name = "/tmp/5oxy/log/%N.log",
            boost::log::keywords::rotation_size = 10 * 1024 * 1024, // 10 МБ
            boost::log::keywords::format = "%Severity% | %TimeStamp%: %Message%"
        );

        // Добавление атрибутов времени
        boost::log::add_common_attributes();

        // Фильтрация сообщений
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    }

    unsigned short getPort(int argc, char **argv) {
        constexpr unsigned short DEFAULT_PORT = 1080;

        if (argc > 1) {
            int desiredPort = 0;

            try {
                desiredPort = std::stoi(argv[1]);
            } catch ([[maybe_unused]] std::invalid_argument &e) {
                BOOST_LOG_TRIVIAL(warning) << "Invalid port: " << argv[1];
                return DEFAULT_PORT;
            } catch ([[maybe_unused]] std::out_of_range &e) {
                BOOST_LOG_TRIVIAL(warning) << "Port out of range: " << argv[1];
                return DEFAULT_PORT;
            }

            if (!(0 <= desiredPort && desiredPort <= USHRT_MAX)) {
                BOOST_LOG_TRIVIAL(warning) << "Port out of range: " << argv[1]
                                           << " (must be in range 0 to " << USHRT_MAX << ")";
                return DEFAULT_PORT;
            }

            return static_cast<unsigned short>(desiredPort);
        }

        return DEFAULT_PORT;
    }
}
