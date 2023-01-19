#include <iostream>

#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  Context context;

  context.output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
  context.input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});

  initLoop(fileName, context);
  processLoop(fileName, context);

  return EXIT_SUCCESS;
}
