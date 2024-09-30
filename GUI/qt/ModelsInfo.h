/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:45:13
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 14:19:39
 * @FilePath: /CppLLMTranslate/GUI/qt/ModelsInfo.h
 * @Description: 
 * 
 * Copyright (c) 2024 by csbobo, All Rights Reserved. 
 */
#ifndef MODELSINFO_H
#define MODELSINFO_H

#include <string>
#include <QString>
#include <vector>  // Added for vector

struct ModelsInfo_s {
    std::string title;  // 新增
    std::string url;
    std::string apiKey;
    std::string model;
};

class ModelsInfo {
  public:
    // 获取单例对象的静态方法
    static ModelsInfo &getInstance();

    // 禁止拷贝构造和赋值操作
    ModelsInfo(const ModelsInfo &) = delete;
    ModelsInfo &operator=(const ModelsInfo &) = delete;

    // 读取文件内容并返回 std::string
    std::string readFile(const QString &filePath);

    // 将字符串内容保存到文件
    bool saveFile(const QString &filePath, const std::string &content);

    ModelsInfo_s DefaultGetServerIP();
    ModelsInfo_s GetServerIP();
    void SetServerIP(const ModelsInfo_s &newServerInfo);

    // 新增方法
    std::vector<ModelsInfo_s> GetAllServerIPs();  // 新增
    void SaveServerIP(const ModelsInfo_s &serverInfo);  // 新增
    void DeleteServerIP(const std::string &title);  // 新增

  private:
    // 私有构造函数
    ModelsInfo() = default;
};

#endif // MODELSINFO_H