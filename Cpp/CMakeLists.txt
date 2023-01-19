cmake_minimum_required(VERSION 3.10)

project(TaskCpp)

# Library
add_library(
    BankingLib
    lib/include/types.hpp
    lib/include/commands.hpp
    lib/include/controller.hpp
    lib/include/utils.hpp
    lib/src/commands.cpp
    lib/src/utils.cpp
    lib/src/controller.cpp
)

set_target_properties(
    BankingLib
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINKER_LANGUAGE CXX
)

target_compile_options(
    BankingLib
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    BankingLib
    PUBLIC
        lib/include
)

# CLI app
add_executable(
    CliBanking
    cli/main.cpp
)

set_target_properties(
    CliBanking
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    CliBanking
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(
    CliBanking
    PRIVATE
        BankingLib
)

# Tests
include(CTest)

add_executable(
    TestBankingLib
    test/test_main_requirements.cpp
    test/main.cpp
)

set_target_properties(
    TestBankingLib
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(
    TestBankingLib
    PRIVATE
        test
)

target_link_libraries(
    TestBankingLib
    PRIVATE
        BankingLib
)

add_test(NAME TestBankingLib COMMAND TestBankingLib)