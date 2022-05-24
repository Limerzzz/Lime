/*
 * @Author: Limer
 * @Date: 2022-05-24 19:31:34
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-24 20:09:10
 * @Description:
 */
#include "Epoll.h"
#include <string.h>
#include <sys/epoll.h>
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

std::vector<epoll_event> Epoll::poll() {
    struct epoll_event ev_arr[1024];
    memset(ev_arr, 1024, '\0');
    int ev_nums = ::epoll_wait(epfd, ev_arr, 1024, -1);
    errif(ev_nums == -1, "fail to load event!\n");
    return std::vector<epoll_event>(ev_arr, ev_arr + ev_nums);
}

void Epoll::close() { ::close(epfd); }