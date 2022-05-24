/*
 * @Author: Limer
 * @Date: 2022-05-24 12:36:48
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-24 20:02:45
 * @Description:
 */
#include "Socket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include "include/Socket.h"
#include "string.h"
#include "util.h"

Socket::Socket() {
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "fail to create a socket!\n");
}

Socket::Socket(int sock) : sockfd(sock) {}

void Socket::bind(InetAddr* addr) {
    int ret = ::bind(sockfd, (sockaddr*)&addr->addr, addr->addr_size);
    errif(ret == -1, "fail to bind a sockaddr!\n");
}

void Socket::listen() {
    int ret = ::listen(sockfd, 1024);
    errif(ret == -1, "fail to listen a socket!\n");
}

void Socket::connect(InetAddr* addr) {
    int ret = ::connect(sockfd, (sockaddr*)&addr->addr, addr->addr_size);
    errif(ret == -1, "fail to connect a socket!\n");
}

Socket Socket::accept() {
    InetAddr cli_addr;
    int cli_sock =
        ::accept(sockfd, (sockaddr*)&cli_addr.addr, &cli_addr.addr_size);
    errif(cli_sock == -1, "fail to accept a sock!\n");
    return Socket(cli_sock);
}

int Socket::get_fd() { return sockfd; }

void Socket::close() { ::close(sockfd); }

void Socket::setnonblocking() {
    ::fcntl(sockfd, ::fcntl(sockfd, F_GETFL) | O_NONBLOCK);
}

InetAddr::InetAddr() {
    addr_size = sizeof(addr);
    ::bzero(&addr, addr_size);
}

InetAddr::InetAddr(const char* ip, int port) {
    addr_size = sizeof(addr);
    bzero(&addr, addr_size);
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = ::htons(port);
    addr.sin_family = AF_INET;
}