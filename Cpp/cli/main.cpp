#include <iostream>

#include "controller.hpp"
#include "utils.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  sbt::Context context;
  context.log = sbt::createFileStorage(fileName);

  context.output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});
  context.input = std::shared_ptr<std::istream>(&std::cin, [](void*) {});

  sbt::initLoop(context);
  sbt::processLoop(context);

  return EXIT_SUCCESS;
}
