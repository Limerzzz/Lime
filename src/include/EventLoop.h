/*
 * @Author: Limer
 * @Date: 2022-05-27 13:23:21
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-27 13:30:55
 * @Description:
 */
#pragma once

class Epoll;
class Channel;
class EventLoop {
   public:
    EventLoop();
    void updateChannel(Channel*);
    void setQuit();
    void loop();

   private:
    Epoll* ep_;
    bool isquit;
};