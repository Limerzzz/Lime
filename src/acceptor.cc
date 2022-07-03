/*
 * @Author: Limer
 * @Date: 2022-05-30 12:29:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-21 14:16:51
 * @Description:
 */
#include "acceptor.h"
#include "channel.h"
#include "event_loop.h"
#include "my_socket.h"

Acceptor::Acceptor(EventLoop* ep) {
    ev_loop_ = ep;
    srv_socket_ = new Socket();
    srv_addr_ = new InetAddr("127.0.0.1", 1234);
    srv_socket_->bind(srv_addr_);
    srv_socket_->listen();
    srv_socket_->setnonblocking();

    srv_chl_ = new Channel(ep, srv_socket_->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::AcceptNewConn, this);
    srv_chl_->setcallback(cb);
    srv_chl_->enableReading();
    srv_chl_->setUseThreadPool(false);
    delete srv_addr_;
}

void Acceptor::AcceptNewConn() { new_conn_callback_(srv_socket_); }

void Acceptor::SetNewConnCallback(std::function<void(Socket*)> cb) {
    new_conn_callback_ = cb;
}

Acceptor::~Acceptor() {
    delete srv_addr_;
    delete srv_socket_;
    delete ev_loop_;
    delete srv_chl_;
}