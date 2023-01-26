#include <utest/utest.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include "controller.hpp"

namespace {

constexpr auto kFileName = "test_account_history.txt";

constexpr auto kLogin = "login";
constexpr auto kLogout = "logout";
constexpr auto kGetBalance = "get balance";
constexpr auto kDeposit = "deposit";
constexpr auto kWithdraw = "withdraw";

// Separator
constexpr auto kSep{" "};
// End of Command
constexpr auto kEOC{"\n"};

constexpr auto kTestUsername = "ola";
constexpr auto kTestPassword = "123";
constexpr int kTestDeposit = 100;
constexpr int kTestWithdraw = 25;

constexpr auto kLoginResponseStart = "Welcome, ";
constexpr auto kLogoutResponse = "logout!";
constexpr auto kOkResponse = "ok!";
// End of Response
constexpr auto kEOR{"\n"};

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
  auto command_buffer = std::make_shared<std::stringstream>();
  *command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                  << kEOC;
  *command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;
  context.input = command_buffer;
  context.output = utest_fixture->output_;

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

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

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

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

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

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

  initLoop(kFileName, context);

  // Act
  processLoop(kFileName, context);

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