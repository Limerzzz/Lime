/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-24 20:10:58
 * @Description: A basic demo of server.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Epoll.h"
#include "Socket.h"
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    Socket* serv_sock = new Socket();
    InetAddr* serv_addr = new InetAddr(ip_addr, port);
    Epoll* ep = new Epoll();

    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();
    ep->add_fd(serv_sock->get_fd(), EPOLLIN);
    int ret = -1;
    for (;;) {
        auto ev_vec = ep->poll();
        printf("-2\n");
        int num_ev = ev_vec.size();
        for (int i = 0; i < num_ev; ++i) {
            int cur_fd = ev_vec[i].data.fd;
            if (cur_fd == serv_sock->get_fd()) {
                auto cli_sock = serv_sock->accept();
                ep->add_fd(cli_sock.get_fd(), EPOLLIN | EPOLLET);
                cli_sock.setnonblocking();
                ::printf("a new connection:%d \n", cli_sock.get_fd());
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
    delete serv_sock;
    delete serv_sock;
    delete ep;
    return 0;
}