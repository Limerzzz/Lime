/*
 * @Author: Limer
 * @Date: 2022-05-31 13:04:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-01 13:17:42
 * @Description:
 */
#pragma once
#include <functional>

class Channel;
class Socket;
class EventLoop;
class Buffer;
class Connection {
   public:
    Connection(EventLoop*, Socket*);
    void echo(int);
    void setDeleteConnCallback(std::function<void(Socket*)>);
    ~Connection();

   private:
    Socket* sock_;
    Channel* chl_;
    EventLoop* ep_;
    std::function<void(Socket*)> dele_cb;
    Buffer* readbuf;
    Buffer* writebuf;
};