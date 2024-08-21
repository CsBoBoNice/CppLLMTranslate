/*
 * @Date: 2024-08-20 11:26:18
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-20 11:37:23
 * @FilePath: /server/src/UdpServer/UdpServer.h
 */
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <string>
#include <netinet/in.h>

class UDP_Server {
private:
    int socketfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in recv_addr;
    socklen_t address_len;
    char buf[2048];

public:
    UDP_Server(int port);
    ~UDP_Server();
    bool Initialize();
    void Run();
    void Close();
};

#endif // UDP_SERVER_H
