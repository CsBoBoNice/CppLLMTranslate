#include "UDP_Client.h"
#include "MessageManager.h"
#include <QThread>

UDP_Client::UDP_Client(const QString& ip, quint16 port) : udpSocket(nullptr), serverAddress(ip), serverPort(port) {}

UDP_Client::~UDP_Client() {
    Close();
}

bool UDP_Client::Initialize(const QString& ip, quint16 port) {
    if (udpSocket && (serverAddress != QHostAddress(ip) || serverPort != port)) {
        this->Close();
    }

    if (!udpSocket) {
        udpSocket = new QUdpSocket();
        serverAddress = QHostAddress(ip);
        serverPort = port;

        if (!udpSocket->bind()) {
            qDebug()  << "Failed to bind UDP socket" ;
            delete udpSocket;
            udpSocket = nullptr;
            return false;
        }
    }

    qDebug()  << "Connected to server [" << serverAddress.toString().toStdString() << "][" << serverPort << "]";
    return true;
}

void UDP_Client::Close() {
    if (udpSocket) {
        udpSocket->close();
        delete udpSocket;
        udpSocket = nullptr;
    }
}

void UDP_Client::Recv_thread() {
    while (udpSocket) {
        if (udpSocket->hasPendingDatagrams()) {
            memset(buf_recv, 0, sizeof(buf_recv));
            QHostAddress sender;
            quint16 senderPort;
            qint64 len = udpSocket->readDatagram(buf_recv, sizeof(buf_recv), &sender, &senderPort);

            if (len > 0) {
                std::string input_str(buf_recv);
                MessageManager::getInstance().pushToInputQueue(input_str);

                qDebug()  << "[" << sender.toString().toStdString() << "][" << senderPort << "]data len:" << len ;
                qDebug()  << "data[" << strlen(buf_recv) << "][" << buf_recv << "]" ;
            }
        }
        QThread::msleep(1);
    }
}

void UDP_Client::Send(QString msg)
{
    std::string message=msg.toStdString();
    udpSocket->writeDatagram(message.c_str(), message.length(), serverAddress, serverPort);
    qDebug()  << "Sent data " << message.length() << " " << message.c_str() ;
}

// timeout 毫秒
QString UDP_Client::Recv(int timeout)
{
    QString ret;
    memset(buf_recv, 0, sizeof(buf_recv));
    QHostAddress sender;
    quint16 senderPort;
    if (udpSocket->waitForReadyRead(timeout)) {
        // 超时时间内接收到数据
        qint64 len = udpSocket->readDatagram(buf_recv, sizeof(buf_recv), &sender, &senderPort);
        if (len > 0) {
            std::string input_str(buf_recv);
            MessageManager::getInstance().pushToInputQueue(input_str);

            qDebug()  << "[" << sender.toString().toStdString() << "][" << senderPort << "]data len:" << len ;
            qDebug()  << "data[" << strlen(buf_recv) << "][" << buf_recv << "]" ;

            return QString::fromStdString(input_str);
        }


    }

    return ret;
}

void UDP_Client::Send_thread() {
    while (udpSocket) {

        std::string message;
        MessageManager::getInstance().popFromOutputQueue(message);

        udpSocket->writeDatagram(message.c_str(), message.length(), serverAddress, serverPort);
        qDebug()  << "Sent data " << message.length() << " " << message.c_str() ;
    }
}
