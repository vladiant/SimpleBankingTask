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
  // Consider empty name as no logged user
  std::string username{};

  // Commands loop
  while (true) {
    std::string line;
    // Command prompt
    std::cout << "$ ";
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
          logFile << username << " "
                  << "logout" << '\n';
          username.clear();
          return EXIT_SUCCESS;
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
          continue;
        }
        break;
      case 2:
        if (command == "get") {
          const std::string subCommand{commands[1]};
          if (subCommand == "balance") {
            if (username.empty()) {
              std::cout << "No user logged!\n";
              continue;
            }
            std::cout << balance << '\n';
          } else {
            printNotSupportedCommand(commands);
            continue;
          }
        } else if (command == "withdraw") {
          if (username.empty()) {
            std::cout << "No user logged!\n";
            continue;
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
            continue;
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
          continue;
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
          continue;
        }
        break;
      case 4:
        if (command == "transfer") {
          if (username.empty()) {
            std::cout << "No user logged!\n";
            continue;
          }
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
          logFile << username << " "
                  << "transfer " << amount << " " << subCommand << " " << user
                  << '\n';
          std::cout << username << " "
                    << "transfer " << amount << " " << subCommand << " " << user
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
