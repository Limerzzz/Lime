/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-23 12:58:06
 * @Description: A basic demo of server.
 */
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// include struct sockaddr_in
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdio>
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024
#define max_event 65535

int setnonblocking(int fd) {
    // ! cmd is F_GETFL/F_SETFL(not F_GETFD/F_SETFD)
    int old_opt = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old_opt | O_NONBLOCK);
    return old_opt;
}

void addfd_epoll(int epfd, int fd, uint32_t events) {
    setnonblocking(fd);
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int main() {
    int ret = -1;
    // create a sockfd which represent the server.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "fail to create a socket!\n");
    // bind a sockaddr.
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = ::inet_addr(ip_addr);
    servAddr.sin_port = ::htons(port);
    servAddr.sin_family = AF_INET;
    ret = bind(sockfd, (sockaddr*)&servAddr, sizeof(servAddr));
    errif(ret == -1, "fail to bind server address to the socket fd\n");
    // listen the post
    ret = listen(sockfd, 16);
    errif(ret == -1, "fail to listen the socket!\n");

    // create a epoll fd.
    int epfd = epoll_create1(0);
    // add sockfd to the listen list
    addfd_epoll(epfd, sockfd, EPOLLIN);
    struct epoll_event ev_arr[max_event];
    for (;;) {
        memset(ev_arr, max_event, '\0');
        int num_ev = epoll_wait(epfd, ev_arr, max_event, -1);
        errif(num_ev == -1, "fail to wait epoll events!\n");

        for (int i = 0; i < num_ev; ++i) {
            int cur_fd = ev_arr[i].data.fd;
            if (cur_fd == sockfd) {
                struct sockaddr_in cntSockAddr;
                bzero(&cntSockAddr, sizeof(cntSockAddr));
                socklen_t len = 0;
                int cntSockfd = accept(sockfd, (sockaddr*)&cntSockAddr, &len);
                addfd_epoll(epfd, cntSockfd, EPOLLIN | EPOLLET);
                printf("a new connection:%d \n", cntSockfd);
            } else {
                char buf[buf_size];
                memset(buf, buf_size, '\0');
                ret = read(cur_fd, buf, buf_size);
                errif(ret == -1, "fail to read message from client!\n");
                if (ret == -1) break;
                ::printf("message from socket: %d, %s\n", cur_fd, buf);
                ret = write(cur_fd, buf, ret + 1);
                errif(ret == -1, "fail to write message !\n");
                if (ret == -1) break;
            }
        }
    }
    close(sockfd);
    close(epfd);
    return 0;
}