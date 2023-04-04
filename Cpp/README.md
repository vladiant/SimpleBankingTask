# Example of Simple C++ Banking Service with Docker build

This sample demonstrates how to create a simple banking service in C++ and build it
it using GCC and CMake in a Docker container.

## Build

```sh
docker build -t simple_banking_task .
```

## Run servers

The folder `./task_log` is used to store the account activity file. 

### Run TCP Server

```sh
docker run --rm --network=host --entrypoint /simple_banking_task/TcpBankingServer --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task
```

### Run UDP Server

```sh
docker run --rm --network=host --entrypoint /simple_banking_task/UdpBankingServer --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task
```

### Run HTTP Server

```sh
docker run --rm --network=host --entrypoint /simple_banking_task/HttpBankingServer --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task
```
