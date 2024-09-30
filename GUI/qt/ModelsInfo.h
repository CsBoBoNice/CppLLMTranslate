/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:45:13
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 15:08:11
 * @FilePath: /CppLLMTranslate/GUI/qt/ModelsInfo.h
 * @Description: 
 * 
 * Copyright (c) 2024 by csbobo, All Rights Reserved. 
 */
#ifndef MODELSINFO_H
#define MODELSINFO_H

// 包含必要的头文件
#include <string>
#include <QString>
#include <vector>
#include "cJSON.h"

// 定义服务器配置结构体
struct ModelsInfo_s {
    std::string title;  // 配置标题
    std::string url;    // 服务器URL
    std::string apiKey; // API密钥
    std::string model;  // 模型名称
};

// ModelsInfo类定义
class ModelsInfo {
  public:
    // 获取ModelsInfo的单例实例
    static ModelsInfo &getInstance();

    // 禁用拷贝构造函数和赋值运算符，确保单例模式
    ModelsInfo(const ModelsInfo &) = delete;
    ModelsInfo &operator=(const ModelsInfo &) = delete;

    // 读取文件内容
    std::string readFileContent(const QString &filePath);
    // 保存文件内容
    bool saveFileContent(const QString &filePath, const std::string &content);

    // 获取默认服务器配置
    ModelsInfo_s getDefaultServerConfig();
    // 获取当前服务器配置
    ModelsInfo_s getCurrentServerConfig();
    // 设置服务器配置
    void setServerConfig(const ModelsInfo_s &newServerConfig);

    // 获取所有服务器配置
    std::vector<ModelsInfo_s> getAllServerConfigs();
    // 保存单个服务器配置
    void saveServerConfig(const ModelsInfo_s &serverConfig);
    // 删除指定标题的服务器配置
    void deleteServerConfig(const std::string &title);

  private:
    // 私有构造函数，防止外部直接创建实例
    ModelsInfo() = default;
    // 从JSON对象更新服务器配置
    void updateServerConfigFromJson(ModelsInfo_s &config, cJSON *jsonObject);
    // 保存所有服务器配置
    void saveAllServerConfigs(const std::vector<ModelsInfo_s> &configs);
};

#endif // MODELSINFO_H