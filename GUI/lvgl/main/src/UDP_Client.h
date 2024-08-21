#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <string>
#include <netinet/in.h>

class UDP_Client {
private:
    int socketfd;
    struct sockaddr_in server_addr;
    char buf_recv[1472];

public:
    UDP_Client(const std::string& ip, int port);
    ~UDP_Client();
    bool Initialize();
    void Run();
    void Close();
};

#endif // UDP_CLIENT_H
