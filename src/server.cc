/*
 * @Author: Limer
 * @Date: 2022-05-26 18:09:08
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-18 16:13:53
 * @Description:
 */
#include "server.h"
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <thread>
#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "event_loop.h"
#include "my_socket.h"
#include "thread_pool.h"

Server::Server(EventLoop* ep) : main_reactor_(ep) {
    acceptor_ = new Acceptor(main_reactor_);
    std::function<void(Socket*)> func =
        std::bind(&Server::newConn, this, std::placeholders::_1);
    acceptor_->SetNewConnCallback(func);

    sub_readctor_size_ = std::thread::hardware_concurrency();
    for (int i = 0; i < sub_readctor_size_; ++i) {
        sub_reactors_.push_back(new EventLoop());
    }
    threadpool_ = new ThreadPool(sub_readctor_size_);
    for (int i = 0; i < sub_readctor_size_; ++i) {
        std::function<void()> func =
            std::bind(&EventLoop::loop, sub_reactors_[i]);
        threadpool_->add(func);
    }
}

Server::~Server() {}

void Server::newConn(Socket* serv_sock) {
    InetAddr* cnt_addr = new InetAddr();
    Socket* cnt_sock = new Socket(serv_sock->accept(cnt_addr));
    cnt_sock->setnonblocking();
    ::printf("new client fd %d! IP: %s Port: %d\n", cnt_sock->get_fd(),
             inet_ntoa(cnt_addr->addr.sin_addr),
             ntohs(cnt_addr->addr.sin_port));
    // random schedule algorithm
    int idx = cnt_sock->get_fd() % sub_readctor_size_;
    Connection* conn = new Connection(sub_reactors_[idx], cnt_sock);
    conns_[cnt_sock->get_fd()] = conn;
    std::function<void(Socket*)> cb =
        std::bind(&Server::deleteConn, this, std::placeholders::_1);
    conn->setDeleteConnCallback(cb);
}

void Server::deleteConn(Socket* sk) {
    close(sk->get_fd());
    auto tmp = conns_[sk->get_fd()];
    conns_.erase(sk->get_fd());
    delete tmp;
}