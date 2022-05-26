/*
 * @Author: Limer
 * @Date: 2022-05-24 19:31:34
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-26 12:32:56
 * @Description:
 */
#include "Epoll.h"
#include <string.h>
#include <sys/epoll.h>
#include "Channel.h"
#include "unistd.h"
#include "util.h"

Epoll::Epoll() { epfd = ::epoll_create1(0); }

void Epoll::add_fd(int sockfd, uint32_t events) {
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = events;
    int ret = ::epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    errif(ret == -1, "fail to add sockfd to epoll listen list!\n");
}

std::vector<Channel*> Epoll::poll() {
    std::vector<Channel*> chl_vec;
    struct epoll_event ev_arr[1024];
    memset(ev_arr, 1024, '\0');
    int ev_nums = ::epoll_wait(epfd, ev_arr, 1024, -1);
    errif(ev_nums == -1, "fail to load event!\n");
    for (int i = 0; i < ev_nums; ++i) {
        // ! 大量的channel被建立了.这是不应该的.
        Channel* chl = new Channel(this, ev_arr[i].data.fd);
        chl->setRevents(ev_arr[i].events);
        chl_vec.push_back(chl);
    }
    return chl_vec;
}

void Epoll::close() { ::close(epfd); }

void Epoll::updateChannel(Channel* chl) {
    int ret = -1;
    int sockfd = chl->getSockfd();
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = chl->getEvents();
    if (!chl->isEpoll()) {
        ret = ::epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
        errif(ret == -1, "fail to add sockfd to epoll listen list!\n");
    } else {
        ret = ::epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
        errif(ret == -1, "fail to change listen method !\n");
    }
}