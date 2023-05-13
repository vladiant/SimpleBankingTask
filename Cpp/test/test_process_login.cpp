#include <utest/utest.h>

#include <sstream>

#include "commands.hpp"
#include "testing_utils.hpp"

namespace sbt {

UTEST(LoginCommandTest, UserPasswordSet_Login) {
  // Arrange
  Arguments arguments = {" ", "user", "password"};
  std::stringstream log;
  std::stringstream out;

  Context context{log, out};

  // Act
  const auto result = processLogin(arguments, context);

  // Assert
  EXPECT_EQ(Status::LOGIN, result);
}

}  // namespace sbt