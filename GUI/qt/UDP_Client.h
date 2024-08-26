#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QString>


class UDP_Client {
  private:
    QUdpSocket *udpSocket= nullptr;
    QHostAddress serverAddress=QHostAddress("0.0.0.0");
    quint16 serverPort=0;
    char buf_recv[65507];

  public:
    UDP_Client(const QString& ip, quint16 port);
    ~UDP_Client();
    bool Initialize(const QString& ip, quint16 port);
    void Close();

    void Send_thread();
    void Recv_thread();

    void Send(QString msg);
    QString Recv(int timeout);

};

#endif // UDP_CLIENT_H
