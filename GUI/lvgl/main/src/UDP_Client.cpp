/*
 * @Date: 2024-08-20 14:56:04
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-20 15:18:49
 * @FilePath: /server/src/UDP_Client/UDP_Client.cpp
 */
#include "UDP_Client.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

UDP_Client::UDP_Client(const std::string &ip, int port)
{
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

UDP_Client::~UDP_Client()
{
    Close();
}

bool UDP_Client::Initialize()
{
    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketfd == -1) {
        std::cerr << "socket fail" << std::endl;
        return false;
    }

    std::cout << "连接服务器成功[" << inet_ntoa(server_addr.sin_addr) << "][" << ntohs(server_addr.sin_port) << "]"
              << std::endl;
    return true;
}

void UDP_Client::Run()
{
    char buf[] = "gogogo";
    socklen_t addr_len = sizeof(server_addr);
    int count_get = 0;

    while (true) {
        int ret = sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        std::cout << "发送数据 ret:" << ret << " " << buf << std::endl;

        // 接收数据
        memset(buf_recv, 0, sizeof(buf_recv));
        addr_len = sizeof(server_addr);
        int recv_len = recvfrom(socketfd, buf_recv, sizeof(buf_recv), 0, (struct sockaddr *)&server_addr, &addr_len);

        if (recv_len == -1) {
            std::cout << "recvfrom() timeout" << std::endl;
        } else {
            char *ip = inet_ntoa(server_addr.sin_addr);
            int port = ntohs(server_addr.sin_port);

            std::cout << "[" << ip << "][" << port << "]收到数据 len:" << recv_len << " " << ++count_get << std::endl;
            std::cout << "数据[" << strlen(buf_recv) << "][" << buf_recv << "]" << std::endl;
        }

        sleep(1);
    }
}

void UDP_Client::Close()
{
    close(socketfd);
}
