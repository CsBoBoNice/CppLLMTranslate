/*
 * @Date: 2024-08-28 14:27:38
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-06 10:23:11
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

struct TranslationProgressConfig {
    int file_index;      // 文件计数
    int paragraph_index; // 段落计数
};

struct TranslationSetInfo {

    int paragraph_effective;
    int paragraph_min;
    int paragraph_max;

    std::string Input_file_path;
    std::string Output_file_path;
    std::string Cut_file_path;
    std::string Reference_file_path;
    std::string Success_file_path;
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

    bool md_file_change = true;
    agreementInfo md_file_info;
    agreementInfo default_get_prompt_md_file();
    agreementInfo get_prompt_md_file();
    void set_prompt_md_file(const agreementInfo &Info);

    bool txt_file_change = true;
    agreementInfo txt_file_info;
    agreementInfo default_get_prompt_txt_file();
    agreementInfo get_prompt_txt_file();
    void set_prompt_txt_file(const agreementInfo &Info);

    bool rst_file_change = true;
    agreementInfo rst_file_info;
    agreementInfo default_get_prompt_rst_file();
    agreementInfo get_prompt_rst_file();
    void set_prompt_rst_file(const agreementInfo &Info);

    bool h_file_change = true;
    agreementInfo h_file_info;
    agreementInfo default_get_prompt_h_file();
    agreementInfo get_prompt_h_file();
    void set_prompt_h_file(const agreementInfo &Info);

    bool TranslationSetInfo_change = true;
    TranslationSetInfo TranslationSetInfo_info;
    TranslationSetInfo default_get_TranslationSetInfo();
    TranslationSetInfo get_TranslationSetInfo();
    void set_TranslationSetInfo(const TranslationSetInfo &Info);

    bool TranslationProgressConfig_change = true;
    TranslationProgressConfig TranslationProgressConfig_info;
    TranslationProgressConfig default_get_TranslationProgressConfig();
    TranslationProgressConfig get_TranslationProgressConfig(std::string Input_file_path);
    void set_TranslationProgressConfig(const TranslationProgressConfig &Info, std::string Input_file_path);

  private:
    // 私有构造函数
    ConfigManager() = default;

    // 用于线程安全的互斥锁
    static std::mutex mutex_;
};

#endif // CONFIGMANAGER_H
