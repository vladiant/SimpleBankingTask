#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

int main(int, char**) {
  Context context;

  initLoop(fileName, context);

  processLoop(fileName, context);

  return EXIT_SUCCESS;
}
