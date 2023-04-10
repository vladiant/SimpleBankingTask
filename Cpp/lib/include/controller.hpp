#pragma once

#include <string>

#include "types.hpp"

namespace sbt {

Status processCommand(const std::string& line, const std::string& fileName,
                      Context& context);

Balances readBallances(const std::string& fileName);

void initLoop(const std::string& fileName, Context& context);

void processLoop(const std::string& fileName, Context& context);

std::string processStatus(Status status, Context& context);

}  // namespace sbt