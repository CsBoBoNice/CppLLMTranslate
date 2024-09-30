/*
 * @Author: csbobo 751541594@qq.com
 * @Date: 2024-09-30 13:45:13
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 14:56:03
 * @FilePath: /CppLLMTranslate/GUI/qt/ModelsInfo.h
 * @Description: 
 * 
 * Copyright (c) 2024 by csbobo, All Rights Reserved. 
 */
#ifndef MODELSINFO_H
#define MODELSINFO_H

#include <string>
#include <QString>
#include <vector>
#include "cJSON.h"

struct ModelsInfo_s {
    std::string title;
    std::string url;
    std::string apiKey;
    std::string model;
};

class ModelsInfo {
  public:
    static ModelsInfo &getInstance();

    ModelsInfo(const ModelsInfo &) = delete;
    ModelsInfo &operator=(const ModelsInfo &) = delete;

    std::string readFileContent(const QString &filePath);
    bool saveFileContent(const QString &filePath, const std::string &content);

    ModelsInfo_s getDefaultServerConfig();
    ModelsInfo_s getCurrentServerConfig();
    void setServerConfig(const ModelsInfo_s &newServerConfig);

    std::vector<ModelsInfo_s> getAllServerConfigs();
    void saveServerConfig(const ModelsInfo_s &serverConfig);
    void deleteServerConfig(const std::string &title);

  private:
    ModelsInfo() = default;
    void updateServerConfigFromJson(ModelsInfo_s &config, cJSON *jsonObject);
    void saveAllServerConfigs(const std::vector<ModelsInfo_s> &configs);
};

#endif // MODELSINFO_H