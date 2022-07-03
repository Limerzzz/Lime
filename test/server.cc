/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-24 13:41:30
 * @Description: A basic demo of server.
 */
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "connection.h"
#include "event_loop.h"
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    EventLoop* ep = new EventLoop();
    Server* srv = new Server(ep);
    srv->set_on_conn([](Connection* conn) {
        conn->ReadMessage();
        conn->WriteMessage();
    });
    ep->loop();
    delete ep;
    delete srv;
}