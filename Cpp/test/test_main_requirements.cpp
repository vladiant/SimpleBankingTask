#include <utest/utest.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include "controller.hpp"
#include "testing_utils.hpp"
#include "utils.hpp"

namespace sbt {

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
  std::unique_ptr<sbt::Storage> storage_;
  std::unique_ptr<std::stringstream> output_;
  std::unique_ptr<sbt::Context> context_;
};

UTEST_F_SETUP(MainRequirementsTest) {
  static_cast<void>(utest_result);
  EXPECT_FALSE(std::filesystem::exists(getPath()));
  utest_fixture->storage_ =
      std::make_unique<std::fstream>(sbt::createFileStorage(getPath()));
  utest_fixture->output_ = std::make_unique<std::stringstream>();
  utest_fixture->context_ = std::make_unique<sbt::Context>(
      *utest_fixture->storage_, *utest_fixture->output_);
}

UTEST_F_TEARDOWN(MainRequirementsTest) {
  static_cast<void>(utest_result);
  static_cast<void>(utest_fixture->context_);
  EXPECT_TRUE(std::filesystem::exists(getPath()));
  EXPECT_TRUE(std::filesystem::remove(getPath()));
}

UTEST_F(MainRequirementsTest, MainRequirements_LoginLogout_Messages) {
  // Arrange
  std::stringstream command_buffer;
  command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                 << kEOC;
  command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;

  initLoop(context);

  // Act
  processLoop(context, command_buffer);

  // Assert
  std::stringstream expectedResponse;
  expectedResponse << kLoginResponseStart << kTestUsername << kEOR;
  expectedResponse << kLogoutResponse << kEOR;
  EXPECT_EQ(utest_fixture->output_->str(), expectedResponse.str());
}

UTEST_F(MainRequirementsTest,
        MainRequirements_LoginGetBalanceLogout_ZeroBalance) {
  // Arrange
  std::stringstream command_buffer;
  command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                 << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;

  initLoop(context);

  // Act
  processLoop(context, command_buffer);

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
  std::stringstream command_buffer;
  command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                 << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kDeposit << kSep << kTestDeposit << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;

  initLoop(context);

  // Act
  processLoop(context, command_buffer);

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
  std::stringstream command_buffer;
  command_buffer << kLogin << kSep << kTestUsername << kSep << kTestPassword
                 << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kDeposit << kSep << kTestDeposit << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kWithdraw << kSep << kTestWithdraw << kEOC;
  command_buffer << kGetBalance << kEOC;
  command_buffer << kLogout << kEOC;

  auto& context = *utest_fixture->context_;

  initLoop(context);

  // Act
  processLoop(context, command_buffer);

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

}  // namespace sbt
