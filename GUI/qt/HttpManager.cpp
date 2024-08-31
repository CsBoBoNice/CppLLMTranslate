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


HttpManager *HttpManager::m_instance = nullptr;

HttpManager &HttpManager::instance()
{
    if (!m_instance) {
        m_instance = new HttpManager();
    }
    return *m_instance;
}

HttpManager::HttpManager()
    : m_maxRetries(3), m_timeout(60000), m_model("gpt-4o"), m_apiKey("888888"),
      m_url("http://127.0.0.1:59218/v1/chat/completions")
{
}

HttpManager::~HttpManager()
{
    // delete m_instance;
    // m_instance = nullptr;
}

void HttpManager::InitHttpManager(QString url, QString apiKey, QString model, int timeout, int maxRetries)
{
    m_apiKey = apiKey;
    m_maxRetries = maxRetries;
    m_timeout = timeout;
    m_url = url;
    m_model = model;
}

bool HttpManager::sendRequest(const QJsonDocument &doc,QString &ret_msg)
{

    QUrl url(m_url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + m_apiKey.toUtf8());

    int retries = 0;
    while (retries < m_maxRetries) {
        QNetworkReply *reply = m_networkManager.post(request, doc.toJson());

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        QTimer::singleShot(m_timeout, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error()) {
            qDebug() << "Network error:" << reply->errorString();
            if (reply->error() == QNetworkReply::OperationCanceledError) {
                qDebug() << "Network operation was canceled.";
            } else if (reply->error() == QNetworkReply::TimeoutError) {
                qDebug() << "Network operation timed out.";
            } else {
                qDebug() << "Network error else:" << reply->errorString();
            }

            ret_msg = "ERROR: " + reply->errorString();

            reply->abort();
            reply->deleteLater();
            retries++;
            continue;
        } else {
            QByteArray response = reply->readAll();
            QString responseStr = QString::fromUtf8(response);
            qDebug() << "Response as QString:" << responseStr;

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(responseStr.toUtf8(), &parseError);
            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                ret_msg = "ERROR: JSON parse error:" + parseError.errorString();
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

                ret_msg = messageContent;
            }

            reply->deleteLater();

            break;
        }
    }

    if (retries >= m_maxRetries) {
        qDebug() << "Failed to parse JSON after" << m_maxRetries << "retries.";
    }

    return true;
}

void HttpManager::sendRequestJson(std::string json_msg)
{
    agreementInfo info = agreement::getInstance().parseJson(json_msg);

    std::string Request_string;
    sendRequestAgreementInfo(info,Request_string);

    agreementInfo retInfo;
    retInfo.cmd = (int)AgreementCmd::success_msg;
    retInfo.msg = Request_string;
    std::string retInfoJson = agreement::getInstance().wrapToJson(retInfo);
    MessageManager::getInstance().pushToInputQueue(retInfoJson);
}

bool HttpManager::sendRequestAgreementInfo(agreementInfo info,std::string& ret_msg)
{
    bool ret=true;

    std::string system = info.system;
    std::string user_msg_1 = info.chat_prefix + info.user_msg_1 + info.chat_suffix;
    std::string user_msg_2 = info.chat_prefix + info.user_msg_2 + info.chat_suffix;
    std::string user_msg_3 = info.chat_prefix + info.user_msg_3 + info.chat_suffix;
    std::string assistant_msg_1 = info.assistant_msg_1;
    std::string assistant_msg_2 = info.assistant_msg_2;
    std::string assistant_msg_3 = info.assistant_msg_3;
    std::string msg = info.chat_prefix + info.msg + info.chat_suffix;

    QJsonArray messages;
    messages.append(QJsonObject{
        {   "role",       "system"},
        {"content", system.c_str()}
    });

    if (!user_msg_1.empty() && !assistant_msg_1.empty()) {
        messages.append(QJsonObject{
            {   "role",             "user"},
            {"content", user_msg_1.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",             "assistant"},
            {"content", assistant_msg_1.c_str()}
        });
    }

    if (!user_msg_2.empty() && !assistant_msg_2.empty()) {
        messages.append(QJsonObject{
            {   "role",             "user"},
            {"content", user_msg_2.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",             "assistant"},
            {"content", assistant_msg_2.c_str()}
        });
    }

    if (!user_msg_3.empty() && !assistant_msg_3.empty()) {
        messages.append(QJsonObject{
            {   "role",             "user"},
            {"content", user_msg_3.c_str()}
        });
        messages.append(QJsonObject{
            {   "role",             "assistant"},
            {"content", assistant_msg_3.c_str()}
        });
    }

    messages.append(QJsonObject{
        {   "role",      "user"},
        {"content", msg.c_str()}
    });

    QJsonObject requestObj{
        {   "model",  m_model},
        {"messages", messages}
    };

    QJsonDocument doc(requestObj);
    // 打印请求的JSON数据
    qDebug() << "Request JSON:\n" << QString::fromUtf8(doc.toJson());
    QString retString;
    ret= sendRequest(doc,retString);
    ret_msg=retString.toStdString();
    return ret;
}

void HttpManager::SendRequest_thread()
{
    while (1) {
        std::string json_msg;
        MessageManager::getInstance().popFromOutputQueue(json_msg);

        sendRequestJson(json_msg);
    }
}
