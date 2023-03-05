#include <httplib.h>

#include <cstddef>
#include <iostream>
#include <list>

#include "controller.hpp"

constexpr auto fileName = "account_history.txt";

constexpr int BANKING_PORT = 50013;

class CommandProcessor {
 public:
  CommandProcessor(const std::string& command, Context& context)
      : mCommand{command}, mContext{context} {}

  void operator()(const httplib::Request& req, httplib::Response& res) {
    std::string line = mCommand;
    for (const auto& elem : req.params) {
      line.append(" ");
      line.append(elem.second);
    }

    const auto status = processCommand(line, fileName, mContext);

    const auto result = processStatus(status, mContext);

    res.set_content(result, "text/plain");
  }

 private:
  std::string mCommand;
  Context& mContext;
};

class CommandRegistrator {
 public:
  CommandRegistrator(Context& context, httplib::Server& server)
      : mContext{context}, mServer{server} {}

  void registerCommand(const std::string& command) {
    mProcessors.emplace_back(command, mContext);
    mServer.Post("/" + command, mProcessors.back());
  }

 private:
  Context& mContext;
  httplib::Server& mServer;
  std::list<CommandProcessor> mProcessors;
};

int main() {
  Context context;
  initLoop(fileName, context);

  httplib::Server svr;
  svr.Post("/stop",
           [&]([[maybe_unused]] const httplib::Request& req,
               [[maybe_unused]] httplib::Response& res) { svr.stop(); });

  CommandRegistrator registrator{context, svr};
  // TODO: Refactor using predefined commands list
  registrator.registerCommand("login");
  registrator.registerCommand("logout");
  registrator.registerCommand("get");
  registrator.registerCommand("withdraw");
  registrator.registerCommand("deposit");
  registrator.registerCommand("transfer");
  registrator.registerCommand("history");

  svr.listen("0.0.0.0", BANKING_PORT);

  return EXIT_SUCCESS;
}