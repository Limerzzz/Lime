/*
 * @Author: Limer
 * @Date: 2022-05-31 13:11:58
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-18 16:02:40
 * @Description:
 */
#include "connection.h"
#include <unistd.h>
#include <functional>
#include "buffer.h"
#include "channel.h"
#include "event_loop.h"
#include "my_socket.h"
#include "string.h"

Connection::Connection(EventLoop* ep, Socket* sock) {
    ep_ = ep;
    chl_ = new Channel(ep, sock->get_fd());
    sock_ = sock;
    readbuf = new Buffer();
    writebuf = new Buffer();
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
        readbuf->append(buf);
        if (readbytes > 0) {
            writebuf->append(buf);
        } else if (readbytes == -1 && errno == EINTR) {
            ::printf("continue reading");
            continue;
        } else if (readbytes == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            ::printf("message from :%d, %s\n", sockfd, readbuf->c_str());
            ::printf("finish reading once\n");
            ::write(sockfd, writebuf->c_str(), writebuf->size() + 1);
            readbuf->clear();
            writebuf->clear();
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