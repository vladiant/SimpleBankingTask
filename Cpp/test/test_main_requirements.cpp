#include <utest/utest.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include "controller.hpp"
#include "testing_utils.hpp"

using Test::Data::kFileName;
using Test::Data::kTestDeposit;
using Test::Data::kTestPassword;
using Test::Data::kTestUsername;
using Test::Data::kTestWithdraw;

using Test::Commands::kDeposit;
using Test::Commands::kEOC;
using Test::Commands::kGetBalance;
using Test::Commands::kLogin;
using Test::Commands::kLogout;
using Test::Commands::kSep;
using Test::Commands::kWithdraw;

using Test::Responses::kEOR;
using Test::Responses::kLoginResponseStart;
using Test::Responses::kLogoutResponse;
using Test::Responses::kOkResponse;

auto getPath() {
  auto tempPath = std::filesystem::temp_directory_path();
  tempPath /= kFileName;
  return tempPath;
}

struct MainRequirementsTest {
  std::unique_ptr<Context> context_;
  std::shared_ptr<std::stringstream> output_;
};

UTEST_F_SETUP(MainRequirementsTest) {
  static_cast<void>(utest_result);
  utest_fixture->context_ = std::make_unique<Context>();
  utest_fixture->output_ = std::make_shared<std::stringstream>();
  EXPECT_FALSE(std::filesystem::exists(getPath()));
}

UTEST_F_TEARDOWN(MainRequirementsTest) {
  static_cast<void>(utest_result);
  static_cast<void>(utest_fixture->context_);
  EXPECT_TRUE(std::filesystem::exists(getPath()));
  EXPECT_TRUE(std::filesystem::remove(getPath()));
}

UTEST_F(MainRequirementsTest, MainRequirements_LoginLogout_Messages) {
  // Arrange
  auto command_buffer = std::make_shared<std::stringstream>();
  *command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                  << kEOC;
  *command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;
  context.input = command_buffer;
  context.output = utest_fixture->output_;

  initLoop(getPath(), context);

  // Act
  processLoop(getPath(), context);

  // Assert
  std::stringstream expectedResponse;
  expectedResponse << kLoginResponseStart << kTestUsername << kEOR;
  expectedResponse << kLogoutResponse << kEOR;
  EXPECT_EQ(utest_fixture->output_->str(), expectedResponse.str());
}

UTEST_F(MainRequirementsTest,
        MainRequirements_LoginGetBalanceLogout_ZeroBalance) {
  // Arrange
  auto command_buffer = std::make_shared<std::stringstream>();
  *command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                  << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;
  context.input = command_buffer;
  context.output = utest_fixture->output_;

  initLoop(getPath(), context);

  // Act
  processLoop(getPath(), context);

  // Assert
  std::stringstream expectedResponse;
  expectedResponse << kLoginResponseStart << kTestUsername << kEOR;
  expectedResponse << 0 << kEOR;
  expectedResponse << kLogoutResponse << kEOR;
  EXPECT_EQ(utest_fixture->output_->str(), expectedResponse.str());
}

UTEST_F(MainRequirementsTest,
        MainRequirements_LoginDepositGetBalanceLogout_DepositedBalance) {
  // Arrange
  auto command_buffer = std::make_shared<std::stringstream>();
  *command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                  << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kDeposit << kSep << kTestDeposit << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;
  context.input = command_buffer;
  context.output = utest_fixture->output_;

  initLoop(getPath(), context);

  // Act
  processLoop(getPath(), context);

  // Assert
  std::stringstream expectedResponse;
  expectedResponse << kLoginResponseStart << kTestUsername << kEOR;
  expectedResponse << 0 << kEOR;
  expectedResponse << kOkResponse << kEOR;
  expectedResponse << kTestDeposit << kEOR;
  expectedResponse << kLogoutResponse << kEOR;
  EXPECT_EQ(utest_fixture->output_->str(), expectedResponse.str());
}

UTEST_F(MainRequirementsTest,
        MainRequirements_LoginDepositWithdrawGetBalanceLogout_TotalBalance) {
  // Arrange
  auto command_buffer = std::make_shared<std::stringstream>();
  *command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                  << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kDeposit << kSep << kTestDeposit << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kWithdraw << kSep << kTestWithdraw << kEOC;
  *command_buffer << kGetBalance << kEOC;
  *command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;
  context.input = command_buffer;
  context.output = utest_fixture->output_;

  initLoop(getPath(), context);

  // Act
  processLoop(getPath(), context);

  // Assert
  std::stringstream expectedResponse;
  expectedResponse << kLoginResponseStart << kTestUsername << kEOR;
  expectedResponse << 0 << kEOR;
  expectedResponse << kOkResponse << kEOR;
  expectedResponse << kTestDeposit << kEOR;
  expectedResponse << kOkResponse << kEOR;
  expectedResponse << kTestDeposit - kTestWithdraw << kEOR;
  expectedResponse << kLogoutResponse << kEOR;
  EXPECT_EQ(utest_fixture->output_->str(), expectedResponse.str());
}