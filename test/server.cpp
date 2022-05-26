/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-26 12:36:05
 * @Description: A basic demo of server.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Channel.h"
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
    auto serv_chl = new Channel(ep, serv_sock->get_fd());
    serv_chl->enableReading();
    int ret = -1;
    for (;;) {
        std::vector<Channel*> ev_vec = ep->poll();
        int num_ev = ev_vec.size();
        for (int i = 0; i < num_ev; ++i) {
            int cur_fd = ev_vec[i]->getSockfd();
            if (cur_fd == serv_sock->get_fd()) {
                auto cli_sock = serv_sock->accept();
                cli_sock.setnonblocking();
                auto chl = new Channel(ep, cli_sock.get_fd());
                chl->enableReading();
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
    serv_sock->close();
    delete serv_sock;
    delete serv_sock;
    delete ep;
    ep->close();
    return 0;
}