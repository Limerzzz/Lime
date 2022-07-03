/*
 * @Author: Limer
 * @Date: 2022-05-31 13:04:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-24 13:54:12
 * @Description:
 */
#pragma once
#include <functional>
#include <string>

class Channel;
class Socket;
class EventLoop;
class Buffer;

class Connection {
   public:
    enum Statue { kDisconnected = 0, KConnected = 1 };

   public:
    Connection(EventLoop*, Socket*);
    void echo(int);
    void setDeleteConnCallback(std::function<void(Socket*)>);
    void set_on_cb(std::function<void(Connection*)>);
    void ReadMessage();
    char* get_write_buf();
    char* get_read_buf();
    void set_write_buf();

    void WriteMessage();
    ~Connection();

   private:
    Socket* sock_;
    Channel* chl_;
    EventLoop* ep_;
    std::function<void(Socket*)> dele_cb_;
    std::function<void(Connection*)> on_cb_;
    Buffer* read_buf_;
    Buffer* write_buf_;
    Connection::Statue statue_;
};