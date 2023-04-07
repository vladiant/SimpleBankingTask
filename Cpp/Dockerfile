FROM alpine:3.13 as build

RUN apk update && apk add --no-cache \
    build-base cmake linux-headers openssl-dev

COPY ./CMakeLists.txt /tmp/src/simple_banking_task/CMakeLists.txt

COPY ./lib /tmp/src/simple_banking_task/lib
COPY ./cli /tmp/src/simple_banking_task/cli
COPY ./http /tmp/src/simple_banking_task/http
COPY ./tcp /tmp/src/simple_banking_task/tcp
COPY ./udp /tmp/src/simple_banking_task/udp
COPY ./test /tmp/src/simple_banking_task/test
COPY ./third_party /tmp/src/simple_banking_task/third_party

WORKDIR /tmp/src/simple_banking_task

RUN mkdir build \
    && cd build \
    && cmake .. -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1 \
    && make \
    && make test \
    && make install

RUN rm -rf /tmp/src

WORKDIR /simple_banking_task

COPY ./entrypoint.sh /simple_banking_task/entrypoint.sh

RUN chmod +x /simple_banking_task/entrypoint.sh

EXPOSE 50013 50014 50015

ENTRYPOINT [ "/simple_banking_task/entrypoint.sh" ]

CMD [ "echo", "Default argument" ]