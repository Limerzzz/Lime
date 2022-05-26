/*
 * @Author: Limer
 * @Date: 2022-05-25 13:03:14
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-26 18:27:24
 * @Description:  used as a brige between epoll and socket.
 */
#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include <cstdint>
#include <functional>
class Epoll;
class Channel {
   public:
    Channel(Epoll*, int);
    void enableReading();
    bool isEpoll();
    uint32_t getEvents();
    uint32_t getRevents();
    int getSockfd();
    void setEvents(uint32_t);
    void setRevents(uint32_t);

    void setcallback(std::function<void()>);

    void handleEvent();

   private:
    Epoll* ep;
    int sockfd;
    uint32_t events;   //  events which the channel should listen.
    uint32_t revents;  // events which the activate.
    bool inEpoll;      // whether in the listen list of epollfd.
    std::function<void()> cb;
};

#endif