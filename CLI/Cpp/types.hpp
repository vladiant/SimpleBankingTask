#pragma once

#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using BalanceType = float;
using Balances = std::map<std::string, BalanceType>;
using Arguments = std::vector<std::string>;

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
