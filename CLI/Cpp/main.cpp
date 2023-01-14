#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include "commands.hpp"
#include "controller.hpp"
#include "types.hpp"
#include "utils.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  Context context;

  initLoop(fileName, context);

  processLoop(fileName, context);

  return EXIT_SUCCESS;
}
