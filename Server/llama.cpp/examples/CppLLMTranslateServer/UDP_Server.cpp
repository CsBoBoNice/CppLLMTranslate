/*
 * @Date: 2024-08-20 11:26:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-23 16:21:32
 * @FilePath: /llama.cpp/examples/CppLLMTranslateServer/UDP_Server.cpp
 */
#include "UDP_Server.h"

#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "MessageManager.h"

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
#ifdef _WIN32
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return -1;
    }

    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketfd == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
#else
    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketfd == -1) {
        std::cerr << "socket fail " << errno << std::endl;

        return false;
    }
#endif

#ifdef _WIN32
    char optval = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#else
    int optval = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif

    if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "bind fail" << std::endl;
        return false;
    }

    std::cout << "+++++++++++ bind IP:0.0.0.0 PORT:" << ntohs(server_addr.sin_port) << std::endl;
    return true;
}

void UDP_Server::Close() const
{
#ifdef _WIN32
    closesocket(socketfd);
    WSACleanup();
#else
    close(socketfd);
#endif
}

void UDP_Server::Recv_thread()
{
    int count_get = 0;
    while (true) {
        memset(&buf, 0, sizeof(buf));
        int ret = recvfrom(socketfd, buf, sizeof(buf), 0, (struct sockaddr *)&recv_addr, &address_len);

        if (ret > 0) {
            std::string input_str(buf);
            MessageManager &manager = MessageManager::getInstance();
            manager.pushToInputQueue(input_str);
        }

        char *ip = inet_ntoa(recv_addr.sin_addr);
        int port = ntohs(recv_addr.sin_port);

        std::cout << "[" << ip << "][" << port << "]recv len:" << ret << " " << ++count_get << std::endl;
        std::cout << "data[" << strlen(buf) << "][" << buf << "]" << std::endl;
    }
}

void UDP_Server::Send_thread()
{
    while (true) {

        MessageManager &manager_input = MessageManager::getInstance();
        std::string message;
        manager_input.popFromOutputQueue(message);

        sendto(socketfd, message.c_str(), message.length(), 0, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
        std::cout << "send " << message.length() << " " << message.c_str() << std::endl;
    }
}
