cmake_minimum_required(VERSION 3.10)

project(TaskCpp)

# Library
add_library(
    BankingLib
    lib/include/null_output.hpp
    lib/include/types.hpp
    lib/include/commands.hpp
    lib/include/queries.hpp
    lib/include/controller.hpp
    lib/include/utils.hpp
    lib/src/commands.cpp
    lib/src/queries.cpp
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
    test/testing_utils.hpp
    test/testing_utils.cpp
    test/test_main_requirements.cpp
    test/test_process_login.cpp
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
        third_party
)

target_link_libraries(
    TestBankingLib
    PRIVATE
        BankingLib
)

add_test(NAME TestBankingLib COMMAND TestBankingLib)

# TCP server
find_package(Threads REQUIRED)

add_executable(
    TcpBankingServer
    tcp/server.cpp
)

set_target_properties(
    TcpBankingServer
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    TcpBankingServer
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    TcpBankingServer 
    PUBLIC 
      third_party/asio
)

target_link_libraries(
    TcpBankingServer
    PRIVATE
        Threads::Threads
        BankingLib
)

# TCP sample client
add_executable(
    TcpBankingClient
    tcp/client.cpp
)

set_target_properties(
    TcpBankingClient
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    TcpBankingClient
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    TcpBankingClient 
    PUBLIC 
      third_party/asio
)

target_link_libraries(
    TcpBankingClient
    PRIVATE
        Threads::Threads
)

# UDP server
find_package(Threads REQUIRED)

add_executable(
    UdpBankingServer
    udp/server.cpp
)

set_target_properties(
    UdpBankingServer
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    UdpBankingServer
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    UdpBankingServer 
    PUBLIC 
      third_party/asio
)

target_link_libraries(
    UdpBankingServer
    PRIVATE
        Threads::Threads
        BankingLib
)

# UDP sample client
add_executable(
    UdpBankingClient
    udp/client.cpp
)

set_target_properties(
    UdpBankingClient
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    UdpBankingClient
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    UdpBankingClient 
    PUBLIC 
      third_party/asio
)

target_link_libraries(
    UdpBankingClient
    PRIVATE
        Threads::Threads
)

# HTTP dependency
find_package(OpenSSL REQUIRED)

# HTTP client
add_executable(
    HttpBankingClient
    http/client.cpp
)

set_target_properties(
    HttpBankingClient
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    HttpBankingClient
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    HttpBankingClient 
    PUBLIC 
      third_party/cpp-httplib
)

target_link_libraries(
    HttpBankingClient
    PRIVATE
        OpenSSL::SSL
)

# HTTP server
add_executable(
    HttpBankingServer
    http/server.cpp
)

set_target_properties(
    HttpBankingServer
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    HttpBankingServer
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(
    HttpBankingServer 
    PUBLIC 
      third_party/cpp-httplib
)

target_link_libraries(
    HttpBankingServer
    PRIVATE
        OpenSSL::SSL
        Threads::Threads
        BankingLib
)

# Package generation
install(
    TARGETS
       BankingLib
       CliBanking
       TestBankingLib
       TcpBankingServer
       TcpBankingClient
       UdpBankingServer
       UdpBankingClient
       HttpBankingServer
       HttpBankingClient
    DESTINATION
       /simple_banking_task
)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION "Simple Banking C++ Task")
set(CPACK_GENERATOR "DEB") # Debian package
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Code Author") # mandatory

include(CPack)