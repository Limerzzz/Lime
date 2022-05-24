/*
 * @Author: Limer
 * @Date: 2022-05-24 13:32:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-24 19:43:21
 * @Description:
 */
#ifndef __EPOLL_H__
#define __EPOLL_H__
#include <sys/epoll.h>
#include <sys/types.h>
#include <vector>
class Epoll {
   public:
    Epoll();
    void add_fd(int, uint32_t);
    std::vector<epoll_event> poll();
    void close();

   private:
    int epfd;
};
#endif