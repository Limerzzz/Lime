/*
 * @Author: Limer
 * @Date: 2022-05-31 13:11:58
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-24 13:53:18
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
#include "util.h"

Connection::Connection(EventLoop* ep, Socket* sock) {
    ep_ = ep;
    chl_ = new Channel(ep, sock->get_fd());
    sock_ = sock;
    statue_ = Connection::Statue::KConnected;
    read_buf_ = new Buffer();
    write_buf_ = new Buffer();

    // std::function<void()> cb =
    //     std::bind(&Connection::echo, this, sock->get_fd());

    chl_->enableReading();
}

Connection::~Connection() { delete sock_; }

void Connection::setDeleteConnCallback(std::function<void(Socket*)> cb) {
    dele_cb_ = cb;
}

void Connection::set_on_cb(std::function<void(Connection*)> cb) {
    on_cb_ = cb;
    chl_->setcallback([this]() { on_cb_(this); });
}

void Connection::ReadMessage() {
    char buf[1024];
    int sockfd = sock_->get_fd();
    read_buf_->clear();
    for (;;) {
        bzero(buf, sizeof(buf));
        ssize_t readbytes = ::read(sockfd, buf, sizeof(buf));
        read_buf_->append(buf);
        if (readbytes > 0) {
            write_buf_->append(buf);
        } else if (readbytes == -1 && errno == EINTR) {
            ::printf("continue reading");
            continue;
        } else if (readbytes == -1 &&
                   ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            ::printf("finish reading once\n");
            break;
        } else if (readbytes == 0) {
            ::printf("EOF, client fd %d disconnected\n", sockfd);
            dele_cb_(sock_);
            break;
        }
    }
}

void Connection::WriteMessage() {
    if (read_buf_->size() == 0) return;
    write_buf_->SetBuffer(read_buf_->c_str());
    int ret = write(sock_->get_fd(), write_buf_->c_str(), write_buf_->size());
    errif(ret == -1, "fail to send message.\n");
}