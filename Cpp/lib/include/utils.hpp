#pragma once

#include "types.hpp"

namespace sbt {

void printCommand(const Arguments& arguments);

void printNotSupportedCommand(const Arguments& arguments);

void initializeUserBalance(const std::string& user, Balances& balances);

Arguments extractCommands(const std::string& line);

}  // namespace sbt