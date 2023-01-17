#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using BalanceType = float;
using Balances = std::map<std::string, BalanceType>;
using Arguments = std::vector<std::string>;

struct Context {
  // Consider empty name as no logged user
  std::string username;
  Balances balances;
  // TODO: General iostream
  std::optional<std::fstream> logFile;
  // TODO: Consider using null object
  std::shared_ptr<std::ostream> output;
};

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  LOGOUT,
  NOT_LOGGED,
  OK
};
