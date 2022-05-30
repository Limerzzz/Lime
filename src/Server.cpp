/*
 * @Author: Limer
 * @Date: 2022-05-26 18:09:08
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:13:00
 * @Description:
 */
#include "Server.h"
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Server::Server(EventLoop* ep_) : ep(ep_) {
    acpt = new Acceptor(ep_);
    std::function<void(Socket*)> cb =
        std::bind(&Server::newConn, this, std::placeholders::_1);
    acpt->SetNewConnCallback(cb);
}

Server::~Server() {
    delete ep;
    delete acpt;
}

void Server::newConn(Socket* serv_sock) {
    InetAddr* cnt_addr = new InetAddr();
    Socket* cnt_sock = new Socket(serv_sock->accept(cnt_addr));
    cnt_sock->setnonblocking();
    ::printf("new client fd %d! IP: %s Port: %d\n", cnt_sock->get_fd(),
             inet_ntoa(cnt_addr->addr.sin_addr),
             ntohs(cnt_addr->addr.sin_port));
    Channel* chl = new Channel(ep, cnt_sock->get_fd());
    std::function<void()> cb =
        std::bind(&Server::readMess, this, cnt_sock->get_fd());
    chl->setcallback(cb);
    chl->enableReading();
}

void Server::readMess(int sockfd) {
    char buf[1024];
    for (;;) {
        bzero(buf, sizeof(buf));
        ssize_t readbytes = ::read(sockfd, buf, sizeof(buf));
        if (readbytes > 0) {
            ::printf("message from client fd %d : %s\n", sockfd, buf);
            write(sockfd, buf, ::strlen(buf) + 1);
        } else if (readbytes == -1 && errno == EINTR) {
            ::printf("continue reading");
            continue;
        } else if (readbytes == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            ::printf("finish reading once\n");
            break;
        } else if (readbytes == 0) {
            ::printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}