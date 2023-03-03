#include <cstddef>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

constexpr int BANKING_PORT = 50013;

int main() {
  httplib::Client cli("0.0.0.0", BANKING_PORT);

  if (auto res = cli.Get("/hi")) {
    if (res->status == 200) {
      std::cout << res->body << std::endl;
    }
  } else {
    auto err = res.error();
    std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
  }

  return EXIT_SUCCESS;
}