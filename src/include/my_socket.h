/*
 * @Author: Limer
 * @Date: 2022-05-23 13:29:07
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-30 13:16:07
 * @Description: Socket class which package some function concerns socket
 * connection.
 */
#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <netinet/in.h>
class InetAddr {
   public:
    InetAddr();
    InetAddr(const char*, int);
    struct sockaddr_in addr;
    socklen_t addr_size;
};
class Socket {
   public:
    Socket();
    Socket(int);
    ~Socket();
    void bind(InetAddr*);
    void listen();
    void connect(InetAddr*);
    int accept(InetAddr*);
    int get_fd();
    void setnonblocking();

   private:
    int sockfd;
};
#endif