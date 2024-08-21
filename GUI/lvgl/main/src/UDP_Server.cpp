/*
 * @Date: 2024-08-20 11:26:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-20 15:18:15
 * @FilePath: /server/src/UDP_Server/UDP_Server.cpp
 */
#include "UDP_Server.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

UDP_Server::UDP_Server(int port)
{
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    address_len = sizeof(recv_addr);
}

UDP_Server::~UDP_Server()
{
    Close();
}

bool UDP_Server::Initialize()
{
    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketfd == -1) {
        std::cerr << "socket fail" << std::endl;
        return false;
    }

    int optval = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "bind fail" << std::endl;
        return false;
    }

    std::cout << "绑定服务器IP:0.0.0.0 端口号PORT:" << ntohs(server_addr.sin_port) << std::endl;
    return true;
}

void UDP_Server::Run()
{
    int count_get = 0;

    while (true) {
        bzero(buf, sizeof(buf));
        int ret = recvfrom(socketfd, buf, sizeof(buf), 0, (struct sockaddr *)&recv_addr, &address_len);

        char *ip = inet_ntoa(recv_addr.sin_addr);
        int port = ntohs(recv_addr.sin_port);

        std::cout << "[" << ip << "][" << port << "]收到数据 len:" << ret << " " << ++count_get << std::endl;
        std::cout << "数据[" << strlen(buf) << "][" << buf << "]" << std::endl;

        strcat(buf, "nice");

        sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
        std::cout << "发送数据 ret:" << ret << " " << buf << std::endl;

        if (!strcmp(buf, "exit")) {
            break;
        }
    }
}

void UDP_Server::Close()
{
    close(socketfd);
}
