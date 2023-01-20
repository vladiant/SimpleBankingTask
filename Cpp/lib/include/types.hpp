#pragma once

#include <iosfwd>
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
  std::shared_ptr<std::iostream> logFile;
  // TODO: Consider using null object
  std::shared_ptr<std::ostream> output;
  // TODO: Consider using null object
  std::shared_ptr<std::istream> input;
};

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  LOGIN,
  LOGOUT,
  NOT_LOGGED,
  OK
};
