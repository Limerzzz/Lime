/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-27 13:45:13
 * @Description: A basic demo of server.
 */
#include "Server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "EventLoop.h"
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    EventLoop* ep = new EventLoop();
    Server* srv = new Server(ep);
    ep->loop();
    delete ep;
    delete srv;
}