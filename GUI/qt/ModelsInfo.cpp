/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:44:51
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 14:43:13
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

ModelsInfo_s ModelsInfo::DefaultGetServerIP()
{
    ModelsInfo_s server_info{
        .url = "http://127.0.0.1:11434/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
    return server_info;
}

ModelsInfo_s ModelsInfo::GetServerIP()
{
    ModelsInfo_s server_info = DefaultGetServerIP();  // 使用默认值初始化
    std::string ip_json;
    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";
    QFile file(ipconfig);
    if (file.exists()) {
        ip_json = readFile(ipconfig);
        cJSON *root = cJSON_Parse(ip_json.c_str());
        if (root) {
            cJSON *serversArray = cJSON_GetObjectItem(root, "servers");
            if (serversArray && cJSON_IsArray(serversArray) && cJSON_GetArraySize(serversArray) > 0) {
                cJSON *firstServer = cJSON_GetArrayItem(serversArray, 0);
                if (firstServer) {
                    cJSON *title = cJSON_GetObjectItem(firstServer, "title");
                    cJSON *url = cJSON_GetObjectItem(firstServer, "url");
                    cJSON *apiKey = cJSON_GetObjectItem(firstServer, "apiKey");
                    cJSON *model = cJSON_GetObjectItem(firstServer, "model");

                    if (title && cJSON_IsString(title)) {
                        server_info.title = title->valuestring;
                    }
                    if (url && cJSON_IsString(url)) {
                        server_info.url = url->valuestring;
                    }
                    if (apiKey && cJSON_IsString(apiKey)) {
                        server_info.apiKey = apiKey->valuestring;
                    }
                    if (model && cJSON_IsString(model)) {
                        server_info.model = model->valuestring;
                    }
                }
            }
            cJSON_Delete(root);
        }
    }
    return server_info;
}

void ModelsInfo::SetServerIP(const ModelsInfo_s &newServerInfo)
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

std::vector<ModelsInfo_s> ModelsInfo::GetAllServerIPs()
{
    std::vector<ModelsInfo_s> allServerInfos;
    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";
    std::string json_content = readFile(ipconfig);
    
    cJSON *root = cJSON_Parse(json_content.c_str());
    if (root == nullptr) {
        return allServerInfos;
    }

    cJSON *serverArray = cJSON_GetObjectItem(root, "servers");
    if (serverArray != nullptr && cJSON_IsArray(serverArray)) {
        int arraySize = cJSON_GetArraySize(serverArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON *serverObject = cJSON_GetArrayItem(serverArray, i);
            ModelsInfo_s serverInfo;
            serverInfo.title = cJSON_GetObjectItem(serverObject, "title")->valuestring;
            serverInfo.url = cJSON_GetObjectItem(serverObject, "url")->valuestring;
            serverInfo.apiKey = cJSON_GetObjectItem(serverObject, "apiKey")->valuestring;
            serverInfo.model = cJSON_GetObjectItem(serverObject, "model")->valuestring;
            allServerInfos.push_back(serverInfo);
        }
    }

    cJSON_Delete(root);
    return allServerInfos;
}

void ModelsInfo::SaveServerIP(const ModelsInfo_s &serverInfo)
{
    std::vector<ModelsInfo_s> allServerInfos = GetAllServerIPs();
    
    // 检查是否已存在相同标题的配置，如果存在则更新
    bool found = false;
    for (auto &info : allServerInfos) {
        if (info.title == serverInfo.title) {
            info = serverInfo;
            found = true;
            break;
        }
    }
    
    // 如果不存在，则添加新配置
    if (!found) {
        allServerInfos.push_back(serverInfo);
    }

    // 保存所有配置到文件
    cJSON *root = cJSON_CreateObject();
    cJSON *serverArray = cJSON_CreateArray();
    for (const auto &info : allServerInfos) {
        cJSON *serverObject = cJSON_CreateObject();
        cJSON_AddStringToObject(serverObject, "title", info.title.c_str());
        cJSON_AddStringToObject(serverObject, "url", info.url.c_str());
        cJSON_AddStringToObject(serverObject, "apiKey", info.apiKey.c_str());
        cJSON_AddStringToObject(serverObject, "model", info.model.c_str());
        cJSON_AddItemToArray(serverArray, serverObject);
    }
    cJSON_AddItemToObject(root, "servers", serverArray);

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";
    saveFile(ipconfig, result);
}

void ModelsInfo::DeleteServerIP(const std::string &title)
{
    std::vector<ModelsInfo_s> allServerInfos = GetAllServerIPs();
    allServerInfos.erase(
        std::remove_if(allServerInfos.begin(), allServerInfos.end(),
                       [&title](const ModelsInfo_s &info) { return info.title == title; }),
        allServerInfos.end());

    // 保存更新后的配置到文件
    cJSON *root = cJSON_CreateObject();
    cJSON *serverArray = cJSON_CreateArray();
    for (const auto &info : allServerInfos) {
        cJSON *serverObject = cJSON_CreateObject();
        cJSON_AddStringToObject(serverObject, "title", info.title.c_str());
        cJSON_AddStringToObject(serverObject, "url", info.url.c_str());
        cJSON_AddStringToObject(serverObject, "apiKey", info.apiKey.c_str());
        cJSON_AddStringToObject(serverObject, "model", info.model.c_str());
        cJSON_AddItemToArray(serverArray, serverObject);
    }
    cJSON_AddItemToObject(root, "servers", serverArray);

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    QString ipconfig = QCoreApplication::applicationDirPath() + "/Server_config.json";
    saveFile(ipconfig, result);
}
