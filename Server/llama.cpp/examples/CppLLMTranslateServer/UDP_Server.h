/*
 * @Date: 2024-08-20 11:26:18
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-23 15:26:45
 * @FilePath: /llama.cpp/examples/CppLLMTranslateServer/UDP_Server.h
 */
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class UDP_Server {
  private:
    int socketfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in recv_addr;
    socklen_t address_len;
    char buf[65507];

  public:
    UDP_Server(int port);
    ~UDP_Server();
    bool Initialize();
    void Run();
    void Close() const;

    void Send_thread();
    void Recv_thread();
};

#endif // UDP_SERVER_H
