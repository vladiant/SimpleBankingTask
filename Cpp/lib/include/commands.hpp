#pragma once

#include <string>
#include <vector>

#include "types.hpp"

namespace sbt {

Status processHistory(const Arguments& arguments, Context& context);

Status processWithdraw(const Arguments& arguments, Context& context);

Status processDeposit(const Arguments& arguments, Context& context);

Status processTransfer(const Arguments& arguments, Context& context);

Status processLogout(const Arguments& arguments, Context& context);

Status processLogin(const Arguments& arguments, Context& context);

}  // namespace sbt