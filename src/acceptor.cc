/*
 * @Author: Limer
 * @Date: 2022-05-30 12:29:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-15 13:54:25
 * @Description:
 */
#include "acceptor.h"
#include "channel.h"
#include "event_loop.h"
#include "my_socket.h"

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
    srv_chl->setUseThreadPool(false);
    delete srv_addr;
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