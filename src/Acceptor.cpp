/*
 * @Author: Limer
 * @Date: 2022-05-30 12:29:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:11:51
 * @Description:
 */
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* ep) {
    ev_loop = ep;
    srv_sock = new Socket();
    srv_addr = new InetAddr("127.0.0.1", 1234);
    srv_sock->bind(srv_addr);
    srv_sock->listen();
    srv_sock->setnonblocking();

    srv_chl = new Channel(ep, srv_sock->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::AcceptNewConn, this);
    srv_chl->setcallback(cb);
    srv_chl->enableReading();
}

void Acceptor::AcceptNewConn() { cb(srv_sock); }

void Acceptor::SetNewConnCallback(std::function<void(Socket*)> cb_) {
    cb = cb_;
}

Acceptor::~Acceptor() {
    delete srv_addr;
    delete srv_sock;
    delete ev_loop;
    delete srv_chl;
}