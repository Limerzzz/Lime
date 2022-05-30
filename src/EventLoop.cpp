/*
 * @Author: Limer
 * @Date: 2022-05-27 13:30:01
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-27 13:38:21
 * @Description:
 */
#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop() {
    ep_ = new Epoll();
    isquit = false;
}

void EventLoop::setQuit() { isquit = true; }

void EventLoop::updateChannel(Channel* chl) { ep_->updateChannel(chl); }

void EventLoop::loop() {
    while (!isquit) {
        std::vector<Channel*> chl_vec = ep_->poll();
        for (auto it : chl_vec) it->handleEvent();
    }
}