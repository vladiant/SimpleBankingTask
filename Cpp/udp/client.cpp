#include <array>
#include <asio.hpp>
#include <iostream>

using asio::ip::udp;

const unsigned short BANKING_PORT = 50014;
constexpr auto kEscSymbol = "\x1B";

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return EXIT_FAILURE;
    }

    asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
        *resolver.resolve(udp::v4(), argv[1], std::to_string(BANKING_PORT))
             .begin();

    std::cout << "Enter ESC to leave the command prompt\n";

    for (std::string request; request != kEscSymbol;) {
      udp::socket socket(io_context);
      socket.open(udp::v4());

      std::cout << "$ ";
      std::getline(std::cin, request);

      if (kEscSymbol == request) {
        break;
      }

      socket.send_to(asio::buffer(request), receiver_endpoint);

      std::array<char, 128> recv_buf{};
      udp::endpoint sender_endpoint;
      size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

      std::cout.write(recv_buf.data(), len);
    }
  } catch (asio::system_error& e) {
    std::cerr << "UDP exception: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
