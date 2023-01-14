#include "commands.hpp"

#include <iostream>
#include <sstream>

Status processHistory(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  const auto fileName = arguments.at(0);

  // Flush existing data
  if (context.logFile) {
    context.logFile->flush();
  }

  std::fstream historyFile(fileName, std::ios_base::in);
  while (historyFile) {
    std::string log;
    std::getline(historyFile, log);
    if (log.empty()) {
      break;
    }
    std::cout << log << '\n';
  }
  return Status::OK;
}

Status processWithdraw(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  std::stringstream ss(arguments.at(0));
  BalanceType amount;
  ss >> amount;
  context.balances[context.username] -= amount;
  if (!context.logFile) {
    return Status::OK;
  }
  *context.logFile << context.username << " "
                   << "withdraw " << amount << '\n';
  return Status::OK;
}

Status processDeposit(const Arguments& arguments, Context& context) {
  // TODO: Handle improper arguments size
  std::stringstream ss(arguments.at(0));
  BalanceType amount;
  ss >> amount;
  context.balances[context.username] += amount;
  if (!context.logFile) {
    return Status::OK;
  }
  *context.logFile << context.username << " "
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

  if (!context.logFile) {
    return Status::OK;
  }

  std::cout << context.username << " "
            << "transfer " << amount << " to " << user << '\n';

  return Status::OK;
}

Status processLogout(const Arguments&, Context& context) {
  // TODO: Handle improper arguments size
  if (context.logFile) {
    *context.logFile << context.username << " "
                     << "logout" << '\n';
  }
  context.username.clear();

  return Status::OK;
}
