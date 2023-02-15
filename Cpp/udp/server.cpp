#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::udp;

const unsigned short BANKING_PORT = 50013;

int main() {
  try {
    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), BANKING_PORT));

    for (;;) {
      std::array<char, 128> recv_buf;
      udp::endpoint remote_endpoint;
      size_t len = socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

      std::cout.write(recv_buf.data(), len);
      std::cout << '\n';

      std::string message = "Response " + std::string(recv_buf.data(), len);

      asio::error_code ignored_error;
      socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);
      if (ignored_error) {
        std::cout << "Sent error\n";
      }
    }
  } catch (asio::system_error& e) {
    std::cerr << "UDP exception: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
