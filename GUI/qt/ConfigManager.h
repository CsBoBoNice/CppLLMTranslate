/*
 * @Date: 2024-08-28 14:27:38
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 14:39:04
 * @FilePath: /CppLLMTranslate/GUI/qt/ConfigManager.h
 */
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <mutex>
#include <string>
#include <mutex>
#include "agreement.h"

struct ServerInfo {
    std::string url;
    std::string apiKey;
    std::string model;
};

class ConfigManager {
  public:
    // 获取单例对象的静态方法
    static ConfigManager &getInstance();

    // 禁止拷贝构造和赋值操作
    ConfigManager(const ConfigManager &) = delete;
    ConfigManager &operator=(const ConfigManager &) = delete;

    // 读取文件内容并返回 std::string
    std::string readFile(const QString &filePath);

    // 将字符串内容保存到文件
    bool saveFile(const QString &filePath, const std::string &content);

    ServerInfo DefaultGetServerIP();
    ServerInfo GetServerIP();
    void SetServerIP(const ServerInfo &newServerInfo);

    agreementInfo Get_config_en_to_zh();
    void Set_config_en_to_zh(const agreementInfo &Info);

    agreementInfo Get_config_zh_to_en();
    void Set_config_zh_to_en(const agreementInfo &Info);

    agreementInfo Get_config_chat();
    void Set_config_chat(const agreementInfo &Info);

    agreementInfo en_to_zh_info;
    agreementInfo zh_to_en_info;
    agreementInfo chat_info;

    bool en_to_zh_change = true;
    bool zh_to_en_change = true;
    bool chat_change = true;

  private:
    // 私有构造函数
    ConfigManager() = default;

    // 用于线程安全的互斥锁
    static std::mutex mutex_;
};

#endif // CONFIGMANAGER_H
