#pragma once

#include <string>

namespace sbt {

void utest_type_printer(const std::string& val);

namespace Test {

namespace Data {

static constexpr auto kFileName = "test_account_history.txt";
static constexpr auto kTestUsername = "ola";
static constexpr auto kTestPassword = "123";
static constexpr int kTestDeposit = 100;
static constexpr int kTestWithdraw = 25;

}  // namespace Data

namespace Commands {

static constexpr auto kLogin = "login";
static constexpr auto kLogout = "logout";
static constexpr auto kGetBalance = "get balance";
static constexpr auto kDeposit = "deposit";
static constexpr auto kWithdraw = "withdraw";
// Separator
static constexpr auto kSep = " ";
// End of Command
static constexpr auto kEOC = "\n";

}  // namespace Commands

namespace Responses {

static constexpr auto kLoginResponseStart = "Welcome, ";
static constexpr auto kLogoutResponse = "logout!";
static constexpr auto kOkResponse = "ok!";
// End of Response
static constexpr auto kEOR{"\n"};

}  // namespace Responses

}  // namespace Test

}  // namespace sbt
