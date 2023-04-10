#include "queries.hpp"

#include <sstream>

#include "utils.hpp"

namespace sbt {

Status processGet(const Arguments& arguments, Context& context) {
  context.result.clear();
  // TODO: Handle improper arguments size
  const std::string subCommand{arguments.at(0)};
  if (subCommand == "balance") {
    std::stringstream out;
    out << context.balances[context.username];
    context.result = out.str();
  } else {
    printNotSupportedCommand(arguments);
    return Status::UNKNOWN_COMMAND;
  }
  return Status::RESULT;
}

}  // namespace sbt