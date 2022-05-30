/*
 * @Author: Limer
 * @Date: 2022-05-26 13:28:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:12:10
 * @Description:
 */
#pragma once

class EventLoop;
class Channel;
class Socket;
class Acceptor;
class Server {
   public:
    Server(EventLoop*);
    ~Server();
    void newConn(Socket*);
    void readMess(int);

   private:
    EventLoop* ep;
    Acceptor* acpt;
};