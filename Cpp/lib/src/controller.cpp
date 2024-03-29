#include "controller.hpp"

#include <iostream>
#include <sstream>

#include "commands.hpp"
#include "null_output.hpp"
#include "queries.hpp"
#include "utils.hpp"

namespace sbt {

Status processCommand(const std::string& line, Context& context) {
  const auto commands = extractCommands(line);

  // Skip empty command line
  if (commands.empty()) {
    return Status::EMPTY;
  }

  const auto& command = commands.at(0);
  const Arguments commandArguments(std::next(std::begin(commands)),
                                   std::end(commands));

  // Process command
  switch (commandArguments.size()) {
    case 0:
      if (command == "logout") {
        return processLogout(commandArguments, context);
      } else if (command == "history") {
        return processHistory(commandArguments, context);
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 1:
      if (command == "get") {
        if (context.username.empty()) {
          context.output << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        return processGet({commands.at(1)}, context);
      } else if (command == "withdraw") {
        if (context.username.empty()) {
          context.output << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        // TODO: Handle insufficient amount
        // TODO: Which user?
        processWithdraw({commands.at(1)}, context);

      } else if (command == "deposit") {
        if (context.username.empty()) {
          context.output << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        // TODO: Which user?
        // TODO: When OK?
        return processDeposit({commands.at(1)}, context);
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 2:
      if (command == "login") {
        return processLogin(commands, context);
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 3:
      if (command == "transfer") {
        if (context.username.empty()) {
          context.output << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        const std::string subCommand{commands.at(2)};
        if (subCommand != "to") {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }

        return processTransfer({commands.at(1), commands.at(3)}, context);

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

Balances readBalances(Storage& storage) {
  NullOutput output;
  Context context{storage, output};

  if (!storage) {
    std::cout << "Empty storage" << '\n';
    return {};
  }

  const auto str = readFromStorage(storage);
  std::stringstream archiveStorage{str};

  // Commands loop
  while (archiveStorage.good()) {
    std::string line;
    std::getline(archiveStorage, line);
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
    const Arguments commandArguments(std::next(std::begin(commands)),
                                     std::end(commands));

    // TODO: Set it to use context
    initializeUserBalance(context.username, context.balances);

    // Process command
    switch (commandArguments.size()) {
      case 2:
        if (command == "withdraw") {
          processWithdraw({commands.at(2)}, context);
        } else if (command == "deposit") {
          processDeposit({commands.at(2)}, context);
        }
        break;
      case 4:
        if (command == "transfer") {
          const std::string subCommand{commands.at(3)};
          if (subCommand != "to") {
            printNotSupportedCommand(commands);
            continue;
          }

          const std::string user{commands.at(4)};

          // It is assumed that record is OK
          // and transfer was done to existing user.
          initializeUserBalance(user, context.balances);

          processTransfer({commands.at(2), user}, context);
        }
        break;
    }
  }

  return context.balances;
}

// TODO: Handle initialization failure
void initLoop(Context& context) {
  // TODO: Handle processing of log storage
  if (!context.log.good()) {
    std::cout << "Error opening storage" << '\n';
  }

  context.balances = readBalances(context.log);
}

void processLoop(Context& context, Input& input) {
  bool shouldProcess = true;
  while (shouldProcess) {
    std::string line;
    // TODO: Set command prompt "$ "
    // TODO: Generic setup of input
    std::getline(input, line);

    // TODO: Refactor balance reading
    const auto status = processCommand(line, context);

    const auto result = processStatus(status, context);

    context.output << result;

    if (Status::LOGOUT == status) {
      shouldProcess = false;
    }
  }  // end command loop
}

std::string processStatus(Status status, Context& context) {
  switch (status) {
    case Status::OK:
      return "ok!\n";
      break;
    case Status::NOT_LOGGED:
      return "not logged!\n";
      break;
    case Status::UNKNOWN_USER:
      return "unknown user!\n";
      break;
    case Status::INVALID_ARGUMENTS:
      return "invalid arguments!\n";
      break;
    case Status::EMPTY:
      return "empty command line!\n";
      break;
    case Status::LOGIN:
      return "Welcome, " + context.username + '\n';
    case Status::LOGOUT:
      return "logout!\n";
      break;
    case Status::RESULT:
      return context.result + '\n';
      break;
    case Status::UNKNOWN_COMMAND:
      return "unknown command!\n";
      break;
  }
  return "";
}

}  // namespace sbt