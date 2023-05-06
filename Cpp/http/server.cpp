#include <httplib.h>

#include <csignal>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <list>
#include <utility>

#include "controller.hpp"
#include "utils.hpp"

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
  CommandProcessor(std::string command, sbt::Context& context)
      : mCommand{std::move(command)}, mContext{context} {}

  void operator()(const httplib::Request& req, httplib::Response& res) {
    std::string line = mCommand;
    for (const auto& elem : req.params) {
      line.append(" ");
      line.append(elem.second);
    }

    const auto status = sbt::processCommand(line, mContext);

    const auto result = sbt::processStatus(status, mContext);

    res.set_content(result, "text/plain");
  }

 private:
  std::string mCommand;
  sbt::Context& mContext;
};

class CommandRegistrator {
 public:
  CommandRegistrator(sbt::Context& context, httplib::Server& server)
      : mContext{context}, mServer{server} {}

  void registerCommand(const std::string& command) {
    mProcessors.emplace_back(command, mContext);
    mServer.Post("/" + command, mProcessors.back());
  }

 private:
  sbt::Context& mContext;
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

  auto storage = sbt::createFileStorage(getPath().string());
  sbt::Context context{storage, std::cout};

  sbt::initLoop(context);

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
