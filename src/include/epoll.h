/*
 * @Author: Limer
 * @Date: 2022-05-24 13:32:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:14:44
 * @Description:
 */
#ifndef __EPOLL_H__
#define __EPOLL_H__
#include <sys/epoll.h>
#include <sys/types.h>
#include <vector>

class Channel;
class Epoll {
   public:
    Epoll();
    ~Epoll();
    std::vector<Channel*> poll();
    void updateChannel(Channel*);

   private:
    int epfd;
};
#endif