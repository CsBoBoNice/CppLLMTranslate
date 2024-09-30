/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:44:51
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 14:55:55
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

std::string ModelsInfo::readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    std::string content = in.readAll().toStdString();
    file.close();
    return content;
}

bool ModelsInfo::saveFileContent(const QString &filePath, const std::string &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << QString::fromStdString(content);
    file.close();
    return true;
}

ModelsInfo_s ModelsInfo::getDefaultServerConfig()
{
    return {.url = "http://127.0.0.1:11434/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
}

ModelsInfo_s ModelsInfo::getCurrentServerConfig()
{
    ModelsInfo_s serverConfig = getDefaultServerConfig();
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";
    QFile file(configPath);
    if (file.exists()) {
        std::string jsonContent = readFileContent(configPath);
        cJSON *root = cJSON_Parse(jsonContent.c_str());
        if (root) {
            cJSON *serversArray = cJSON_GetObjectItem(root, "servers");
            if (serversArray && cJSON_IsArray(serversArray) && cJSON_GetArraySize(serversArray) > 0) {
                cJSON *firstServer = cJSON_GetArrayItem(serversArray, 0);
                if (firstServer) {
                    updateServerConfigFromJson(serverConfig, firstServer);
                }
            }
            cJSON_Delete(root);
        }
    }
    return serverConfig;
}

void ModelsInfo::updateServerConfigFromJson(ModelsInfo_s &config, cJSON *jsonObject)
{
    cJSON *title = cJSON_GetObjectItem(jsonObject, "title");
    cJSON *url = cJSON_GetObjectItem(jsonObject, "url");
    cJSON *apiKey = cJSON_GetObjectItem(jsonObject, "apiKey");
    cJSON *model = cJSON_GetObjectItem(jsonObject, "model");

    if (title && cJSON_IsString(title))
        config.title = title->valuestring;
    if (url && cJSON_IsString(url))
        config.url = url->valuestring;
    if (apiKey && cJSON_IsString(apiKey))
        config.apiKey = apiKey->valuestring;
    if (model && cJSON_IsString(model))
        config.model = model->valuestring;
}

void ModelsInfo::setServerConfig(const ModelsInfo_s &newServerConfig)
{
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "url", newServerConfig.url.c_str());
    cJSON_AddStringToObject(root, "apiKey", newServerConfig.apiKey.c_str());
    cJSON_AddStringToObject(root, "model", newServerConfig.model.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    saveFileContent(configPath, result);
}

std::vector<ModelsInfo_s> ModelsInfo::getAllServerConfigs()
{
    std::vector<ModelsInfo_s> allConfigs;
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";
    std::string jsonContent = readFileContent(configPath);

    cJSON *root = cJSON_Parse(jsonContent.c_str());
    if (root == nullptr) {
        return allConfigs;
    }

    cJSON *serverArray = cJSON_GetObjectItem(root, "servers");
    if (serverArray != nullptr && cJSON_IsArray(serverArray)) {
        int arraySize = cJSON_GetArraySize(serverArray);
        for (int i = 0; i < arraySize; i++) {
            cJSON *serverObject = cJSON_GetArrayItem(serverArray, i);
            ModelsInfo_s serverConfig;
            updateServerConfigFromJson(serverConfig, serverObject);
            allConfigs.push_back(serverConfig);
        }
    }

    cJSON_Delete(root);
    return allConfigs;
}

void ModelsInfo::saveServerConfig(const ModelsInfo_s &serverConfig)
{
    std::vector<ModelsInfo_s> allConfigs = getAllServerConfigs();

    auto it = std::find_if(allConfigs.begin(), allConfigs.end(),
                           [&](const ModelsInfo_s &config) { return config.title == serverConfig.title; });

    if (it != allConfigs.end()) {
        *it = serverConfig;
    } else {
        allConfigs.push_back(serverConfig);
    }

    saveAllServerConfigs(allConfigs);
}

void ModelsInfo::deleteServerConfig(const std::string &title)
{
    std::vector<ModelsInfo_s> allConfigs = getAllServerConfigs();
    allConfigs.erase(std::remove_if(allConfigs.begin(), allConfigs.end(),
                                    [&title](const ModelsInfo_s &config) { return config.title == title; }),
                     allConfigs.end());

    saveAllServerConfigs(allConfigs);
}

void ModelsInfo::saveAllServerConfigs(const std::vector<ModelsInfo_s> &configs)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *serverArray = cJSON_CreateArray();
    for (const auto &config : configs) {
        cJSON *serverObject = cJSON_CreateObject();
        cJSON_AddStringToObject(serverObject, "title", config.title.c_str());
        cJSON_AddStringToObject(serverObject, "url", config.url.c_str());
        cJSON_AddStringToObject(serverObject, "apiKey", config.apiKey.c_str());
        cJSON_AddStringToObject(serverObject, "model", config.model.c_str());
        cJSON_AddItemToArray(serverArray, serverObject);
    }
    cJSON_AddItemToObject(root, "servers", serverArray);

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";
    saveFileContent(configPath, result);
}