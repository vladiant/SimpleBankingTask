#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "utils.hpp"

namespace sbt {

Status processHistory([[maybe_unused]] const Arguments& arguments,
                      Context& context) {
  // Flush existing data
  if (context.log) {
    context.log->flush();
  }

  auto data = readFromStorage(context.log);
  if (context.output) {
    *context.output << data << '\n';
  }

  return Status::OK;
}

Status processWithdraw(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  std::stringstream ss(arguments.at(0));
  BalanceType amount;
  ss >> amount;
  context.balances[context.username] -= amount;
  if (!context.log) {
    return Status::OK;
  }
  *context.log << context.username << " "
               << "withdraw " << amount << '\n';
  return Status::OK;
}

Status processDeposit(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  std::stringstream ss(arguments.at(0));
  BalanceType amount;
  ss >> amount;
  context.balances[context.username] += amount;
  if (!context.log) {
    return Status::OK;
  }
  *context.log << context.username << " "
               << "deposit " << amount << '\n';
  return Status::OK;
}

Status processTransfer(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  std::stringstream ss(arguments.at(0));
  BalanceType amount;
  ss >> amount;
  const std::string user{arguments.at(1)};
  // TODO: Handle insufficient balance

  // Self transfer
  if (context.username == user) {
    // Ignore self transfer, no problem
    return Status::OK;
  }

  // TODO: Is transfer a deposit
  // TODO: When OK? Should user exist?

  if (context.balances.find(user) == context.balances.end()) {
    return Status::UNKNOWN_USER;
  }

  context.balances[context.username] -= amount;
  context.balances[user] += amount;

  if (context.log) {
    *context.log << context.username << " "
                 << "transfer " << amount << " to " << user << '\n';
  }

  // TODO: Fix duplication
  if (context.output) {
    *context.output << context.username << " "
                    << "transfer " << amount << " to " << user << '\n';
  }

  return Status::OK;
}

Status processLogout(const Arguments&, Context& context) {
  // TODO: Handle improper arguments size
  if (context.log) {
    *context.log << context.username << " "
                 << "logout" << '\n';
  }
  context.username.clear();

  // Flush existing data
  if (context.log) {
    context.log->flush();
  }

  return Status::LOGOUT;
}

Status processLogin(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  const std::string password{arguments.at(1)};

  // TODO: Handle password check
  context.username = arguments.at(0);
  initializeUserBalance(context.username, context.balances);

  if (context.log) {
    *context.log << context.username << " "
                 << "login "
                 << " " << password << '\n';
  }

  return Status::LOGIN;
}

}  // namespace sbt