/*
 * @Author: Limer
 * @Date: 2022-05-26 13:28:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-17 13:39:02
 * @Description:
 */
#pragma once
#include <unordered_map>
#include <vector>

class EventLoop;
class Channel;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
   public:
    Server(EventLoop*);
    ~Server();
    void newConn(Socket*);
    void deleteConn(Socket*);
    //
   private:
    EventLoop* main_reactor_;
    Acceptor* acceptor_;
    std::unordered_map<int, Connection*> conns_;
    ThreadPool* threadpool_;
    int sub_readctor_size_;
    std::vector<EventLoop*> sub_reactors_;
};