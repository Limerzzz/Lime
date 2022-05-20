/*
 * @Author: Limer
 * @Date: 2022-05-19 12:30:17
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-19 13:36:50
 * @Description:
 */
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// include struct sockaddr_in
#include <arpa/inet.h>
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    int ret = -1;
    // 1. creat a socket.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "fail to create a socket!\n");
    // 2. bind a sockaddr.
    struct sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = ::inet_addr(ip_addr);
    servAddr.sin_port = ::htons(port);
    servAddr.sin_family = AF_INET;
    ret = bind(sockfd, (sockaddr*)&servAddr, sizeof(servAddr));
    errif(ret == -1, "fail to bind server address to the socket fd\n");
    // 3. listen the post
    ret = listen(sockfd, 10);
    errif(ret == -1, "fail to listen the socket!\n");

    // 4. accept a sockfd which want to make a connect.
    struct sockaddr_in cntSockAddr;
    bzero(&cntSockAddr, sizeof(cntSockAddr));
    socklen_t len = 0;
    int cntSockfd = accept(sockfd, (sockaddr*)&cntSockAddr, &len);
    while (true) {
        char buf[buf_size];
        memset(buf, buf_size, '\0');
        ret = read(cntSockfd, buf, buf_size);
        errif(ret == -1, "fail to read message from client!\n");
        if (ret == -1) break;
        ret = write(cntSockfd, buf, ret + 1);
    }
    close(sockfd);
    close(cntSockfd);
    return 0;
}