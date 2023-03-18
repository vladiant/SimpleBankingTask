FROM alpine:3.12 as build

RUN apk update && apk add --no-cache \
    build-base cmake linux-headers openssl-dev


COPY ./CMakeLists.txt /usr/local/src/simple_banking_task/CMakeLists.txt

COPY ./lib /usr/local/src/simple_banking_task/lib
COPY ./cli /usr/local/src/simple_banking_task/cli
COPY ./http /usr/local/src/simple_banking_task/http
COPY ./tcp /usr/local/src/simple_banking_task/tcp
COPY ./udp /usr/local/src/simple_banking_task/udp
COPY ./test /usr/local/src/simple_banking_task/test
COPY ./third_party /usr/local/src/simple_banking_task/third_party

WORKDIR /usr/local/src/simple_banking_task

RUN mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && make test \
    && make install

EXPOSE 50013 50014 50015