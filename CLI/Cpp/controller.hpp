#pragma once

#include "types.hpp"

Status processCommand(const std::string& line, const std::string& fileName,
                      Context& context);

Balances readBallances(const std::string& fileName);

void initLoop(const std::string& fileName, Context& context);

void processLoop(const std::string& fileName, Context& context);