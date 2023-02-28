#include <array>
#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

const unsigned short BANKING_PORT = 50013;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return EXIT_FAILURE;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
        resolver.resolve(argv[1], std::to_string(BANKING_PORT));

    std::cout << "Enter ESC to leave the command prompt\n";

    for (std::string request; request != "\x1B";) {
      tcp::socket socket(io_context);
      asio::connect(socket, endpoints);

      std::cout << "$ ";
      std::getline(std::cin, request);

      for (;;) {
        std::array<char, 128> buf;
        asio::error_code error;

        socket.write_some(asio::buffer(request), error);
        if (error) {
          std::cout << "Sent error\n";
          throw asio::system_error(error);
        }

        size_t len = socket.read_some(asio::buffer(buf), error);

        if (error == asio::error::eof) {
          // Connection cleanly closed
          break;
        } else if (error) {
          std::cout << "Receive error\n";
          throw asio::system_error(error);  // Some other error.
        }

        std::cout.write(buf.data(), len);
      }
    }
  } catch (asio::system_error& e) {
    std::cerr << "TCP exception: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
