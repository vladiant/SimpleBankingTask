#include <fstream>
#include <iostream>
#include <sstream>

using BalanceType = float;

constexpr auto fileName = "account_history.txt";

void printCommand(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    std::cout << argv[i] << " ";
  }
  std::cout << '\n';
}

void printNotSupportedCommand(int argc, char** argv) {
  std::cout << "Command not supported: ";
  printCommand(argc, argv);
}

int main(int argc, char** argv) {
  // TODO: Handle processing of file
  std::fstream logFile(
      fileName, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  if (!logFile.is_open()) {
    std::cout << "Error opening " << fileName << '\n';
  }

  // TODO: Read balance from file
  // TODO: Handle diffrent user
  BalanceType balance = 0;

  const std::string command = argc > 1 ? argv[1] : "";

  // Process command
  switch (argc) {
    case 2:
      if (command == "logout") {
        // TODO: Handle commands after logouts
        logFile << "logout" << '\n';
      } else if (command == "history") {
        std::fstream historyFile(fileName, std::ios_base::in);
        while (historyFile) {
          std::string log;
          std::getline(historyFile, log);
          std::cout << log << '\n';
        }
      } else {
        printNotSupportedCommand(argc, argv);
        return EXIT_FAILURE;
      }
      break;
    case 3:  // get balance, withdraw [amount], deposit [amount]
      if (command == "get") {
        const std::string subCommand{argv[2]};
        if (subCommand == "balance") {
          std::cout << balance << '\n';
        } else {
          printNotSupportedCommand(argc, argv);
          return EXIT_FAILURE;
        }
      } else if (command == "withdraw") {
        // TODO: Handle insufficient amount
        // TODO: Which user?
        std::stringstream ss(argv[2]);
        BalanceType amount;
        ss >> amount;
        balance -= amount;
        logFile << "withdraw " << amount << '\n';
      } else if (command == "deposit") {
        std::stringstream ss(argv[2]);
        BalanceType amount;
        ss >> amount;
        balance += amount;
        // TODO: Which user?
        // TODO: When OK?
        logFile << "deposit " << amount << '\n';
        std::cout << "ok!\n";
      } else {
        printNotSupportedCommand(argc, argv);
        return EXIT_FAILURE;
      }
      break;
    case 4:
      if (command == "login") {
        const std::string username{argv[2]};
        const std::string password{argv[3]};
        // TODO: Handle password check
        std::cout << "Welcome, " << username << '\n';
        logFile << "login " << username << " " << password << '\n';
      } else {
        printNotSupportedCommand(argc, argv);
        return EXIT_FAILURE;
      }
      break;
    case 5:  // transfer [amount] to [user]
      if (command == "transfer") {
        const std::string subCommand{argv[3]};
        if (subCommand != "to") {
          printNotSupportedCommand(argc, argv);
          return EXIT_FAILURE;
        }
        std::stringstream ss(argv[2]);
        BalanceType amount;
        ss >> amount;
        const std::string user{argv[4]};
        // TODO: Handle insufficient balance
        // TODO: Handle missing user(s)
        logFile << "transfer " << amount << " " << subCommand << " " << user
                << '\n';
        std::cout << "transfer " << amount << " " << subCommand << " " << user
                  << '\n';
      } else {
        printNotSupportedCommand(argc, argv);
        return EXIT_FAILURE;
      }
      break;
    default:
      printNotSupportedCommand(argc, argv);
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
