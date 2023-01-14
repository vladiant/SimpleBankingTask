#pragma once

#include <fstream>
#include <map>
#include <optional>
#include <string>

using BalanceType = float;
using Balances = std::map<std::string, BalanceType>;

struct Context {
  std::string username;
  Balances balances;
  std::optional<std::fstream> logFile;
};

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  LOGOUT,
  NOT_LOGGED,
  OK
};
