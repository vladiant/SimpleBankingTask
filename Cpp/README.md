# Example of Simple C++ Banking Service with Docker build

This sample demonstrates how to create a simple banking service in C++ and build it
it using GCC and CMake in a Docker container.

## Build

```sh
docker build -t simple_banking_task .
```

## Run servers

The folder `./task_log` is used to store the account activity file. 

### TCP Server

```sh
docker run --rm -it --network=host  --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task TcpBankingServer
```

### UDP Server

```sh
docker run --rm -it --network=host  --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task UdpBankingServer
```

### HTTP Server

```sh
docker run --rm -it --network=host  --mount src="$(pwd)"/task_log,target=/simple_banking_task/log,type=bind simple_banking_task HttpBankingServer
```

## Clients

### TCP Client

```sh
docker run --rm -it --network=host -it simple_banking_task TcpBankingClient 0.0.0.0
```

### UDP Client

```sh
docker run --rm -it --network=host -it simple_banking_task UdpBankingClient 0.0.0.0
```

### HTTP Client

```sh
docker run --rm -it --network=host -it simple_banking_task HttpBankingClient 0.0.0.0
```

## Debug

```sh
docker run --rm -it --network=host --entrypoint /bin/sh simple_banking_task
```
