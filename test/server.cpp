/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-26 18:28:18
 * @Description: A basic demo of server.
 */
#include "Server.h"
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
    Epoll* ep = new Epoll();
    Server* srv = new Server(ep);
    for (;;) {
        auto Evec = ep->poll();
        for (auto it : Evec) {
            it->handleEvent();
        }
    }
    delete ep;
    delete srv;
}