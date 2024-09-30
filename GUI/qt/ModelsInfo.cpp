/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:44:51
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 15:07:44
 * @FilePath: /CppLLMTranslate/GUI/qt/ModelsInfo.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by csbobo, All Rights Reserved. 
 */

// 包含必要的头文件
#include "ModelsInfo.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include "cJSON.h"

// 获取ModelsInfo的单例实例
ModelsInfo &ModelsInfo::getInstance()
{
    static ModelsInfo instance;
    return instance;
}

// 读取文件内容的函数
std::string ModelsInfo::readFileContent(const QString &filePath)
{
    QFile file(filePath);
    // 以只读和文本模式打开文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";  // 如果无法打开文件，返回空字符串
    }

    QTextStream in(&file);
    // 读取所有内容并转换为std::string
    std::string content = in.readAll().toStdString();
    file.close();
    return content;
}

// 保存文件内容的函数
bool ModelsInfo::saveFileContent(const QString &filePath, const std::string &content)
{
    QFile file(filePath);
    // 以只写和文本模式打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;  // 如果无法打开文件，返回false
    }

    QTextStream out(&file);
    // 将std::string转换为QString并写入文件
    out << QString::fromStdString(content);
    file.close();
    return true;
}

// 获取默认服务器配置
ModelsInfo_s ModelsInfo::getDefaultServerConfig()
{
    return {.url = "http://127.0.0.1:11434/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
}

// 获取当前服务器配置
ModelsInfo_s ModelsInfo::getCurrentServerConfig()
{
    ModelsInfo_s serverConfig = getDefaultServerConfig();
    // 构建配置文件路径
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";
    QFile file(configPath);
    if (file.exists()) {
        // 如果配置文件存在，读取其内容
        std::string jsonContent = readFileContent(configPath);
        cJSON *root = cJSON_Parse(jsonContent.c_str());
        if (root) {
            cJSON *serversArray = cJSON_GetObjectItem(root, "servers");
            if (serversArray && cJSON_IsArray(serversArray) && cJSON_GetArraySize(serversArray) > 0) {
                // 获取第一个服务器配置
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

// 从JSON对象更新服务器配置
void ModelsInfo::updateServerConfigFromJson(ModelsInfo_s &config, cJSON *jsonObject)
{
    // 获取各个字段的值并更新配置
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

// 设置服务器配置
void ModelsInfo::setServerConfig(const ModelsInfo_s &newServerConfig)
{
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";

    // 创建JSON对象并添加配置信息
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "url", newServerConfig.url.c_str());
    cJSON_AddStringToObject(root, "apiKey", newServerConfig.apiKey.c_str());
    cJSON_AddStringToObject(root, "model", newServerConfig.model.c_str());

    // 将JSON对象转换为字符串
    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    // 保存配置到文件
    saveFileContent(configPath, result);
}

// 获取所有服务器配置
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
            // 遍历所有服务器配置并添加到vector中
            cJSON *serverObject = cJSON_GetArrayItem(serverArray, i);
            ModelsInfo_s serverConfig;
            updateServerConfigFromJson(serverConfig, serverObject);
            allConfigs.push_back(serverConfig);
        }
    }

    cJSON_Delete(root);
    return allConfigs;
}

// 保存单个服务器配置
void ModelsInfo::saveServerConfig(const ModelsInfo_s &serverConfig)
{
    std::vector<ModelsInfo_s> allConfigs = getAllServerConfigs();

    // 查找是否存在相同标题的配置
    auto it = std::find_if(allConfigs.begin(), allConfigs.end(),
                           [&](const ModelsInfo_s &config) { return config.title == serverConfig.title; });

    if (it != allConfigs.end()) {
        // 如果存在，更新配置
        *it = serverConfig;
    } else {
        // 如果不存在，添加新配置
        allConfigs.push_back(serverConfig);
    }

    // 保存所有配置
    saveAllServerConfigs(allConfigs);
}

// 删除指定标题的服务器配置
void ModelsInfo::deleteServerConfig(const std::string &title)
{
    std::vector<ModelsInfo_s> allConfigs = getAllServerConfigs();
    // 移除指定标题的配置
    allConfigs.erase(std::remove_if(allConfigs.begin(), allConfigs.end(),
                                    [&title](const ModelsInfo_s &config) { return config.title == title; }),
                     allConfigs.end());

    // 保存更新后的配置
    saveAllServerConfigs(allConfigs);
}

// 保存所有服务器配置
void ModelsInfo::saveAllServerConfigs(const std::vector<ModelsInfo_s> &configs)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *serverArray = cJSON_CreateArray();
    for (const auto &config : configs) {
        // 为每个配置创建JSON对象
        cJSON *serverObject = cJSON_CreateObject();
        cJSON_AddStringToObject(serverObject, "title", config.title.c_str());
        cJSON_AddStringToObject(serverObject, "url", config.url.c_str());
        cJSON_AddStringToObject(serverObject, "apiKey", config.apiKey.c_str());
        cJSON_AddStringToObject(serverObject, "model", config.model.c_str());
        cJSON_AddItemToArray(serverArray, serverObject);
    }
    cJSON_AddItemToObject(root, "servers", serverArray);

    // 将JSON对象转换为字符串
    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);

    // 保存配置到文件
    QString configPath = QCoreApplication::applicationDirPath() + "/Server_config.json";
    saveFileContent(configPath, result);
}