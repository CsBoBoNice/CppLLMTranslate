#include "HttpManager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include "MessageManager.h"

// 初始化静态成员变量
QString HttpManager::ApiKey = "888888";
int HttpManager::MaxRetries = 3;
int HttpManager::Timeout = 180000;
QString HttpManager::Url = "http://127.0.0.1:11434/v1/chat/completions";
QString HttpManager::Model = "gpt-4o";

HttpManager::HttpManager() {}

HttpManager::~HttpManager()
{
    // delete m_instance;
    // m_instance = nullptr;
}

void HttpManager::InitHttpManager(QString url, QString apiKey, QString model, int timeout, int maxRetries)
{
    ApiKey = apiKey;
    MaxRetries = maxRetries;
    Timeout = timeout;
    Url = url;
    Model = model;
}

bool HttpManager::SendRequest(const QJsonDocument &doc, QString &retMsg)
{
    QUrl url(Url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + ApiKey.toUtf8());

    int retries = 0;
    while (retries < MaxRetries) {
        QNetworkAccessManager manager;
        manager.setTransferTimeout(Timeout);
        QNetworkReply *reply = manager.post(request, doc.toJson());

        QEventLoop loop;
        QTimer timer;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

        timer.start(Timeout);
        loop.exec();

        // 取消定时器，防止它再次触发事件循环的退出
        timer.stop();

        if (reply->error()) {
            qDebug() << "Network error:" << reply->errorString();
            if (reply->error() == QNetworkReply::OperationCanceledError) {
                qDebug() << "Network operation was canceled.";
            } else if (reply->error() == QNetworkReply::TimeoutError) {
                qDebug() << "Network operation timed out.";
            } else {
                qDebug() << "Network error else:" << reply->errorString();
            }

            retMsg = "ERROR: " + reply->errorString();

            reply->abort();
            reply->deleteLater();
            retries++;
            continue;
        } else {
            QByteArray response = reply->readAll();
            QString responseStr = QString::fromUtf8(response);
            qDebug() << "Response as QString:" << responseStr;

            if (responseStr.isEmpty()) {
                qDebug() << "Empty response from server.";
                retMsg = "ERROR: Empty response from server.";
                reply->deleteLater();
                retries++;
                return false;
            }

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(responseStr.toUtf8(), &parseError);
            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                retMsg = "ERROR: JSON parse error:" + parseError.errorString();
                reply->deleteLater();
                return false;
            }

            QJsonObject jsonObj = doc.object();
            QJsonArray choices = jsonObj["choices"].toArray();
            if (!choices.isEmpty()) {
                QJsonObject firstChoice = choices.first().toObject();
                QString finishReason = firstChoice["finish_reason"].toString();
                QString messageContent = firstChoice["message"].toObject()["content"].toString();
                QString role = firstChoice["message"].toObject()["role"].toString();

                qDebug() << "Finish Reason:" << finishReason;
                qDebug() << "Message Content:" << messageContent;
                qDebug() << "Role:" << role;

                retMsg = messageContent;
            }

            reply->deleteLater();
            break;
        }
    }

    if (retries >= MaxRetries) {
        qDebug() << "Failed to parse JSON after" << MaxRetries << "retries.";
    }

    return true;
}

void HttpManager::SendRequestJson(std::string jsonMsg)
{
    agreementInfo info = agreement::getInstance().parseJson(jsonMsg);

    std::string requestString;
    SendRequestAgreementInfo(info, requestString);

    agreementInfo retInfo;
    retInfo.cmd = (int)AgreementCmd::success_msg;
    retInfo.msg = requestString;
    std::string retInfoJson = agreement::getInstance().wrapToJson(retInfo);
    MessageManager::getInstance().pushToInputQueue(retInfoJson);
}

bool HttpManager::SendRequestAgreementInfo(agreementInfo info, std::string &retMsg)
{
    bool ret = true;

    std::string system = info.system;
    std::string userMsg1 = info.chat_prefix + info.user_msg_1 + info.chat_suffix;
    std::string userMsg2 = info.chat_prefix + info.user_msg_2 + info.chat_suffix;
    std::string userMsg3 = info.chat_prefix + info.user_msg_3 + info.chat_suffix;
    std::string assistantMsg1 = info.assistant_msg_1;
    std::string assistantMsg2 = info.assistant_msg_2;
    std::string assistantMsg3 = info.assistant_msg_3;
    std::string msg = info.chat_prefix + info.msg + info.chat_suffix;

    QJsonArray messages;
    messages.append(QJsonObject{
        {   "role",       "system"},
        {"content", system.c_str()}
    });

    if (!userMsg1.empty() && !assistantMsg1.empty()) {
        messages.append(QJsonObject{
            {   "role",           "user"},
            {"content", userMsg1.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",           "assistant"},
            {"content", assistantMsg1.c_str()}
        });
    }

    if (!userMsg2.empty() && !assistantMsg2.empty()) {
        messages.append(QJsonObject{
            {   "role",           "user"},
            {"content", userMsg2.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",           "assistant"},
            {"content", assistantMsg2.c_str()}
        });
    }

    if (!userMsg3.empty() && !assistantMsg3.empty()) {
        messages.append(QJsonObject{
            {   "role",           "user"},
            {"content", userMsg3.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",           "assistant"},
            {"content", assistantMsg3.c_str()}
        });
    }

    messages.append(QJsonObject{
        {   "role",      "user"},
        {"content", msg.c_str()}
    });

    QJsonObject requestObj{
        {   "model",    Model},
        {"messages", messages}
    };

    QJsonDocument doc(requestObj);
    // 打印请求的JSON数据
    qDebug() << "Request JSON:\n" << QString::fromUtf8(doc.toJson());
    QString retString;
    ret = SendRequest(doc, retString);
    retMsg = retString.toStdString();
    return ret;
}

void HttpManager::SendRequestThread()
{
    while (1) {
        std::string jsonMsg;
        if (MessageManager::getInstance().popFromOutputQueue(jsonMsg)) {
            SendRequestJson(jsonMsg);
        }
    }
}
