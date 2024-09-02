/*
 * @Date: 2024-08-30 11:01:26
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-30 13:41:42
 * @FilePath: /CppLLMTranslate/GUI/qt/HttpManager.h
 */
#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include "agreement.h"

class HttpManager : public QObject {
    Q_OBJECT

  public:

    HttpManager();
    ~HttpManager();

    static void InitHttpManager(QString url, QString apiKey, QString model, int timeout, int maxRetries);

    bool sendRequest(const QJsonDocument &doc,QString &ret_msg);

    void sendRequestJson(std::string json_msg);
    bool sendRequestAgreementInfo(agreementInfo info,std::string& ret_msg);

    bool sendRequest_AgreementInfo(agreementInfo info);

    void SendRequest_thread();

    static QString m_apiKey;
    static int m_maxRetries;
    static int m_timeout;
    static QString m_url;
    static QString m_model;


};

#endif // HTTPMANAGER_H
