/*
 * @Author: Limer
 * @Date: 2022-05-25 13:21:20
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-25 13:55:30
 * @Description:
 */
#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll* ep, int sockfd) : ep(ep), sockfd(sockfd) {
    events = 0;
    revents = 0;
    inEpoll = false;
}

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