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

  // Commands loop
  while (true) {
    std::string line;
    std::getline(std::cin, line);

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
      continue;
    }

    const auto& command = commands.at(0);

    // Process command
    switch (commands.size()) {
      case 1:
        if (command == "logout") {
          // TODO: Handle commands after logouts
          logFile << "logout" << '\n';
          return EXIT_SUCCESS;
        } else if (command == "history") {
          std::fstream historyFile(fileName, std::ios_base::in);
          while (historyFile) {
            std::string log;
            std::getline(historyFile, log);
            std::cout << log << '\n';
          }
        } else {
          printNotSupportedCommand(commands);
          continue;
        }
        break;
      case 2:  // get balance, withdraw [amount], deposit [amount]
        if (command == "get") {
          const std::string subCommand{commands[1]};
          if (subCommand == "balance") {
            std::cout << balance << '\n';
          } else {
            printNotSupportedCommand(commands);
            continue;
          }
        } else if (command == "withdraw") {
          // TODO: Handle insufficient amount
          // TODO: Which user?
          std::stringstream ss(commands[1]);
          BalanceType amount;
          ss >> amount;
          balance -= amount;
          logFile << "withdraw " << amount << '\n';
        } else if (command == "deposit") {
          std::stringstream ss(commands[1]);
          BalanceType amount;
          ss >> amount;
          balance += amount;
          // TODO: Which user?
          // TODO: When OK?
          logFile << "deposit " << amount << '\n';
          std::cout << "ok!\n";
        } else {
          printNotSupportedCommand(commands);
          continue;
        }
        break;
      case 3:
        if (command == "login") {
          const std::string username{commands[1]};
          const std::string password{commands[2]};
          // TODO: Handle password check
          std::cout << "Welcome, " << username << '\n';
          logFile << "login " << username << " " << password << '\n';
        } else {
          printNotSupportedCommand(commands);
          continue;
        }
        break;
      case 4:  // transfer [amount] to [user]
        if (command == "transfer") {
          const std::string subCommand{commands[2]};
          if (subCommand != "to") {
            printNotSupportedCommand(commands);
            continue;
          }
          std::stringstream ss(commands[1]);
          BalanceType amount;
          ss >> amount;
          const std::string user{commands[3]};
          // TODO: Handle insufficient balance
          // TODO: Handle missing user(s)
          logFile << "transfer " << amount << " " << subCommand << " " << user
                  << '\n';
          std::cout << "transfer " << amount << " " << subCommand << " " << user
                    << '\n';
        } else {
          printNotSupportedCommand(commands);
          continue;
        }
        break;
      default:
        printNotSupportedCommand(commands);
        continue;
    }

  }  // end command loop

  return EXIT_SUCCESS;
}
