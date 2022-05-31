/*
 * @Author: Limer
 * @Date: 2022-05-26 18:09:08
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-31 13:48:16
 * @Description:
 */
#include "Server.h"
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
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
    Connection* conn = new Connection(ep, cnt_sock);
    conns[cnt_sock->get_fd()] = conn;
    std::function<void(Socket*)> cb =
        std::bind(&Server::deleteConn, this, std::placeholders::_1);
    conn->setDeleteConnCallback(cb);
}

void Server::deleteConn(Socket* sk) {
    close(sk->get_fd());
    auto tmp = conns[sk->get_fd()];
    conns.erase(sk->get_fd());
    delete tmp;
}