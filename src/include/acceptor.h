/*
 * @Author: Limer
 * @Date: 2022-05-30 12:23:25
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:10:31
 * @Description:
 */
#pragma once
#include <functional>
class EventLoop;
class Channel;
class Socket;
class InetAddr;
class Acceptor {
   public:
    Acceptor(EventLoop*);
    void SetNewConnCallback(std::function<void(Socket*)>);
    void AcceptNewConn();
    ~Acceptor();

   private:
    Channel* srv_chl;
    Socket* srv_sock;
    EventLoop* ev_loop;
    InetAddr* srv_addr;
    std::function<void(Socket*)> cb;
};