/*
 * @Author: Limer
 * @Date: 2022-05-31 13:11:58
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-31 13:48:58
 * @Description:
 */
#include "Connection.h"
#include <unistd.h>
#include <functional>
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "string.h"

Connection::Connection(EventLoop* ep, Socket* sock) {
    ep_ = ep;
    chl_ = new Channel(ep, sock->get_fd());
    sock_ = sock;
    std::function<void()> cb =
        std::bind(&Connection::echo, this, sock->get_fd());
    chl_->setcallback(cb);
    chl_->enableReading();
}

Connection::~Connection() { delete sock_; }

void Connection::echo(int sockfd) {
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
            dele_cb(sock_);
            break;
        }
    }
}

void Connection::setDeleteConnCallback(std::function<void(Socket*)> cb) {
    dele_cb = cb;
}