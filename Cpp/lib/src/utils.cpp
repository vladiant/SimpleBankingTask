#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace sbt {

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

std::string readFromStorage(Storage& storage) {
  std::string str;

  if (!storage) {
    return str;
  }

  // Get stored data size
  storage.seekg(0, std::ios::end);
  str.reserve(storage.tellg());
  storage.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(storage)),
             std::istreambuf_iterator<char>());

  // Assume new lines will be stored at the end
  storage.seekg(0, std::ios::end);
  return str;
}

std::fstream createFileStorage(const std::string& fileName) {
  return std::fstream{
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out};
}

}  // namespace sbt