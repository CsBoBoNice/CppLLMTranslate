/*
 * @Date: 2024-08-21 08:27:54
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-22 14:08:30
 * @FilePath: /CppLLMTranslate/GUI/lvgl/main/src/UDP_Client.h
 */
#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <string>
#include <netinet/in.h>

class UDP_Client {
private:
    int socketfd;
    struct sockaddr_in server_addr;
    char buf_recv[65507];

public:
    UDP_Client(const std::string& ip, int port);
    ~UDP_Client();
    bool Initialize();
    void Run();
    void Close();


    void Send_thread();
    void Recv_thread();

};

#endif // UDP_CLIENT_H
