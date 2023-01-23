#include <utest/utest.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include "controller.hpp"

namespace {
constexpr auto kFileName = "test_account_history.txt";

void utest_type_printer(const std::string& val) { std::cout << '\n' << val; }
}  // namespace

struct MainRequirementsTest {
  std::unique_ptr<Context> context_;
  std::shared_ptr<std::stringstream> output_;
};

UTEST_F_SETUP(MainRequirementsTest) {
  static_cast<void>(utest_result);
  utest_fixture->context_ = std::make_unique<Context>();
  utest_fixture->output_ = std::make_shared<std::stringstream>();
  EXPECT_FALSE(std::filesystem::exists(kFileName));
}

UTEST_F_TEARDOWN(MainRequirementsTest) {
  static_cast<void>(utest_result);
  static_cast<void>(utest_fixture->context_);
  EXPECT_TRUE(std::filesystem::exists(kFileName));
  EXPECT_TRUE(std::filesystem::remove(kFileName));
}

UTEST_F(MainRequirementsTest, MainRequirements_LoginLogout_Messages) {
  // Arrange
  auto& context = *utest_fixture->context_;
  context.input =
      std::make_shared<std::stringstream>("login ola 123\nlogout\n");
  context.output = utest_fixture->output_;

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

  // Assert
  EXPECT_EQ(utest_fixture->output_->str(),
            std::string("Welcome, ola\nlogout!\n"));
}

UTEST_F(MainRequirementsTest,
        MainRequirements_LoginGetBalanceLogout_ZeroBalance) {
  // Arrange
  auto& context = *utest_fixture->context_;
  context.input = std::make_shared<std::stringstream>(
      "login ola 123\nget balance\nlogout\n");
  context.output = utest_fixture->output_;

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

  // Assert
  EXPECT_EQ(utest_fixture->output_->str(),
            std::string("Welcome, ola\n0\nlogout!\n"));
}