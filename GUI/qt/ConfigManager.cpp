/*
 * @Date: 2024-08-28 14:04:01
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-02 16:32:25
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
    ServerInfo server_info{.url = "http://127.0.0.1:59218/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
    return server_info;
}

ServerInfo ConfigManager::GetServerIP()
{
    ServerInfo server_info{.url = "http://127.0.0.1:59218/v1/chat/completions", .apiKey = "888888", .model = "gpt-4o"};
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

agreementInfo ConfigManager::default_get_prompt_md_file()
{
    agreementInfo info;
    info.system = R"(
- Role: 专业翻译员
- Background: 用户需要将英文文档翻译成简体中文，并且需要保持文档的原始格式和专业术语的准确性。
- Profile: 你是一位经验丰富的翻译专家，精通英文和简体中文，擅长技术文档的翻译工作。
- Skills: 语言翻译能力、文档格式理解、专业术语准确性把控。
- Goals: 确保文档翻译后，格式、结构、链接和图片标记保持原样，同时专业术语翻译准确无误。
- Constrains: 翻译过程中，不得更改文档的任何非文本内容，包括链接、图片标记等。
- OutputFormat: 翻译后的文档以Markdown格式输出。翻译后仅输出翻译内容，无需其他解释说明。
- Workflow:
  1. 阅读并理解原文本内容及其格式。
  2. 翻译文本内容，同时保留所有格式化指令。
  3. 检查翻译后的文档，确保链接和图片标记未被更改，专业术语准确。
- Initialization: 欢迎来到文档翻译服务，我将帮助您将英文文档翻译成简体中文，同时保留所有格式化指令和文档结构。请发送您需要翻译的文档。
)";
    info.chat_prefix = R"(
[待翻译内容开始]
)";
    info.chat_suffix = R"(
[待翻译内容结束]

开始翻译任务，翻译后仅输出翻译内容，无需其他解释说明。

[优质翻译开始]
)";
    info.user_msg_1 = R"(
## Memory requirements
To decode and display a GIF animation the following amount of RAM is required:
- `LV_COLOR_DEPTH 8`: 3 x image width x image height
- `LV_COLOR_DEPTH 16`: 4 x image width x image height
- `LV_COLOR_DEPTH 32`: 5 x image width x image height

## Example
```eval_rst
.. include:: ../../examples/libs/gif/index.rst
```

## API

```eval_rst
.. doxygenfile:: lv_gif.h
  :project: lvgl
```
)";
    info.user_msg_2.clear();
    info.user_msg_3.clear();

    info.assistant_msg_1 = R"(
## 内存要求

为了解码和显示GIF动画，需要以下数量的RAM：
- `LV_COLOR_DEPTH 8`: 3 x 图像宽度 x 图像高度
- `LV_COLOR_DEPTH 16`: 4 x 图像宽度 x 图像高度
- `LV_COLOR_DEPTH 32`: 5 x 图像宽度 x 图像高度

## 示例

```eval_rst
.. include:: ../../examples/libs/gif/index.rst
```

## API

```eval_rst
.. doxygenfile:: lv_gif.h
  :project: lvgl
```
)";
    info.assistant_msg_2.clear();
    info.assistant_msg_3.clear();
    return info;
}

agreementInfo ConfigManager::get_prompt_md_file()
{
    if (!md_file_change) {
        return md_file_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_get_prompt_md_file();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_md.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_get_prompt_md_file();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    md_file_info = info; // 保存到全局变量
    return info;
}

void ConfigManager::set_prompt_md_file(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_md.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    md_file_change = true; // 设置改变标志
}
