/*
 * @Author: Limer
 * @Date: 2022-05-27 13:23:21
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-08 18:42:46
 * @Description:
 */
#pragma once
#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
   public:
    EventLoop();
    void updateChannel(Channel*);
    void setQuit();
    void loop();
    // confine the function of threadpool
    void addThread(std::function<void()>);

   private:
    Epoll* ep_;
    ThreadPool* pool_;
    bool isquit;
};