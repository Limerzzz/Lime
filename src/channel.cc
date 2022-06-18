/*
 * @Author: Limer
 * @Date: 2022-05-25 13:21:20
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-15 13:54:34
 * @Description:
 */
#include "channel.h"
#include <sys/epoll.h>
#include "event_loop.h"

Channel::Channel(EventLoop* ep, int sockfd) : ep(ep), sockfd(sockfd) {
    events = 0;
    revents = 0;
    inEpoll = false;
    useThreadPool = true;
}

Channel::~Channel() {}

int Channel::getSockfd() { return sockfd; }
bool Channel::isEpoll() { return inEpoll; }
uint32_t Channel::getEvents() { return events; }
uint32_t Channel::getRevents() { return revents; }
void Channel::setEvents(uint32_t events) {
    events = events;
    ep->updateChannel(this);
}
void Channel::setRevents(uint32_t events) { revents = events; }

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

void Channel::setcallback(std::function<void()> cb_) { cb = cb_; }
void Channel::setUseThreadPool(bool tf) { useThreadPool = tf; };

void Channel::handleEvent() {
    if (useThreadPool)
        ep->addThread(cb);
    else
        cb();
}