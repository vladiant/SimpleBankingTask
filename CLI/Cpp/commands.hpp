#pragma once

#include <string>
#include <vector>

#include "types.hpp"

Status processHistory(const std::vector<std::string>& arguments,
                      Context& context);

Status processWithdraw(const std::vector<std::string>& arguments,
                       Context& context);

Status processDeposit(const std::vector<std::string>& arguments,
                      Context& context);

Status processTransfer(const std::vector<std::string>& arguments,
                       Context& context);
