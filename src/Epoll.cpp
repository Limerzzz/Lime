/*
 * @Author: Limer
 * @Date: 2022-05-24 19:31:34
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:15:13
 * @Description:
 */
#include "Epoll.h"
#include <string.h>
#include <sys/epoll.h>
#include "Channel.h"
#include "unistd.h"
#include "util.h"

Epoll::Epoll() { epfd = ::epoll_create1(0); }
Epoll::~Epoll() { close(epfd); }

std::vector<Channel*> Epoll::poll() {
    std::vector<Channel*> chl_vec;
    struct epoll_event ev_arr[1024];
    memset(ev_arr, 1024, '\0');
    int ev_nums = ::epoll_wait(epfd, ev_arr, 1024, -1);
    errif(ev_nums == -1, "fail to load event!\n");
    for (int i = 0; i < ev_nums; ++i) {
        Channel* chl = (Channel*)ev_arr[i].data.ptr;
        chl->setRevents(ev_arr[i].events);
        chl_vec.push_back(chl);
    }
    return chl_vec;
}

void Epoll::updateChannel(Channel* chl) {
    int ret = -1;
    int sockfd = chl->getSockfd();
    struct epoll_event ev;
    ev.data.ptr = chl;  // using ptr pointer to chl
    ev.events = chl->getEvents();
    if (!chl->isEpoll()) {
        ret = ::epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
        errif(ret == -1, "fail to add sockfd to epoll listen list!\n");
    } else {
        ret = ::epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
        errif(ret == -1, "fail to change listen method !\n");
    }
}