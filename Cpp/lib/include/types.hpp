#pragma once

#include <iosfwd>
#include <map>
#include <memory>
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
using Output = std::shared_ptr<std::ostream>;

struct Context {
  Context(Storage& aLog) : log{aLog} {}

  // Consider empty name as no logged user
  std::string username;
  Balances balances;
  Storage& log;
  // TODO: Consider using null object
  Output output;
  Result result;
};

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  LOGIN,
  LOGOUT,
  NOT_LOGGED,
  RESULT,
  OK
};

}  // namespace sbt