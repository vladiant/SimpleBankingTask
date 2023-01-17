#include <iostream>

#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  Context context;

  context.output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});

  initLoop(fileName, context);

  processLoop(fileName, context);

  return EXIT_SUCCESS;
}
