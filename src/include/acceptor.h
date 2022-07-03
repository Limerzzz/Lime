/*
 * @Author: Limer
 * @Date: 2022-05-30 12:23:25
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-21 14:14:01
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
    Channel* srv_chl_;
    Socket* srv_socket_;
    EventLoop* ev_loop_;
    InetAddr* srv_addr_;
    std::function<void(Socket*)> new_conn_callback_;
};