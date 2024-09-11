/*
 * @Date: 2024-08-30 11:01:26
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-11 09:56:43
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

    bool SendRequest(const QJsonDocument &doc, QString &retMsg);

    void SendRequestJson(std::string jsonMsg);
    bool SendRequestAgreementInfo(agreementInfo info, std::string &retMsg);

    bool SendRequestAgreementInfo(agreementInfo info);

    void SendRequestThread();

  public:
    static QString ApiKey;
    static int MaxRetries;
    static int Timeout;
    static QString Url;
    static QString Model;
};

#endif // HTTPMANAGER_H
