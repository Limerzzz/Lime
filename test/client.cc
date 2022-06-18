/*
 * @Author: Limer
 * @Date: 2022-05-19 13:37:51
 * @LastEditors: Limer
 * @LastEditTime: 2022-06-01 20:11:31
 * @Description:
 */
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "buffer.h"
#include "my_socket.h"
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    int ret = -1;
    Socket* sock = new Socket();
    InetAddr* srv_addr = new InetAddr(ip_addr, port);
    sock->connect(srv_addr);
    sock->setnonblocking();
    Buffer* readbuf = new Buffer();
    Buffer* writebuf = new Buffer();
    while (true) {
        char buf[buf_size];
        writebuf->getline();
        ret = write(sock->get_fd(), writebuf->c_str(), writebuf->size() + 1);
        errif(ret == -1, "fail to write message");
        writebuf->clear();
        int already_read = 0;
        for (;;) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sock->get_fd(), buf, sizeof(buf));
            if (read_bytes > 0) {
                readbuf->append(buf);
                already_read += read_bytes;
            } else if (read_bytes == 0) {  // EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= readbuf->size()) {
                printf("message from server: %s\n", readbuf->c_str());
                break;
            }
        }
        readbuf->clear();
    }
    delete sock;
    delete srv_addr;
    delete readbuf;
    delete writebuf;
    return 0;
}