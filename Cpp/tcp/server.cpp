#include <asio.hpp>
#include <iostream>
#include <string>

#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

using asio::ip::tcp;

const unsigned short BANKING_PORT = 50013;

int main() {
  try {
    Context context;

    context.output = std::make_shared<std::stringstream>();

    initLoop(fileName, context);

    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), BANKING_PORT));

    for (;;) {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      std::array<char, 128> buf;
      asio::error_code error;
      size_t len = socket.read_some(asio::buffer(buf), error);

      if (error) {
        std::cout << "Receive error\n";
        throw asio::system_error(error);  // Some other error.
      }

      const std::string line(buf.data(), len);
      const auto status = processCommand(line, fileName, context);

      const auto result = processStatus(status, context);

      asio::error_code ignored_error;
      asio::write(socket, asio::buffer(result), ignored_error);
      if (ignored_error) {
        std::cout << "Sent error\n";
      }
    }
  } catch (asio::system_error& e) {
    std::cerr << "TCP exception: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}