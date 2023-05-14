#include "testing_utils.hpp"

#include <iostream>

#include "types.hpp"

namespace sbt {
void utest_type_printer(const std::string& val) { std::cout << '\n' << val; }

void utest_type_printer(const Status& val) {
  switch (val) {
    case Status::OK:
      std::cout << '\n' << "OK";
      break;
    case Status::EMPTY:
      std::cout << '\n' << "EMPTY";
      break;
    case Status::LOGIN:
      std::cout << '\n' << "LOGIN";
      break;
    case Status::LOGOUT:
      std::cout << '\n' << "LOGOUT";
      break;
    case Status::NOT_LOGGED:
      std::cout << '\n' << "NOT_LOGGED";
      break;
    case Status::RESULT:
      std::cout << '\n' << "RESULT";
      break;
    case Status::UNKNOWN_COMMAND:
      std::cout << '\n' << "UNKNOWN_COMMAND";
      break;
    case Status::UNKNOWN_USER:
      std::cout << '\n' << "UNKNOWN_USER";
      break;
    case Status::INVALID_ARGUMENTS:
      std::cout << '\n' << "INVALID_ARGUMENTS";
      break;
    default:
      std::cout << '\n' << "Unknown enum value: " << static_cast<int64_t>(val);
      break;
  }
}

}  // namespace sbt
