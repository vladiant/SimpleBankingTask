#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include "types.hpp"

constexpr auto fileName = "account_history.txt";

void printCommand(const std::vector<std::string>& commands) {
  for (const auto& command : commands) {
    std::cout << command << " ";
  }
  std::cout << '\n';
}

void printNotSupportedCommand(const std::vector<std::string>& commands) {
  std::cout << "Command not supported: ";
  printCommand(commands);
}

std::vector<std::string> extractCommands(const std::string& line) {
  std::istringstream inputLine{line};
  std::vector<std::string> commands;

  for (std::string command; std::getline(inputLine, command, ' ');) {
    if (command.empty()) {
      continue;
    }

    commands.emplace_back(std::move(command));
  }

  return commands;
}

void initializeUserBalance(const std::string& user, Balances& balances) {
  const auto it = balances.find(user);
  if (it == balances.end()) {
    balances[user] = 0;
  }
}

Status processHistory(const std::vector<std::string>&, Context& context) {
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

Status processWithdraw(const std::vector<std::string>& arguments,
                       Context& context) {
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

Status processDeposit(const std::vector<std::string>& arguments,
                      Context& context) {
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

Status processTransfer(const std::vector<std::string>& arguments,
                       Context& context) {
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

Status processCommand(const std::string& line, Context& context) {
  const auto commands = extractCommands(line);

  // Skip empty command line
  if (commands.empty()) {
    return Status::EMPTY;
  }

  const auto& command = commands.at(0);

  // Process command
  switch (commands.size()) {
    case 1:
      if (command == "logout") {
        if (context.logFile) {
          *context.logFile << context.username << " "
                           << "logout" << '\n';
          context.username.clear();
        }

        return Status::LOGOUT;
      } else if (command == "history") {
        processHistory({}, context);
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 2:
      if (command == "get") {
        const std::string subCommand{commands[1]};
        if (subCommand == "balance") {
          if (context.username.empty()) {
            std::cout << "No user logged!\n";
            return Status::NOT_LOGGED;
          }
          std::cout << context.balances[context.username] << '\n';
        } else {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }
      } else if (command == "withdraw") {
        if (context.username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        // TODO: Handle insufficient amount
        // TODO: Which user?
        processWithdraw({commands[1]}, context);

      } else if (command == "deposit") {
        if (context.username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        // TODO: Which user?
        // TODO: When OK?
        processDeposit({commands[1]}, context);
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 3:
      if (command == "login") {
        const std::string password{commands[2]};
        // TODO: Handle password check
        context.username = commands[1];
        initializeUserBalance(context.username, context.balances);
        std::cout << "Welcome, " << context.username << '\n';
        if (context.logFile) {
          *context.logFile << context.username << " "
                           << "login " << context.username << " " << password
                           << '\n';
        }

      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 4:
      if (command == "transfer") {
        if (context.username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        const std::string subCommand{commands[2]};
        if (subCommand != "to") {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }

        processTransfer({commands[1], commands[3]}, context);

      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    default:
      printNotSupportedCommand(commands);
      return Status::UNKNOWN_COMMAND;
  }

  return Status::OK;
}

auto readBallances(const std::string& fileName) {
  Context context;

  // TODO: Process it properly under context
  std::fstream logFile(fileName, std::ios_base::in);

  if (!logFile.is_open()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  // Commands loop
  while (logFile.good()) {
    std::string line;
    std::getline(logFile, line);
    if (line.empty()) {
      break;
    }

    const auto commands = extractCommands(line);

    // Skip empty command line
    if (commands.empty()) {
      continue;
    }

    // First command is the username
    context.username = commands.at(0);
    const auto& command = commands.at(1);

    // TODO: Set it to use context
    initializeUserBalance(context.username, context.balances);

    // Process command
    switch (commands.size()) {
      case 3:
        if (command == "withdraw") {
          processWithdraw({commands[2]}, context);
        } else if (command == "deposit") {
          processDeposit({commands[2]}, context);
        }
        break;
      case 5:
        if (command == "transfer") {
          const std::string subCommand{commands[3]};
          if (subCommand != "to") {
            printNotSupportedCommand(commands);
            continue;
          }

          const std::string user{commands[4]};

          // It is assumed that record is OK
          // and transfer was done to existing user.
          initializeUserBalance(user, context.balances);

          processTransfer({commands[2], user}, context);
        }
        break;
    }
  }

  return context.balances;
}

int main(int, char**) {
  Context context;

  // TODO: Handle processing of file
  context.logFile.emplace(
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  if (!context.logFile->is_open()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  // Consider empty name as no logged user
  std::string username{};

  context.balances = readBallances(fileName);

  // Commands loop
  bool shouldProcess = true;
  while (shouldProcess) {
    std::string line;
    // Command prompt
    std::cout << "$ ";
    std::getline(std::cin, line);

    // TODO: Refactor balance reading
    const auto status = processCommand(line, context);

    // Status process
    switch (status) {
      case Status::OK:
        std::cout << "ok!\n";
        break;
      case Status::NOT_LOGGED:
        std::cout << "not logged!\n";
        break;
      case Status::UNKNOWN_USER:
        std::cout << "unknown user!\n";
        break;
      case Status::EMPTY:
        std::cout << "empty command line!\n";
        break;
      case Status::LOGOUT:
        std::cout << "logout!\n";
        shouldProcess = false;
        break;
      case Status::UNKNOWN_COMMAND:
        std::cout << "unknown command!\n";
        break;
    }

  }  // end command loop

  return EXIT_SUCCESS;
}
