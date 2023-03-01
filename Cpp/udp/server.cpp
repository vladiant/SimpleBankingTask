#include <asio.hpp>
#include <iostream>
#include <string>

#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

using asio::ip::udp;

const unsigned short BANKING_PORT = 50013;

int main() {
  try {
    Context context;

    context.output = std::make_shared<std::stringstream>();

    initLoop(fileName, context);

    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), BANKING_PORT));

    for (;;) {
      std::array<char, 128> recv_buf;
      udp::endpoint remote_endpoint;
      size_t len = socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

      const std::string line(recv_buf.data(), len);
      const auto status = processCommand(line, fileName, context);

      const auto result = processStatus(status, context);

      asio::error_code ignored_error;
      socket.send_to(asio::buffer(result), remote_endpoint, 0, ignored_error);
      if (ignored_error) {
        std::cout << "Sent error\n";
      }
    }
  } catch (asio::system_error& e) {
    std::cerr << "UDP exception: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
