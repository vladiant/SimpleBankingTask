#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using BalanceType = float;
using Balances = std::map<std::string, BalanceType>;

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

enum class Status {
  EMPTY,
  UNKNOWN_COMMAND,
  UNKNOWN_USER,
  LOGOUT,
  NOT_LOGGED,
  OK
};

Status processCommand(const std::string& line, std::fstream& logFile,
                      std::string& username, Balances& balances) {
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
        logFile << username << " "
                << "logout" << '\n';
        username.clear();
        return Status::LOGOUT;
      } else if (command == "history") {
        // Flush existing data
        logFile.flush();
        std::fstream historyFile(fileName, std::ios_base::in);
        while (historyFile) {
          std::string log;
          std::getline(historyFile, log);
          if (log.empty()) {
            break;
          }
          std::cout << log << '\n';
        }
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 2:
      if (command == "get") {
        const std::string subCommand{commands[1]};
        if (subCommand == "balance") {
          if (username.empty()) {
            std::cout << "No user logged!\n";
            return Status::NOT_LOGGED;
          }
          std::cout << balances[username] << '\n';
        } else {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }
      } else if (command == "withdraw") {
        if (username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        // TODO: Handle insufficient amount
        // TODO: Which user?
        std::stringstream ss(commands[1]);
        BalanceType amount;
        ss >> amount;
        balances[username] -= amount;
        logFile << username << " "
                << "withdraw " << amount << '\n';
      } else if (command == "deposit") {
        if (username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        std::stringstream ss(commands[1]);
        BalanceType amount;
        ss >> amount;
        balances[username] += amount;
        // TODO: Which user?
        // TODO: When OK?
        logFile << username << " "
                << "deposit " << amount << '\n';
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 3:
      if (command == "login") {
        const std::string password{commands[2]};
        // TODO: Handle password check
        username = commands[1];
        initializeUserBalance(username, balances);
        std::cout << "Welcome, " << username << '\n';
        logFile << username << " "
                << "login " << username << " " << password << '\n';
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN_COMMAND;
      }
      break;
    case 4:
      if (command == "transfer") {
        if (username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        const std::string subCommand{commands[2]};
        if (subCommand != "to") {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN_COMMAND;
        }
        std::stringstream ss(commands[1]);
        BalanceType amount;
        ss >> amount;
        const std::string user{commands[3]};
        // TODO: Handle insufficient balance

        // Self transfer
        if (username == user) {
          // Ignore self transfer, no problem
          return Status::OK;
        }

        if (balances.find(user) == balances.end()) {
          return Status::UNKNOWN_USER;
        }

        balances[username] -= amount;
        balances[user] += amount;

        logFile << username << " "
                << "transfer " << amount << " " << subCommand << " " << user
                << '\n';
        std::cout << username << " "
                  << "transfer " << amount << " " << subCommand << " " << user
                  << '\n';

        // TODO: Is transfer a deposit
        // TODO: When OK? Should user exist?
        logFile << user << " "
                << "deposit " << amount << '\n';
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
  std::map<std::string, BalanceType> balances;

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
    const auto& currentUsername = commands.at(0);
    const auto& command = commands.at(1);

    initializeUserBalance(currentUsername, balances);

    // Process command
    switch (commands.size()) {
      case 3:
        if (command == "withdraw") {
          std::stringstream ss(commands[2]);
          BalanceType amount;
          ss >> amount;
          balances[currentUsername] -= amount;
        } else if (command == "deposit") {
          std::stringstream ss(commands[2]);
          BalanceType amount;
          ss >> amount;
          balances[currentUsername] += amount;
        }
        break;
      case 5:
        if (command == "transfer") {
          const std::string subCommand{commands[3]};
          if (subCommand != "to") {
            printNotSupportedCommand(commands);
            continue;
          }
          std::stringstream ss(commands[2]);
          BalanceType amount;
          ss >> amount;
          const std::string user{commands[4]};

          // It is assumed that record is OK
          // and transfer was done to existing user.
          initializeUserBalance(user, balances);

          balances[currentUsername] -= amount;
          balances[user] += amount;
        }
        break;
    }
  }

  return balances;
}

int main(int, char**) {
  // TODO: Handle processing of file
  std::fstream logFile(
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  if (!logFile.is_open()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  // Consider empty name as no logged user
  std::string username{};

  auto balances = readBallances(fileName);

  // Commands loop
  while (true) {
    std::string line;
    // Command prompt
    std::cout << "$ ";
    std::getline(std::cin, line);

    // TODO: Refactor balance reading
    const auto status = processCommand(line, logFile, username, balances);

    if (Status::LOGOUT == status) {
      break;
    }

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
    }

  }  // end command loop

  return EXIT_SUCCESS;
}
