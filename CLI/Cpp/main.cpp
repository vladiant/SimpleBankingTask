#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using BalanceType = float;

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

enum class Status { EMPTY, UNKNOWN, LOGOUT, NOT_LOGGED, OK };

Status processCommand(const std::string& line, std::fstream& logFile,
                      std::string& username, BalanceType& balance) {
  // Extract commands from line
  std::istringstream inputLine{line};
  std::vector<std::string> commands;
  for (std::string command; std::getline(inputLine, command, ' ');) {
    if (command.empty()) {
      continue;
    }

    commands.emplace_back(std::move(command));
  }

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
        // TODO: Flush current file?
        std::fstream historyFile(fileName, std::ios_base::in);
        while (historyFile) {
          std::string log;
          std::getline(historyFile, log);
          std::cout << log << '\n';
        }
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN;
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
          std::cout << balance << '\n';
        } else {
          printNotSupportedCommand(commands);
          return Status::UNKNOWN;
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
        balance -= amount;
        logFile << username << " "
                << "withdraw " << amount << '\n';
        std::cout << "ok!\n";
      } else if (command == "deposit") {
        if (username.empty()) {
          std::cout << "No user logged!\n";
          return Status::NOT_LOGGED;
        }
        std::stringstream ss(commands[1]);
        BalanceType amount;
        ss >> amount;
        balance += amount;
        // TODO: Which user?
        // TODO: When OK?
        logFile << username << " "
                << "deposit " << amount << '\n';
        std::cout << "ok!\n";
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN;
      }
      break;
    case 3:
      if (command == "login") {
        const std::string password{commands[2]};
        // TODO: Handle password check
        username = commands[1];
        std::cout << "Welcome, " << username << '\n';
        logFile << username << " "
                << "login " << username << " " << password << '\n';
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN;
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
          return Status::UNKNOWN;
        }
        std::stringstream ss(commands[1]);
        BalanceType amount;
        ss >> amount;
        const std::string user{commands[3]};
        // TODO: Handle insufficient balance
        // TODO: Handle missing user(s)
        logFile << username << " "
                << "transfer " << amount << " " << subCommand << " " << user
                << '\n';
        std::cout << username << " "
                  << "transfer " << amount << " " << subCommand << " " << user
                  << '\n';
      } else {
        printNotSupportedCommand(commands);
        return Status::UNKNOWN;
      }
      break;
    default:
      printNotSupportedCommand(commands);
      return Status::UNKNOWN;
  }

  return Status::OK;
}

int main(int, char**) {
  // TODO: Handle processing of file
  std::fstream logFile(
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  if (!logFile.is_open()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  // TODO: Read balance from file
  // TODO: Handle diffrent user
  BalanceType balance = 0;
  // Consider empty name as no logged user
  std::string username{};

  // Commands loop
  while (true) {
    std::string line;
    // Command prompt
    std::cout << "$ ";
    std::getline(std::cin, line);

    const auto status = processCommand(line, logFile, username, balance);

    if (Status::LOGOUT == status) {
      break;
    }

    // TODO: Process different statuses

  }  // end command loop

  return EXIT_SUCCESS;
}
