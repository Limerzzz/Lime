/*
 * @Author: Limer
 * @Date: 2022-05-27 13:30:01
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-23 13:08:36
 * @Description:
 */
#include "event_loop.h"
#include <vector>
#include "channel.h"
#include "epoll.h"
#include "thread_pool.h"

EventLoop::EventLoop() {
    ep_ = new Epoll();
    pool_ = new ThreadPool();
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

void EventLoop::addThread(std::function<void()> func) { pool_->add(func); }
