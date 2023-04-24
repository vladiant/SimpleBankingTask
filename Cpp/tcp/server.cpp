#include <asio.hpp>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "controller.hpp"
#include "utils.hpp"

constexpr auto kLogFolder = "log";
constexpr auto kFileName = "account_history.txt";
const unsigned short BANKING_PORT = 50013;

using asio::ip::tcp;

std::weak_ptr<tcp::socket> socketToCancel;

auto getPath() {
  std::filesystem::path tempPath{kLogFolder};
  tempPath /= kFileName;
  return tempPath;
}

void signalHandler([[maybe_unused]] int signal) {
  if (auto handler = socketToCancel.lock()) {
    handler->shutdown(asio::socket_base::shutdown_both);
  }
}

int main() {
  if (!std::filesystem::exists(kLogFolder)) {
    std::filesystem::create_directory(kLogFolder);
  }

  try {
    std::signal(SIGINT, signalHandler);
    // K8s signals
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGKILL, signalHandler);
    // K8s optional signal
    std::signal(SIGQUIT, signalHandler);

    auto storage = sbt::createFileStorage(getPath().string());
    sbt::Context context{storage};

    sbt::initLoop(context);

    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), BANKING_PORT));

    for (;;) {
      tcp::socket socket(io_context);
      std::shared_ptr<tcp::socket> socketPtr(&socket, [](void*) {});
      socketToCancel = socketPtr;

      acceptor.accept(socket);

      std::array<char, 128> buf;
      asio::error_code error;
      size_t len = socket.read_some(asio::buffer(buf), error);

      if (error) {
        std::cout << "Receive error\n";
        throw asio::system_error(error);  // Some other error.
      }

      const std::string line(buf.data(), len);
      const auto status = sbt::processCommand(line, context);

      const auto result = sbt::processStatus(status, context);

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
