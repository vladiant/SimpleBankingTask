#include <iostream>

#include "controller.hpp"
#include "utils.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  auto storage = sbt::createFileStorage(fileName);
  sbt::Context context{storage};

  context.output = std::shared_ptr<std::ostream>(&std::cout, [](void*) {});

  sbt::initLoop(context);
  sbt::processLoop(context, std::cin);

  return EXIT_SUCCESS;
}
