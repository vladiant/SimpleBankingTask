#include "controller.hpp"

#include <fstream>
#include <iostream>

#include "commands.hpp"
#include "utils.hpp"

// TODO: Fix usage of filename - in context
Status processCommand(const std::string& line, const std::string& fileName,
                      Context& context) {
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
        processLogout({}, context);
        return Status::LOGOUT;
      } else if (command == "history") {
        processHistory({fileName}, context);
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
            if (context.output) {
              *context.output << "No user logged!\n";
            }
            return Status::NOT_LOGGED;
          }
          if (context.output) {
            *context.output << context.balances[context.username] << '\n';
          }
        } else {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }
      } else if (command == "withdraw") {
        if (context.username.empty()) {
          if (context.output) {
            *context.output << "No user logged!\n";
          }
          return Status::NOT_LOGGED;
        }
        // TODO: Handle insufficient amount
        // TODO: Which user?
        processWithdraw({commands[1]}, context);

      } else if (command == "deposit") {
        if (context.username.empty()) {
          if (context.output) {
            *context.output << "No user logged!\n";
          }
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
        processLogin({commands[1], commands[2]}, context);
        if (context.output) {
          *context.output << "Welcome, " << context.username << '\n';
        }
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 4:
      if (command == "transfer") {
        if (context.username.empty()) {
          if (context.output) {
            *context.output << "No user logged!\n";
          }
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

Balances readBallances(const std::string& fileName) {
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

// TODO: Fix usage of filename - in context
// TODO: Haindle initialization failure
void initLoop(const std::string& fileName, Context& context) {
  // TODO: Handle processing of file
  context.logFile = std::make_shared<std::fstream>(
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  if (!context.logFile->good()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  context.balances = readBallances(fileName);
}

// TODO: Fix usage of filename - in context
void processLoop(const std::string& fileName, Context& context) {
  bool shouldProcess = true;
  while (shouldProcess) {
    std::string line;
    // TODO: Generic setup of input
    // Command prompt
    std::cout << "$ ";
    // TODO: Generic setup of input
    // TODO: Check if valid
    std::getline(*context.input, line);

    // TODO: Refactor balance reading
    const auto status = processCommand(line, fileName, context);

    // Status process
    switch (status) {
      case Status::OK:
        if (context.output) {
          *context.output << "ok!\n";
        }
        break;
      case Status::NOT_LOGGED:
        if (context.output) {
          *context.output << "not logged!\n";
        }
        break;
      case Status::UNKNOWN_USER:
        if (context.output) {
          *context.output << "unknown user!\n";
        }
        break;
      case Status::EMPTY:
        if (context.output) {
          *context.output << "empty command line!\n";
        }
        break;
      case Status::LOGOUT:
        if (context.output) {
          *context.output << "logout!\n";
        }
        shouldProcess = false;
        break;
      case Status::UNKNOWN_COMMAND:
        if (context.output) {
          *context.output << "unknown command!\n";
        }
        break;
    }

  }  // end command loop
}