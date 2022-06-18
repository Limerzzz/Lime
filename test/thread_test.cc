/*
 * @Author: Limer
 * @Date: 2022-06-15 13:45:31
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-16 13:44:55
 * @Description:
 */
#include <unistd.h>
#include <cstring>
#include <functional>
#include <iostream>

#include "buffer.h"
#include "my_socket.h"
#include "thread_pool.h"
#include "util.h"

void oneclient(int msgs, int wait) {
    Socket* sock = new Socket();
    InetAddr* addr = new InetAddr("127.0.0.1", 1234);
    sock->connect(addr);

    Buffer* send_buf = new Buffer();
    ::sleep(wait);
    int count = 0;
    while (count < msgs) {
        send_buf->SetBuffer("I'm client!\n");
        ssize_t write_bytes =
            ::write(sock->get_fd(), send_buf->c_str(), send_buf->size());
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
    }
    delete addr;
    delete send_buf;
    delete sock;
}

int main(int argc, char** argv) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o = -1;
    const char* opt_string = "t:m:w:";
    while ((o = getopt(argc, argv, opt_string)) != -1) {
        switch (o) {
            case 't':
                threads = std::stoi(optarg);
                break;
            case 'm':
                msgs = std::stoi(optarg);
                break;
            case 'w':
                wait = std::stoi(optarg);
                break;
            case '?':
                ::printf("error optopt: %c\n", optopt);
                ::printf("error opterr: %d\n", opterr);
                break;
            default:
                break;
        }
    }
    ThreadPool* pool = new ThreadPool(threads);
    std::function<void()> func = std::bind(oneclient, msgs, wait);
    for (int i = 0; i < threads; ++i) {
        pool->add(func);
    }
    delete pool;
    return 0;
}