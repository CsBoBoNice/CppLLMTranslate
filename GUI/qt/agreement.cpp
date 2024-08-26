/*
 * @Date: 2024-08-23 13:55:54
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-23 17:31:02
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
    if (cmd != nullptr) {
        info.cmd = cmd->valueint;
    }

    cJSON *msg = cJSON_GetObjectItem(root, "msg");
    if (msg != nullptr) {
        info.msg = msg->valuestring;
    }

    cJSON *system = cJSON_GetObjectItem(root, "system");
    if (system != nullptr) {
        info.system = system->valuestring;
    }

    cJSON *chat_prefix = cJSON_GetObjectItem(root, "chat_prefix");
    if (chat_prefix != nullptr) {
        info.chat_prefix = chat_prefix->valuestring;
    }

    cJSON *chat_suffix = cJSON_GetObjectItem(root, "chat_suffix");
    if (chat_suffix != nullptr) {
        info.chat_suffix = chat_suffix->valuestring;
    }

    cJSON *user_msg_1 = cJSON_GetObjectItem(root, "user_msg_1");
    if (user_msg_1 != nullptr) {
        info.user_msg_1 = user_msg_1->valuestring;
    }

    cJSON *user_msg_2 = cJSON_GetObjectItem(root, "user_msg_2");
    if (user_msg_2 != nullptr) {
        info.user_msg_2 = user_msg_2->valuestring;
    }

    cJSON *user_msg_3 = cJSON_GetObjectItem(root, "user_msg_3");
    if (user_msg_3 != nullptr) {

        info.user_msg_3 = user_msg_3->valuestring;
    }

    cJSON *assistant_msg_1 = cJSON_GetObjectItem(root, "assistant_msg_1");
    if (assistant_msg_1 != nullptr) {
        info.assistant_msg_1 = assistant_msg_1->valuestring;
    }

    cJSON *assistant_msg_2 = cJSON_GetObjectItem(root, "assistant_msg_2");
    if (assistant_msg_2 != nullptr) {
        info.assistant_msg_2 = assistant_msg_2->valuestring;
    }

    cJSON *assistant_msg_3 = cJSON_GetObjectItem(root, "assistant_msg_3");
    if (assistant_msg_3 != nullptr) {
        info.assistant_msg_3 = assistant_msg_3->valuestring;
    }

    cJSON_Delete(root);
    return info;
}

std::string agreement::wrapToJson(const agreementInfo &info)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "cmd", info.cmd);
    cJSON_AddStringToObject(root, "msg", info.msg.c_str());
    cJSON_AddStringToObject(root, "system", info.system.c_str());
    cJSON_AddStringToObject(root, "chat_prefix", info.chat_prefix.c_str());
    cJSON_AddStringToObject(root, "chat_suffix", info.chat_suffix.c_str());
    cJSON_AddStringToObject(root, "user_msg_1", info.user_msg_1.c_str());
    cJSON_AddStringToObject(root, "user_msg_2", info.user_msg_2.c_str());
    cJSON_AddStringToObject(root, "user_msg_3", info.user_msg_3.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_1", info.assistant_msg_1.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_2", info.assistant_msg_2.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_3", info.assistant_msg_3.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);
    return result;
}
