/*
 * @Date: 2024-08-23 13:55:54
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-23 14:12:12
 * @FilePath: /lvgl/main/src/agreement.cpp
 */
// agreement.cpp
#include "agreement.h"

agreement &agreement::getInstance()
{
    // 静态成员变量，用于存储单例对象
    static agreement instance;
    return instance;
}

void agreement::setStatus(AgreementStatus status)
{
    std::lock_guard<std::mutex> lock(mtx);
    this->status = status;
}

AgreementStatus agreement::getStatus() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return status;
}

agreementInfo agreement::parseJson(const std::string &jsonStr)
{
    agreementInfo info;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    if (root == nullptr) {
        // 解析失败
        return info;
    }

    cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
    cJSON *msg = cJSON_GetObjectItem(root, "msg");
    if (cmd != nullptr) {
        info.cmd = cmd->valueint;
    }
    if (msg != nullptr) {
        info.msg = msg->valuestring;
    }

    cJSON_Delete(root);
    return info;
}

std::string agreement::wrapToJson(const agreementInfo &info)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "cmd", info.cmd);
    cJSON_AddStringToObject(root, "msg", info.msg.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);
    return result;
}
