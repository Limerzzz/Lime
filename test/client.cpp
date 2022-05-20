/*
 * @Author: Limer
 * @Date: 2022-05-19 13:37:51
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-19 13:59:34
 * @Description:
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "util.h"

#define port 1234
#define ip_addr "127.0.0.1"
#define buf_size 1024

int main() {
    int ret = -1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servSockAddr;
    servSockAddr.sin_addr.s_addr = inet_addr(ip_addr);
    servSockAddr.sin_port = htons(port);
    servSockAddr.sin_family = AF_INET;
    ret = ::connect(sockfd, (sockaddr*)&servSockAddr, sizeof(servSockAddr));
    errif(ret == -1, "fail to connect server");
    while (true) {
        char buf[buf_size];
        memset(buf, buf_size, '\0');
        scanf("%s", buf);
        ret = write(sockfd, buf, strlen(buf) + 1);
        errif(ret == -1, "fail to write message");
        memset(buf, buf_size, '\0');
        ret = read(sockfd, buf, buf_size);
        errif(ret == -1, "fail to read message");
        printf("%s\n", buf);
    }
    close(sockfd);
    return 0;
}