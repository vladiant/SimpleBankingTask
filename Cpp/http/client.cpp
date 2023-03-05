#include <cstddef>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include "utils.hpp"

constexpr int BANKING_PORT = 50013;

int main() {
  httplib::Client cli("0.0.0.0", BANKING_PORT);

  std::cout << "Enter ESC to leave the command prompt\n";

  for (std::string line; line != "\x1B";) {
    std::cout << "$ ";
    std::getline(std::cin, line);

    const auto commands = extractCommands(line);

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