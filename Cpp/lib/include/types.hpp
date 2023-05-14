#pragma once

#include <iosfwd>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace sbt {

using BalanceType = float;
using Balances = std::map<std::string, BalanceType>;
using Arguments = std::vector<std::string>;
using Result = std::string;
using Storage = std::iostream;
using Input = std::istream;
using Output = std::ostream;

struct Context {
  Context(Storage& aLog, Output& aOutput) : log{aLog}, output{aOutput} {}

  // Consider empty name as no logged user
  std::string username;
  Balances balances;
  Storage& log;
  Output& output;
  Result result;
};

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  INVALID_ARGUMENTS,
  LOGIN,
  LOGOUT,
  NOT_LOGGED,
  RESULT,
  OK
};

}  // namespace sbt