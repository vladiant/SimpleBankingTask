#include <cstddef>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include "utils.hpp"

constexpr auto kEscSymbol = "\x1B";
constexpr int BANKING_PORT = 50015;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: client <host>" << std::endl;
    return EXIT_FAILURE;
  }

  httplib::Client cli(argv[1], BANKING_PORT);

  std::cout << "Enter ESC to leave the command prompt\n";

  for (std::string line; line != kEscSymbol;) {
    std::cout << "$ ";
    std::getline(std::cin, line);

    const auto commands = extractCommands(line);

    if (kEscSymbol == line) {
      break;
    }

    if (commands.empty()) {
      continue;
    }

    const std::string command = "/" + commands.at(0);

    httplib::Params params;
    for (auto it = std::next(std::begin(commands)); it != std::end(commands);
         ++it) {
      params.emplace(std::to_string(std::distance(std::begin(commands), it)),
                     *it);
    }

    if (auto res = cli.Post(command, params)) {
      if (res->status == 200) {
        std::cout << res->body;
      } else {
        std::cout << "HTTP response failed with status: "
                  << httplib::detail::status_message(res->status) << '\n';
      }
    } else {
      auto err = res.error();
      std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    }
  }

  return EXIT_SUCCESS;
}