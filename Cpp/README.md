# Example of Simple C++ Banking Service with Docker build

This sample demonstrates how to create a simple banking service in C++ and build it
it using GCC and CMake in a Docker container.

## Build

```sh
docker build -t simple_banking_task .
```

# Run TCP Server

```sh
docker run --rm -it --network=host --entrypoint /simple_banking_task/TcpBankingServer simple_banking_task
```

# Run UDP Server

```sh
docker run --rm -it --network=host --entrypoint /simple_banking_task/UdpBankingServer simple_banking_task
```

# Run HTTP Server

```sh
docker run --rm -it --network=host --entrypoint /simple_banking_task/HttpBankingServer simple_banking_task
```