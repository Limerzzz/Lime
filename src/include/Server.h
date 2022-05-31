/*
 * @Author: Limer
 * @Date: 2022-05-26 13:28:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-31 13:27:37
 * @Description:
 */
#pragma once
#include <unordered_map>

class EventLoop;
class Channel;
class Socket;
class Acceptor;
class Connection;
class Server {
   public:
    Server(EventLoop*);
    ~Server();
    void newConn(Socket*);
    void deleteConn(Socket*);

   private:
    EventLoop* ep;
    Acceptor* acpt;
    std::unordered_map<int, Connection*> conns;
};