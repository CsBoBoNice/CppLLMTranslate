/*
 * @Date: 2024-08-28 14:04:01
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-30 10:45:23
 * @FilePath: /CppLLMTranslate/GUI/qt/ConfigManager.cpp
 */
#include "ConfigManager.h"
#include <QApplication>
#include "cJSON.h"

std::mutex ConfigManager::mutex_;

ConfigManager &ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

std::string ConfigManager::readFile(const QString &filePath)
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

bool ConfigManager::saveFile(const QString &filePath, const std::string &content)
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

ServerInfo ConfigManager::DefaultGetServerIP()
{
    ServerInfo server_info{.url="http://172.0.0.1:59218/v1/chat/completions",.apiKey="888888", .model="glm-4-flash" };
    return server_info;
}

ServerInfo ConfigManager::GetServerIP()
{
    ServerInfo server_info{.url="http://172.0.0.1:59218/v1/chat/completions",.apiKey="888888", .model="glm-4-flash" };
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

void ConfigManager::SetServerIP(const ServerInfo &newServerInfo)
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

agreementInfo ConfigManager::Get_config_en_to_zh()
{
    if (!en_to_zh_change) {
        return en_to_zh_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = agreement::getInstance().default_en_to_zh();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/en_to_zh_config.json";
    QFile file(info_path);
    if (file.exists() == true) {
        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = agreement::getInstance().default_en_to_zh();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    en_to_zh_info = info; // 保存到全局变量
    return info;
}

void ConfigManager::Set_config_en_to_zh(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/en_to_zh_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    en_to_zh_change = true; // 设置改变标志
}

agreementInfo ConfigManager::Get_config_zh_to_en()
{
    if (!zh_to_en_change) {
        return zh_to_en_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = agreement::getInstance().default_zh_to_en();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/zh_to_en_config.json";
    QFile file(info_path);
    if (file.exists() == true) {
        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = agreement::getInstance().default_zh_to_en();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    zh_to_en_info = info; // 保存到全局变量
    return info;
}

void ConfigManager::Set_config_zh_to_en(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/zh_to_en_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    zh_to_en_change = true; // 设置改变标志
}

agreementInfo ConfigManager::Get_config_chat()
{
    if (!chat_change) {
        return chat_info; // 未改变，直接返回全局变量
    }
    agreementInfo info = agreement::getInstance().default_chat();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/chat_config.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = agreement::getInstance().default_chat();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    chat_info = info; // 保存到全局变量
    return info;
}

void ConfigManager::Set_config_chat(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/chat_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    chat_change = true; // 设置改变标志
}
