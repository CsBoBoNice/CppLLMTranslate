/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:44:51
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 13:56:09
 * @FilePath: /CppLLMTranslate/GUI/qt/ModelsInfo.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by csbobo, All Rights Reserved. 
 */

#include "ModelsInfo.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include "cJSON.h"

ModelsInfo &ModelsInfo::getInstance()
{
    static ModelsInfo instance;
    return instance;
}

std::string ModelsInfo::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ""; // 文件打开失败，返回空字符串
    }

    QTextStream in(&file);
    std::string content = in.readAll().toStdString();
    file.close();
    return content;
}

bool ModelsInfo::saveFile(const QString &filePath, const std::string &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // 文件打开失败，返回 false
    }

    QTextStream out(&file);
    out << QString::fromStdString(content);
    file.close();
    return true;
}

ServerInfo ModelsInfo::DefaultGetServerIP()
{
    ServerInfo server_info{.url = "http://127.0.0.1:11434/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
    return server_info;
}

ServerInfo ModelsInfo::GetServerIP()
{
    ServerInfo server_info{.url = "http://127.0.0.1:11434/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
    std::string ip_json;
    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";
    QFile file(ipconfig);
    if (file.exists() == true) {
        ip_json = readFile(ipconfig);
        cJSON *root = cJSON_Parse(ip_json.c_str());
        if (root == nullptr) {
            // 解析失败
            goto return_info;
        }

        cJSON *url_str = cJSON_GetObjectItem(root, "url");
        if (url_str != nullptr) {
            server_info.url = url_str->valuestring;
        }

        cJSON *apiKey_str = cJSON_GetObjectItem(root, "apiKey");
        if (apiKey_str != nullptr) {
            server_info.apiKey = apiKey_str->valuestring;
        }

        cJSON *model_str = cJSON_GetObjectItem(root, "model");
        if (model_str != nullptr) {
            server_info.model = model_str->valuestring;
        }

        cJSON_Delete(root);
    }

return_info:

    return server_info;
}

void ModelsInfo::SetServerIP(const ServerInfo &newServerInfo)
{
    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "url", newServerInfo.url.c_str());
    cJSON_AddStringToObject(root, "apiKey", newServerInfo.apiKey.c_str());
    cJSON_AddStringToObject(root, "model", newServerInfo.model.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    saveFile(ipconfig, result); // 保存配置文件
}
