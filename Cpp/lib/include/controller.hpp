#pragma once

#include <string>

#include "types.hpp"

namespace sbt {

Status processCommand(const std::string& line, Context& context);

Balances readBalances(Storage& storage);

void initLoop(Context& context);

void processLoop(Context& context, Input& input);

std::string processStatus(Status status, Context& context);

}  // namespace sbt