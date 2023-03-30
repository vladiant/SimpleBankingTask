#include <httplib.h>

#include <csignal>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <list>

#include "controller.hpp"

constexpr auto kLogFolder = "log";
constexpr auto kFileName = "account_history.txt";

constexpr int BANKING_PORT = 50015;

httplib::Server svr;

auto getPath() {
  std::filesystem::path tempPath{kLogFolder};
  tempPath /= kFileName;
  return tempPath;
}

void signalHandler([[maybe_unused]] int signal) { svr.stop(); }

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

    const auto status = processCommand(line, getPath().string(), mContext);

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
  if (!std::filesystem::exists(kLogFolder)) {
    std::filesystem::create_directory(kLogFolder);
  }

  std::signal(SIGINT, signalHandler);
  // K8s signals
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGKILL, signalHandler);
  // K8s optional signal
  std::signal(SIGQUIT, signalHandler);

  Context context;
  initLoop(getPath().string(), context);

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
