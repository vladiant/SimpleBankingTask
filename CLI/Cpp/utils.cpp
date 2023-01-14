#include "utils.hpp"

#include <iostream>
#include <sstream>

void printCommand(const Arguments& arguments) {
  for (const auto& argument : arguments) {
    std::cout << argument << " ";
  }
  std::cout << '\n';
}

void printNotSupportedCommand(const Arguments& arguments) {
  std::cout << "Command not supported: ";
  printCommand(arguments);
}

void initializeUserBalance(const std::string& user, Balances& balances) {
  const auto it = balances.find(user);
  if (it == balances.end()) {
    balances[user] = 0;
  }
}

Arguments extractCommands(const std::string& line) {
  std::istringstream inputLine{line};
  Arguments commands;

  for (std::string command; std::getline(inputLine, command, ' ');) {
    if (command.empty()) {
      continue;
    }

    commands.emplace_back(std::move(command));
  }

  return commands;
}