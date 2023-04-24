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
const unsigned short BANKING_PORT = 50014;

using asio::ip::udp;

std::weak_ptr<udp::socket> socketToCancel;

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

    udp::socket socket(io_context, udp::endpoint(udp::v4(), BANKING_PORT));
    std::shared_ptr<udp::socket> socketPtr(&socket, [](void*) {});
    socketToCancel = socketPtr;

    for (;;) {
      std::array<char, 128> recv_buf;
      udp::endpoint remote_endpoint;
      size_t len = socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

      const std::string line(recv_buf.data(), len);
      const auto status = sbt::processCommand(line, context);

      const auto result = sbt::processStatus(status, context);

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
