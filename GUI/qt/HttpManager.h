#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>


class HttpManager : public QObject
{
    Q_OBJECT

  public:
    static HttpManager& instance();
    ~HttpManager();

    void InitHttpManager(QString url,QString apiKey,QString model,int timeout,int maxRetries);
    QString sendRequest(const QJsonDocument &doc);
    void sendRequestJson(std::string json_msg);
    void SendRequest_thread();


  private:
    HttpManager();

    static HttpManager* m_instance;

    QNetworkAccessManager m_networkManager;
    QString m_apiKey;
    int m_maxRetries;
    int m_timeout;
    QString m_url;
    QString m_model;
};

#endif // HTTPMANAGER_H
