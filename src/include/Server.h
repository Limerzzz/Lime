/*
 * @Author: Limer
 * @Date: 2022-05-26 13:28:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-26 13:59:52
 * @Description:
 */
#pragma once

class Epoll;
class Channel;
class Socket;
class Server {
   public:
    Server(Epoll*);
    void newConn(Socket*);
    void readMess(int);

   private:
    Epoll* ep;
};